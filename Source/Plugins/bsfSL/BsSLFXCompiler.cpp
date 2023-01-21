//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "BsSLFXCompiler.h"
#include "RenderAPI/BsGpuProgram.h"
#include <regex>
#include "Material/BsShader.h"
#include "Material/BsTechnique.h"
#include "Material/BsPass.h"
#include "RenderAPI/BsSamplerState.h"
#include "RenderAPI/BsRenderAPI.h"
#include "Debug/BsDebug.h"
#include "Material/BsShaderManager.h"
#include "Material/BsShaderInclude.h"
#include "Math/BsMatrix4.h"
#include "Resources/BsBuiltinResources.h"
#include "Material/BsShaderVariation.h"
#include "Renderer/BsRenderer.h"
#include "Renderer/BsRendererManager.h"
#include "FileSystem/BsFileSystem.h"
#include "FileSystem/BsDataStream.h"

#define XSC_ENABLE_LANGUAGE_EXT 1
#include "Xsc/Xsc.h"

extern "C" {
#include "BsMMAlloc.h"

#define YY_NO_UNISTD_H 1
#include "BsParserFX.h"
#include "BsLexerFX.h"
}

using namespace std;
using namespace bs;

// Print out the FX AST, only for debug purposes
void SLFXDebugPrint(ASTFXNode* node, String indent)
{
	B3D_LOG(Info, BSLCompiler, indent + "NODE {0}", node->Type);

	for(int i = 0; i < node->Options->Count; i++)
	{
		OptionDataType odt = OPTION_LOOKUP[(int)node->Options->Entries[i].Type].DataType;
		if(odt == ODT_Complex)
		{
			B3D_LOG(Info, BSLCompiler, "{0}{1}. {2}", indent, i, node->Options->Entries[i].Type);
			SLFXDebugPrint(node->Options->Entries[i].Value.NodePtr, indent + "\t");
			continue;
		}

		String value;
		switch(odt)
		{
		case ODT_Bool:
			value = ToString(node->Options->Entries[i].Value.IntValue != 0);
			break;
		case ODT_Int:
			value = ToString(node->Options->Entries[i].Value.IntValue);
			break;
		case ODT_Float:
			value = ToString(node->Options->Entries[i].Value.FloatValue);
			break;
		case ODT_String:
			value = node->Options->Entries[i].Value.StrValue;
			break;
		case ODT_Matrix:
			{
				Matrix4 mat4 = *(Matrix4*)(node->Options->Entries[i].Value.MatrixValue);
				value = ToString(mat4);
			}
			break;
		default:
			break;
		}

		B3D_LOG(Info, BSLCompiler, "{0}{1}. {2} = {3}", indent, i, node->Options->Entries[i].Type, value);
	}
}

class XscLog : public Xsc::Log
{
public:
	void SubmitReport(const Xsc::Report& report) override
	{
		switch(report.Type())
		{
		case Xsc::ReportTypes::Info:
			mInfos.push_back({ FullIndent(), report });
			break;
		case Xsc::ReportTypes::Warning:
			mWarnings.push_back({ FullIndent(), report });
			break;
		case Xsc::ReportTypes::Error:
			mErrors.push_back({ FullIndent(), report });
			break;
		}
	}

	void GetMessages(StringStream& output)
	{
		PrintAndClearReports(output, mInfos);
		PrintAndClearReports(output, mWarnings, (mWarnings.size() == 1 ? "WARNING" : "WARNINGS"));
		PrintAndClearReports(output, mErrors, (mErrors.size() == 1 ? "ERROR" : "ERRORS"));
	}

private:
	struct IndentReport
	{
		std::string Indent;
		Xsc::Report Report;
	};

	static void PrintMultiLineString(StringStream& output, const std::string& str, const std::string& indent)
	{
		// Determine at which position the actual text begins (excluding the "error (X:Y) : " or the like)
		auto textStartPos = str.find(" : ");
		if(textStartPos != std::string::npos)
			textStartPos += 3;
		else
			textStartPos = 0;

		std::string newLineIndent(textStartPos, ' ');

		size_t start = 0;
		bool useNewLineIndent = false;
		while(start < str.size())
		{
			output << indent;

			if(useNewLineIndent)
				output << newLineIndent;

			// Print next line
			auto end = str.find('\n', start);

			if(end != std::string::npos)
			{
				output << str.substr(start, end - start);
				start = end + 1;
			}
			else
			{
				output << str.substr(start);
				start = end;
			}

			output << std::endl;
			useNewLineIndent = true;
		}
	}

	void PrintReport(StringStream& output, const IndentReport& r)
	{
		// Print optional context description
		if(!r.Report.Context().empty())
			PrintMultiLineString(output, r.Report.Context(), r.Indent);

		// Print report message
		const auto& msg = r.Report.Message();
		PrintMultiLineString(output, msg, r.Indent);

		// Print optional line and line-marker
		if(r.Report.HasLine())
		{
			const auto& line = r.Report.Line();
			const auto& marker = r.Report.Marker();

			// Print line
			output << r.Indent << line << std::endl;

			// Print line marker
			output << r.Indent << marker << std::endl;
		}

		// Print optional hints
		for(const auto& hint : r.Report.GetHints())
			output << r.Indent << hint << std::endl;
	}

	void PrintAndClearReports(StringStream& output, Vector<IndentReport>& reports, const String& headline = "")
	{
		if(!reports.empty())
		{
			if(!headline.empty())
			{
				String s = ToString((u32)reports.size()) + " " + headline;
				output << s << std::endl;
				output << String(s.size(), '-') << std::endl;
			}

			for(const auto& r : reports)
				PrintReport(output, r);

			reports.clear();
		}
	}

	Vector<IndentReport> mInfos;
	Vector<IndentReport> mWarnings;
	Vector<IndentReport> mErrors;
};

GpuParameterObjectType ReflTypeToTextureType(Xsc::Reflection::BufferType type)
{
	switch(type)
	{
	case Xsc::Reflection::BufferType::RWTexture1D: return GPOT_RWTEXTURE1D;
	case Xsc::Reflection::BufferType::RWTexture1DArray: return GPOT_RWTEXTURE1DARRAY;
	case Xsc::Reflection::BufferType::RWTexture2D: return GPOT_RWTEXTURE2D;
	case Xsc::Reflection::BufferType::RWTexture2DArray: return GPOT_RWTEXTURE2DARRAY;
	case Xsc::Reflection::BufferType::RWTexture3D: return GPOT_RWTEXTURE3D;
	case Xsc::Reflection::BufferType::Texture1D: return GPOT_TEXTURE1D;
	case Xsc::Reflection::BufferType::Texture1DArray: return GPOT_TEXTURE1DARRAY;
	case Xsc::Reflection::BufferType::Texture2D: return GPOT_TEXTURE2D;
	case Xsc::Reflection::BufferType::Texture2DArray: return GPOT_TEXTURE2DARRAY;
	case Xsc::Reflection::BufferType::Texture3D: return GPOT_TEXTURE3D;
	case Xsc::Reflection::BufferType::TextureCube: return GPOT_TEXTURECUBE;
	case Xsc::Reflection::BufferType::TextureCubeArray: return GPOT_TEXTURECUBEARRAY;
	case Xsc::Reflection::BufferType::Texture2DMS: return GPOT_TEXTURE2DMS;
	case Xsc::Reflection::BufferType::Texture2DMSArray: return GPOT_TEXTURE2DMSARRAY;
	default: return GPOT_UNKNOWN;
	}
}

GpuParameterObjectType ReflTypeToBufferType(Xsc::Reflection::BufferType type)
{
	switch(type)
	{
	case Xsc::Reflection::BufferType::Buffer: return GPOT_RWTYPED_BUFFER;
	case Xsc::Reflection::BufferType::StructuredBuffer: return GPOT_STRUCTURED_BUFFER;
	case Xsc::Reflection::BufferType::ByteAddressBuffer: return GPOT_BYTE_BUFFER;
	case Xsc::Reflection::BufferType::RWBuffer: return GPOT_RWTYPED_BUFFER;
	case Xsc::Reflection::BufferType::RWStructuredBuffer: return GPOT_RWSTRUCTURED_BUFFER;
	case Xsc::Reflection::BufferType::RWByteAddressBuffer: return GPOT_RWBYTE_BUFFER;
	case Xsc::Reflection::BufferType::AppendStructuredBuffer: return GPOT_RWAPPEND_BUFFER;
	case Xsc::Reflection::BufferType::ConsumeStructuredBuffer: return GPOT_RWCONSUME_BUFFER;
	default: return GPOT_UNKNOWN;
	}
}

GpuDataParameterType ReflTypeToDataType(Xsc::Reflection::DataType type)
{
	switch(type)
	{
	case Xsc::Reflection::DataType::Bool: return GPDT_BOOL;
	case Xsc::Reflection::DataType::Float: return GPDT_FLOAT1;
	case Xsc::Reflection::DataType::Float2: return GPDT_FLOAT2;
	case Xsc::Reflection::DataType::Float3: return GPDT_FLOAT3;
	case Xsc::Reflection::DataType::Float4: return GPDT_FLOAT4;
	case Xsc::Reflection::DataType::Double: return GPDT_DOUBLE1;
	case Xsc::Reflection::DataType::Double2: return GPDT_DOUBLE2;
	case Xsc::Reflection::DataType::Double3: return GPDT_DOUBLE3;
	case Xsc::Reflection::DataType::Double4: return GPDT_DOUBLE4;
	case Xsc::Reflection::DataType::Half: return GPDT_HALF1;
	case Xsc::Reflection::DataType::Half2: return GPDT_HALF2;
	case Xsc::Reflection::DataType::Half3: return GPDT_HALF3;
	case Xsc::Reflection::DataType::Half4: return GPDT_HALF4;
	case Xsc::Reflection::DataType::Int: return GPDT_INT1;
	case Xsc::Reflection::DataType::Int2: return GPDT_INT2;
	case Xsc::Reflection::DataType::Int3: return GPDT_INT3;
	case Xsc::Reflection::DataType::Int4: return GPDT_INT4;
	case Xsc::Reflection::DataType::UInt: return GPDT_UINT1;
	case Xsc::Reflection::DataType::UInt2: return GPDT_UINT2;
	case Xsc::Reflection::DataType::UInt3: return GPDT_UINT3;
	case Xsc::Reflection::DataType::UInt4: return GPDT_UINT4;
	case Xsc::Reflection::DataType::Float2x2: return GPDT_MATRIX_2X2;
	case Xsc::Reflection::DataType::Float2x3: return GPDT_MATRIX_2X3;
	case Xsc::Reflection::DataType::Float2x4: return GPDT_MATRIX_2X4;
	case Xsc::Reflection::DataType::Float3x2: return GPDT_MATRIX_3X4;
	case Xsc::Reflection::DataType::Float3x3: return GPDT_MATRIX_3X3;
	case Xsc::Reflection::DataType::Float3x4: return GPDT_MATRIX_3X4;
	case Xsc::Reflection::DataType::Float4x2: return GPDT_MATRIX_4X2;
	case Xsc::Reflection::DataType::Float4x3: return GPDT_MATRIX_4X3;
	case Xsc::Reflection::DataType::Float4x4: return GPDT_MATRIX_4X4;
	case Xsc::Reflection::DataType::Double2x2: return GPDT_DOUBLE_MATRIX_2X2;
	case Xsc::Reflection::DataType::Double2x3: return GPDT_DOUBLE_MATRIX_2X3;
	case Xsc::Reflection::DataType::Double2x4: return GPDT_DOUBLE_MATRIX_2X4;
	case Xsc::Reflection::DataType::Double3x2: return GPDT_DOUBLE_MATRIX_3X4;
	case Xsc::Reflection::DataType::Double3x3: return GPDT_DOUBLE_MATRIX_3X3;
	case Xsc::Reflection::DataType::Double3x4: return GPDT_DOUBLE_MATRIX_3X4;
	case Xsc::Reflection::DataType::Double4x2: return GPDT_DOUBLE_MATRIX_4X2;
	case Xsc::Reflection::DataType::Double4x3: return GPDT_DOUBLE_MATRIX_4X3;
	case Xsc::Reflection::DataType::Double4x4: return GPDT_DOUBLE_MATRIX_4X4;
	case Xsc::Reflection::DataType::Half2x2: return GPDT_HALF_MATRIX_2X2;
	case Xsc::Reflection::DataType::Half2x3: return GPDT_HALF_MATRIX_2X3;
	case Xsc::Reflection::DataType::Half2x4: return GPDT_HALF_MATRIX_2X4;
	case Xsc::Reflection::DataType::Half3x2: return GPDT_HALF_MATRIX_3X4;
	case Xsc::Reflection::DataType::Half3x3: return GPDT_HALF_MATRIX_3X3;
	case Xsc::Reflection::DataType::Half3x4: return GPDT_HALF_MATRIX_3X4;
	case Xsc::Reflection::DataType::Half4x2: return GPDT_HALF_MATRIX_4X2;
	case Xsc::Reflection::DataType::Half4x3: return GPDT_HALF_MATRIX_4X3;
	case Xsc::Reflection::DataType::Half4x4: return GPDT_HALF_MATRIX_4X4;
	default: return GPDT_UNKNOWN;
	}
}

static HTexture GetBuiltinTexture(u32 idx)
{
	if(idx == 1)
		return BuiltinResources::GetTexture(BuiltinTexture::White);
	else if(idx == 2)
		return BuiltinResources::GetTexture(BuiltinTexture::Black);
	else if(idx == 3)
		return BuiltinResources::GetTexture(BuiltinTexture::Normal);

	return HTexture();
}

static HTexture GetBuiltin3DTexture(u32 index)
{
	if (index == 1)
		return BuiltinResources::GetTexture(BuiltinTexture::White3D);
	else if (index == 2)
		return BuiltinResources::GetTexture(BuiltinTexture::Black3D);

	return HTexture();
}


u32 GetStructSize(i32 structIdx, const std::vector<Xsc::Reflection::Struct>& structLookup)
{
	if(structIdx < 0 || structIdx >= (i32)structLookup.size())
		return 0;

	u32 size = 0;

	const Xsc::Reflection::Struct& structInfo = structLookup[structIdx];
	for(auto& entry : structInfo.members)
	{
		if(entry.type == Xsc::Reflection::VariableType::Variable)
		{
			// Note: We're ignoring any padding. Since we can't guarantee the padding will be same for structs across
			// different render backends it's expected for the user to set up structs in such a way so padding is not
			// needed (i.e. add padding variables manually).
			GpuDataParameterType type = ReflTypeToDataType((Xsc::Reflection::DataType)entry.baseType);

			const GpuDataParameterTypeInformation& typeInfo = GpuParams::kParamSizes.Lookup[(int)type];
			size += typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize * entry.arraySize;
		}
		else if(entry.type == Xsc::Reflection::VariableType::Struct)
			size += GetStructSize(entry.baseType, structLookup);
	}

	return size;
}

TextureAddressingMode ParseTexAddrMode(Xsc::Reflection::TextureAddressMode addrMode)
{
	switch(addrMode)
	{
	case Xsc::Reflection::TextureAddressMode::Border:
		return TAM_BORDER;
	case Xsc::Reflection::TextureAddressMode::Clamp:
		return TAM_CLAMP;
	case Xsc::Reflection::TextureAddressMode::Mirror:
	case Xsc::Reflection::TextureAddressMode::MirrorOnce:
		return TAM_MIRROR;
	case Xsc::Reflection::TextureAddressMode::Wrap:
	default:
		return TAM_WRAP;
	}
}

CompareFunction ParseCompFunction(Xsc::Reflection::ComparisonFunc compFunc)
{
	switch(compFunc)
	{
	case Xsc::Reflection::ComparisonFunc::Always:
	default:
		return CMPF_ALWAYS_PASS;
	case Xsc::Reflection::ComparisonFunc::Never:
		return CMPF_ALWAYS_FAIL;
	case Xsc::Reflection::ComparisonFunc::Equal:
		return CMPF_EQUAL;
	case Xsc::Reflection::ComparisonFunc::Greater:
		return CMPF_GREATER;
	case Xsc::Reflection::ComparisonFunc::GreaterEqual:
		return CMPF_GREATER_EQUAL;
	case Xsc::Reflection::ComparisonFunc::Less:
		return CMPF_LESS;
	case Xsc::Reflection::ComparisonFunc::LessEqual:
		return CMPF_LESS_EQUAL;
	case Xsc::Reflection::ComparisonFunc::NotEqual:
		return CMPF_NOT_EQUAL;
	}
}

static SAMPLER_STATE_DESC ParseSamplerState(const Xsc::Reflection::SamplerState& samplerReflectionInformation)
{
	SAMPLER_STATE_DESC samplerCreateInformation;

	samplerCreateInformation.AddressMode.U = ParseTexAddrMode(samplerReflectionInformation.addressU);
	samplerCreateInformation.AddressMode.V = ParseTexAddrMode(samplerReflectionInformation.addressV);
	samplerCreateInformation.AddressMode.W = ParseTexAddrMode(samplerReflectionInformation.addressW);

	samplerCreateInformation.BorderColor[0] = samplerReflectionInformation.borderColor[0];
	samplerCreateInformation.BorderColor[1] = samplerReflectionInformation.borderColor[1];
	samplerCreateInformation.BorderColor[2] = samplerReflectionInformation.borderColor[2];
	samplerCreateInformation.BorderColor[3] = samplerReflectionInformation.borderColor[3];

	samplerCreateInformation.ComparisonFunc = ParseCompFunction(samplerReflectionInformation.comparisonFunc);
	samplerCreateInformation.MaxAniso = samplerReflectionInformation.maxAnisotropy;
	samplerCreateInformation.MipMax = samplerReflectionInformation.maxLOD;
	samplerCreateInformation.MipMin = samplerReflectionInformation.minLOD;
	samplerCreateInformation.MipmapBias = samplerReflectionInformation.mipLODBias;

	switch(samplerReflectionInformation.filter)
	{
	case Xsc::Reflection::Filter::MinMagMipPoint:
	case Xsc::Reflection::Filter::ComparisonMinMagMipPoint:
		samplerCreateInformation.MinFilter = FO_POINT;
		samplerCreateInformation.MagFilter = FO_POINT;
		samplerCreateInformation.MipFilter = FO_POINT;
		break;
	case Xsc::Reflection::Filter::MinMagPointMipLinear:
	case Xsc::Reflection::Filter::ComparisonMinMagPointMipLinear:
		samplerCreateInformation.MinFilter = FO_POINT;
		samplerCreateInformation.MagFilter = FO_POINT;
		samplerCreateInformation.MipFilter = FO_LINEAR;
		break;
	case Xsc::Reflection::Filter::MinPointMagLinearMipPoint:
	case Xsc::Reflection::Filter::ComparisonMinPointMagLinearMipPoint:
		samplerCreateInformation.MinFilter = FO_POINT;
		samplerCreateInformation.MagFilter = FO_LINEAR;
		samplerCreateInformation.MipFilter = FO_POINT;
		break;
	case Xsc::Reflection::Filter::MinPointMagMipLinear:
	case Xsc::Reflection::Filter::ComparisonMinPointMagMipLinear:
		samplerCreateInformation.MinFilter = FO_POINT;
		samplerCreateInformation.MagFilter = FO_LINEAR;
		samplerCreateInformation.MipFilter = FO_LINEAR;
		break;
	case Xsc::Reflection::Filter::MinLinearMagMipPoint:
	case Xsc::Reflection::Filter::ComparisonMinLinearMagMipPoint:
		samplerCreateInformation.MinFilter = FO_LINEAR;
		samplerCreateInformation.MagFilter = FO_POINT;
		samplerCreateInformation.MipFilter = FO_POINT;
		break;
	case Xsc::Reflection::Filter::MinLinearMagPointMipLinear:
	case Xsc::Reflection::Filter::ComparisonMinLinearMagPointMipLinear:
		samplerCreateInformation.MinFilter = FO_LINEAR;
		samplerCreateInformation.MagFilter = FO_POINT;
		samplerCreateInformation.MipFilter = FO_LINEAR;
		break;
	case Xsc::Reflection::Filter::MinMagLinearMipPoint:
	case Xsc::Reflection::Filter::ComparisonMinMagLinearMipPoint:
		samplerCreateInformation.MinFilter = FO_LINEAR;
		samplerCreateInformation.MagFilter = FO_LINEAR;
		samplerCreateInformation.MipFilter = FO_POINT;
		break;
	case Xsc::Reflection::Filter::MinMagMipLinear:
	case Xsc::Reflection::Filter::ComparisonMinMagMipLinear:
		samplerCreateInformation.MinFilter = FO_LINEAR;
		samplerCreateInformation.MagFilter = FO_LINEAR;
		samplerCreateInformation.MipFilter = FO_LINEAR;
		break;
	case Xsc::Reflection::Filter::Anisotropic:
	case Xsc::Reflection::Filter::ComparisonAnisotropic:
		samplerCreateInformation.MinFilter = FO_ANISOTROPIC;
		samplerCreateInformation.MagFilter = FO_ANISOTROPIC;
		samplerCreateInformation.MipFilter = FO_ANISOTROPIC;
		break;
	default:
		break;
	}

	return samplerCreateInformation;
}

static bool ParseParameters(const Xsc::Reflection::ReflectionData& reflectionData, BSLFXCompileResult& outCompileResult, ShaderCreateInformation& outShaderCreateInformation)
{
	for(auto& entry : reflectionData.uniforms)
	{
		if((entry.flags & Xsc::Reflection::Uniform::Flags::Internal) != 0)
			continue;

		String ident = entry.ident.c_str();
		bool isBlockHiddenInInspector = false;
		auto parseCommonAttributes = [&entry, &ident, &outShaderCreateInformation, &isBlockHiddenInInspector]()
		{
			if(!entry.readableName.empty())
			{
				ShaderParameterAttribute attribute;
				attribute.Value.assign(entry.readableName.data(), entry.readableName.size());
				attribute.NextParameterIndex = (u32)-1;
				attribute.Type = ShaderParamAttributeType::Name;

				outShaderCreateInformation.SetParameterAttribute(ident, attribute);
			}

			if((entry.flags & Xsc::Reflection::Uniform::Flags::HideInInspector) != 0 || isBlockHiddenInInspector)
			{
				ShaderParameterAttribute attribute;
				attribute.NextParameterIndex = (u32)-1;
				attribute.Type = ShaderParamAttributeType::HideInInspector;

				outShaderCreateInformation.SetParameterAttribute(ident, attribute);
			}

			if((entry.flags & Xsc::Reflection::Uniform::Flags::HDR) != 0)
			{
				ShaderParameterAttribute attribute;
				attribute.NextParameterIndex = (u32)-1;
				attribute.Type = ShaderParamAttributeType::HDR;

				outShaderCreateInformation.SetParameterAttribute(ident, attribute);
			}
		};

		switch(entry.type)
		{
		case Xsc::Reflection::VariableType::UniformBuffer:
			outShaderCreateInformation.SetParamBlockAttribs(entry.ident.c_str(), false, GBU_STATIC);
			break;
		case Xsc::Reflection::VariableType::Buffer:
			{
				GpuParameterObjectType objType = ReflTypeToTextureType((Xsc::Reflection::BufferType)entry.baseType);
				if(objType != GPOT_UNKNOWN)
				{
					const bool hasDefaultValue = entry.defaultValue == -1;
					HTexture defaultValue;

					if (!hasDefaultValue)
					{
						const Xsc::Reflection::DefaultValue& reflectedDefaultValue = reflectionData.defaultValues[entry.defaultValue];
						defaultValue = objType == GPOT_TEXTURE3D ? GetBuiltin3DTexture(reflectedDefaultValue.integer) : GetBuiltinTexture(reflectedDefaultValue.integer);
					}

					// Warn if parameter was already registered in some previous variation with a different value
					if(auto foundTextureParameter = outShaderCreateInformation.TextureParams.find(ident); foundTextureParameter != outShaderCreateInformation.TextureParams.end())
					{
						const bool isExistingValueDefault = foundTextureParameter->second.DefaultValueIndex == ~0u;
						if (hasDefaultValue != isExistingValueDefault)
						{
							outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Texture parameter '{0}' has a different default value across variations.", entry.ident.c_str());
							return false;
						}

						if (!hasDefaultValue)
						{
							const HTexture existingTexture = outShaderCreateInformation.TextureDefaultValues[foundTextureParameter->second.DefaultValueIndex];
							if (existingTexture != defaultValue)
							{
								outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Texture parameter '{0}' has a different default value across variations.", entry.ident.c_str());
								return false;
							}
						}

						continue;
					}

					if(entry.defaultValue == -1)
						outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, objType, StringID::kNone, entry.arraySize));
					else
						outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, objType, StringID::kNone, entry.arraySize), defaultValue);

					parseCommonAttributes();
				}
				else
				{
					// Ignore parameters that were already registered in some previous variation. Note that this implies
					// you cannot have same names for different parameters in different variations.
					if(outShaderCreateInformation.BufferParams.find(ident) != outShaderCreateInformation.BufferParams.end())
						continue;

					objType = ReflTypeToBufferType((Xsc::Reflection::BufferType)entry.baseType);
					outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, objType, StringID::kNone, entry.arraySize));

					parseCommonAttributes();
				}
			}
			break;
		case Xsc::Reflection::VariableType::Sampler:
			{
				if(auto foundSamplerReflectionData = reflectionData.samplerStates.find(entry.ident); foundSamplerReflectionData != reflectionData.samplerStates.end())
				{
					SAMPLER_STATE_DESC defaultSamplerStateCreateInformation;
					if (foundSamplerReflectionData->second.isNonDefault)
						defaultSamplerStateCreateInformation = ParseSamplerState(foundSamplerReflectionData->second);

					if (auto foundSamplerParameter = outShaderCreateInformation.SamplerParams.find(ident); foundSamplerParameter != outShaderCreateInformation.SamplerParams.end())
					{
						const bool isExistingValueNonDefault = foundSamplerParameter->second.DefaultValueIndex != ~0u;
						if (foundSamplerReflectionData->second.isNonDefault != isExistingValueNonDefault)
						{
							outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Sampler parameter '{0}' has a different default value across variations.", entry.ident.c_str());
							return false;
						}

						if (foundSamplerReflectionData->second.isNonDefault)
						{
							const SPtr<const SamplerState> existingSamplerState = outShaderCreateInformation.SamplerDefaultValues[foundSamplerParameter->second.DefaultValueIndex];
							if (existingSamplerState->GetProperties().GetDesc() != defaultSamplerStateCreateInformation)
							{
								outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Sampler parameter '{0}' has a different default value across variations.", entry.ident.c_str());
								return false;
							}
						}

						continue;
					}

					const String alias = foundSamplerReflectionData->second.alias.c_str();
					if(foundSamplerReflectionData->second.isNonDefault)
					{
						const SPtr<SamplerState> defaultValue = SamplerState::Create(defaultSamplerStateCreateInformation);
						outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, GPOT_SAMPLER2D), defaultValue);

						if(!alias.empty())
							outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, alias, GPOT_SAMPLER2D), defaultValue);
					}
					else
					{
						// Ignore parameters that were already registered in some previous variation. Note that this implies
						// you cannot have same names for different parameters in different variations.
						if (outShaderCreateInformation.SamplerParams.find(ident) != outShaderCreateInformation.SamplerParams.end())
							continue;

						outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, GPOT_SAMPLER2D));

						if(!alias.empty())
							outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, alias, GPOT_SAMPLER2D));
					}
				}
				else
				{
					outShaderCreateInformation.AddParameter(ShaderObjectParameterInformation(ident, ident, GPOT_SAMPLER2D));
				}
				break;
			}
		case Xsc::Reflection::VariableType::Variable:
			{
				bool isBlockInternal = false;
				if(entry.uniformBlock != -1)
				{
					std::string blockName = reflectionData.constantBuffers[entry.uniformBlock].ident;
					for(auto& uniform : reflectionData.uniforms)
					{
						if(uniform.type == Xsc::Reflection::VariableType::UniformBuffer && uniform.ident == blockName)
						{
							isBlockInternal = (uniform.flags & Xsc::Reflection::Uniform::Flags::Internal) != 0;
							isBlockHiddenInInspector = (uniform.flags & Xsc::Reflection::Uniform::Flags::HideInInspector) != 0;
							break;
						}
					}
				}

				if(!isBlockInternal)
				{
					GpuDataParameterType type = ReflTypeToDataType((Xsc::Reflection::DataType)entry.baseType);
					if((entry.flags & Xsc::Reflection::Uniform::Flags::Color) != 0 &&
					   (type == GPDT_FLOAT3 || type == GPDT_FLOAT4))
					{
						type = GPDT_COLOR;
					}

					u32 arraySize = entry.arraySize;

					if(entry.defaultValue == -1)
						outShaderCreateInformation.AddParameter(ShaderDataParameterInformation(ident, ident, type, StringID::kNone, arraySize));
					else
					{
						const Xsc::Reflection::DefaultValue& defVal = reflectionData.defaultValues[entry.defaultValue];

						outShaderCreateInformation.AddParameter(ShaderDataParameterInformation(ident, ident, type, StringID::kNone, arraySize, 0), (u8*)defVal.matrix);
					}

					if(!entry.spriteUVRef.empty() && (type == GPDT_FLOAT4))
					{
						ShaderParameterAttribute attribute;
						attribute.Value.assign(entry.spriteUVRef.data(), entry.spriteUVRef.size());
						attribute.NextParameterIndex = (u32)-1;
						attribute.Type = ShaderParamAttributeType::SpriteUV;

						outShaderCreateInformation.SetParameterAttribute(ident, attribute);
					}

					parseCommonAttributes();
				}
			}
			break;
		case Xsc::Reflection::VariableType::Struct:
			{
				i32 structIdx = entry.baseType;
				u32 structSize = GetStructSize(structIdx, reflectionData.structs);

				outShaderCreateInformation.AddParameter(ShaderDataParameterInformation(ident, ident, GPDT_STRUCT, StringID::kNone, entry.arraySize, structSize));
			}
			break;
		default:;
		}
	}

	return true;
}

/** Types of supported code output when cross compiling HLSL to GLSL. */
enum class CrossCompileOutput
{
	GLSL45,
	GLSL41,
	VKSL45,
	MVKSL
};

String CrossCompile(const String& hlsl, GpuProgramType type, CrossCompileOutput outputType, bool optionalEntry, u32& startBindingSlot, BSLFXCompileResult& outCompileResult, ShaderCreateInformation* shaderDesc = nullptr, SmallVector<GpuProgramType, 2>* detectedTypes = nullptr)
{
	SPtr<StringStream> input = B3DMakeShared<StringStream>();

	bool isVKSL = outputType == CrossCompileOutput::VKSL45 || outputType == CrossCompileOutput::MVKSL;
	switch(outputType)
	{
	case CrossCompileOutput::GLSL41:
	case CrossCompileOutput::GLSL45:
		*input << "#define OPENGL 1" << std::endl;
		break;
	case CrossCompileOutput::VKSL45:
		*input << "#define VULKAN 1" << std::endl;
		break;
	case CrossCompileOutput::MVKSL:
		*input << "#define METAL 1" << std::endl;
		break;
	}

	// Clear '\r' as it's breaking XShaderCompiler when used in mutiline preprocessor statements
	for (const char& currentCharacter : hlsl)
	{
		if (currentCharacter == '\r')
			continue;

		*input << currentCharacter;
	}

	Xsc::ShaderInput inputDesc;
	inputDesc.shaderVersion = Xsc::InputShaderVersion::HLSL5;
	inputDesc.sourceCode = input;
	inputDesc.extensions = Xsc::Extensions::LayoutAttribute;

	switch(type)
	{
	case GPT_VERTEX_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::VertexShader;
		inputDesc.entryPoint = "vsmain";
		break;
	case GPT_GEOMETRY_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::GeometryShader;
		inputDesc.entryPoint = "gsmain";
		break;
	case GPT_HULL_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::TessellationControlShader;
		inputDesc.entryPoint = "hsmain";
		break;
	case GPT_DOMAIN_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::TessellationEvaluationShader;
		inputDesc.entryPoint = "dsmain";
		break;
	case GPT_FRAGMENT_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::FragmentShader;
		inputDesc.entryPoint = "fsmain";
		break;
	case GPT_COMPUTE_PROGRAM:
		inputDesc.shaderTarget = Xsc::ShaderTarget::ComputeShader;
		inputDesc.entryPoint = "csmain";
		break;
	default:
		break;
	}

	StringStream output;

	Xsc::ShaderOutput outputDesc;
	outputDesc.sourceCode = &output;
	outputDesc.options.autoBinding = isVKSL;
	outputDesc.options.autoBindingStartSlot = startBindingSlot;
	outputDesc.options.fragmentLocations = true;
	outputDesc.options.separateShaders = true;
	outputDesc.options.separateSamplers = isVKSL;
	outputDesc.options.allowExtensions = true;
	outputDesc.nameMangling.inputPrefix = "bs_";
	outputDesc.nameMangling.outputPrefix = "bs_";
	outputDesc.nameMangling.useAlwaysSemantics = true;
	outputDesc.nameMangling.renameBufferFields = true;

	switch(outputType)
	{
	case CrossCompileOutput::GLSL45:
		outputDesc.shaderVersion = Xsc::OutputShaderVersion::GLSL450;
		break;
	case CrossCompileOutput::GLSL41:
		outputDesc.shaderVersion = Xsc::OutputShaderVersion::GLSL410;
		break;
	case CrossCompileOutput::VKSL45:
		outputDesc.shaderVersion = Xsc::OutputShaderVersion::VKSL450;
		break;
	case CrossCompileOutput::MVKSL:
		outputDesc.shaderVersion = Xsc::OutputShaderVersion::VKSL450;
		break;
	}

	XscLog log;
	Xsc::Reflection::ReflectionData reflectionData;
	bool compileSuccess = Xsc::CompileShader(inputDesc, outputDesc, &log, &reflectionData);
	if(!compileSuccess)
	{
		// If enabled, don't fail if entry point isn't found
		bool done = true;
		if(optionalEntry)
		{
			bool entryFound = false;
			for(auto& entry : reflectionData.functions)
			{
				if(entry.ident == inputDesc.entryPoint)
				{
					entryFound = true;
					break;
				}
			}

			if(!entryFound)
				done = false;
		}

		if(done)
		{
			StringStream logOutput;
			log.GetMessages(logOutput);

			outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Log: \n\n{0}", logOutput.str());
			return "";
		}
	}

	for(auto& entry : reflectionData.constantBuffers)
		startBindingSlot = std::max(startBindingSlot, entry.location + 1u);

	for(auto& entry : reflectionData.textures)
		startBindingSlot = std::max(startBindingSlot, entry.location + 1u);

	for(auto& entry : reflectionData.storageBuffers)
		startBindingSlot = std::max(startBindingSlot, entry.location + 1u);

	if(detectedTypes != nullptr)
	{
		for(auto& entry : reflectionData.functions)
		{
			if(entry.ident == "vsmain")
				detectedTypes->Add(GPT_VERTEX_PROGRAM);
			else if(entry.ident == "fsmain")
				detectedTypes->Add(GPT_FRAGMENT_PROGRAM);
			else if(entry.ident == "gsmain")
				detectedTypes->Add(GPT_GEOMETRY_PROGRAM);
			else if(entry.ident == "dsmain")
				detectedTypes->Add(GPT_DOMAIN_PROGRAM);
			else if(entry.ident == "hsmain")
				detectedTypes->Add(GPT_HULL_PROGRAM);
			else if(entry.ident == "csmain")
				detectedTypes->Add(GPT_COMPUTE_PROGRAM);
		}

		// If no entry points found, and error occurred, report error
		if(!compileSuccess && detectedTypes->Empty())
		{
			StringStream logOutput;
			log.GetMessages(logOutput);

			outCompileResult.ErrorMessage = StringUtil::Format("Shader cross compilation failed. Log: \n\n{0}", logOutput.str());
			return "";
		}
	}

	if (shaderDesc != nullptr)
	{
		if (!ParseParameters(reflectionData, outCompileResult, *shaderDesc))
			return "";
	}

	return output.str();
}

String CrossCompile(const String& hlsl, GpuProgramType type, CrossCompileOutput outputType, u32& startBindingSlot, BSLFXCompileResult& outCompileResult)
{
	return CrossCompile(hlsl, type, outputType, false, startBindingSlot, outCompileResult);
}

void ReflectHlsl(const String& hlsl, ShaderCreateInformation& shaderDesc, SmallVector<GpuProgramType, 2>& entryPoints, BSLFXCompileResult& outCompileResult)
{
	u32 dummy = 0;
	CrossCompile(hlsl, GPT_VERTEX_PROGRAM, CrossCompileOutput::GLSL45, true, dummy, outCompileResult, &shaderDesc, &entryPoints);
}

BSLFXCompileResult BSLFXCompiler::Compile(const String& name, const String& source, const UnorderedMap<String, String>& defines, ShadingLanguageFlags languages, SPtr<Shader>& outShader)
{
	SPtr<BSLFXShaderMetaData> shaderMetaData;
	BSLFXCompileResult compileStatus = CompileMetaData(source, defines, shaderMetaData);
	if(!compileStatus.ErrorMessage.empty())
		return compileStatus;

	B3D_ASSERT(shaderMetaData != nullptr);

		BSLFXCompileResult compileResult;

	SmallVector<ShadingLanguageFlag, (u32)ShadingLanguageFlag::Count> requiredLanguageSet;
	for(u32 shadingLanguageIndex = 0; shadingLanguageIndex < (u32)ShadingLanguageFlag::Count; shadingLanguageIndex++)
	{
		if(languages.IsSet((ShadingLanguageFlag)(1 << shadingLanguageIndex)))
			requiredLanguageSet.Add((ShadingLanguageFlag)(1 << shadingLanguageIndex));
	}

	// For every variation, re-parse the file with relevant defines
	for(auto& variation : shaderMetaData->Variations)
	{
		ParsedShaderOrMixinNode parsedNode;
		compileResult = ParseVariation(shaderMetaData->Name, source, variation, defines, parsedNode);

		if(!compileResult.ErrorMessage.empty())
			return compileResult;

		for(u32 languageIndex = 0; languageIndex < requiredLanguageSet.size(); ++languageIndex)
		{
			const auto passCount = (u32)parsedNode.Passes.size();
			for(u32 passIndex = 0; passIndex < passCount; passIndex++)
			{
				const ParsedShaderPassNode& parsedShaderPassNode = parsedNode.Passes[passIndex];

				// Find valid entry points and parameters
				// Note: Ideally we don't need to do a full reflection pass for each GPU program type (i.e. by adding some kind of AST caching to XShaderCompiler)
				ReflectHlsl(parsedShaderPassNode.Code, shaderMetaData->ShaderInformation, shaderMetaData->GPUProgramTypes, compileStatus);
			}

			SPtr<Technique> compiledVariation;
			compileStatus = CompileVariation(shaderMetaData->Name, parsedNode, *shaderMetaData, variation, requiredLanguageSet[languageIndex], compiledVariation);

			if(!compileStatus.ErrorMessage.empty())
				return compileStatus;

			shaderMetaData->ShaderInformation.Techniques.push_back(compiledVariation);
		}
	}

	// Verify techniques compile correctly
	bool hasError = false;
	StringStream gpuProgError;
	for(auto& technique : shaderMetaData->ShaderInformation.Techniques)
	{
		if(!technique->IsSupported())
			continue;

		u32 numPasses = technique->GetNumPasses();
		technique->Compile();

		for(u32 i = 0; i < numPasses; i++)
		{
			SPtr<Pass> pass = technique->GetPass(i);

			auto checkCompileStatus = [&](const String& prefix, const SPtr<GpuProgram>& prog)
			{
				if(prog != nullptr)
				{
					prog->BlockUntilCoreInitialized();

					if(!prog->IsCompiled())
					{
						hasError = true;
						gpuProgError << prefix << ": " << prog->GetCompileErrorMessage() << std::endl;
					}
				}
			};

			const SPtr<GraphicsPipelineState>& graphicsPipeline = pass->GetGraphicsPipelineState();
			if(graphicsPipeline)
			{
				checkCompileStatus("Vertex program", graphicsPipeline->GetVertexProgram());
				checkCompileStatus("Fragment program", graphicsPipeline->GetFragmentProgram());
				checkCompileStatus("Geometry program", graphicsPipeline->GetGeometryProgram());
				checkCompileStatus("Hull program", graphicsPipeline->GetHullProgram());
				checkCompileStatus("Domain program", graphicsPipeline->GetDomainProgram());
			}

			const SPtr<ComputePipelineState>& computePipeline = pass->GetComputePipelineState();
			if(computePipeline)
				checkCompileStatus("Compute program", computePipeline->GetProgram());
		}
	}

	if(hasError)
	{
		compileResult.ErrorMessage = "Failed compiling GPU program(s): " + gpuProgError.str();
		compileResult.ErrorLine = 0;
		compileResult.ErrorColumn = 0;
	}

	if(compileStatus.ErrorMessage.empty())
	{
		outShader = Shader::CreateShared(name, shaderMetaData->ShaderInformation);
		outShader->SetIncludeFiles(shaderMetaData->Includes);
	}

	return compileResult;
}

BSLFXCompileResult BSLFXCompiler::CompileMetaData(const String& source, const UnorderedMap<String, String>& defines, SPtr<BSLFXShaderMetaData>& outShaderMetaData)
{
	SPtr<BSLFXShaderMetaData> shaderMetaData = B3DMakeShared<BSLFXShaderMetaData>();

	ParsedShaderMetaData parsedShaderMetaData;
	BSLFXCompileResult compileResult = ParseMetaData(source, defines, shaderMetaData->ShaderInformation, parsedShaderMetaData, shaderMetaData->Includes);

	if(!compileResult.ErrorMessage.empty())
		return compileResult;

	shaderMetaData->Variations = CreateShaderVariations(parsedShaderMetaData);
	shaderMetaData->Name = parsedShaderMetaData.Name;
	shaderMetaData->Defines = defines;

	outShaderMetaData = shaderMetaData;
	return compileResult;
}

BSLFXCompileResult BSLFXCompiler::CompileVariation(const String& source, const ShaderVariation& variation, ShadingLanguageFlag language, BSLFXShaderMetaData& inOutShaderMetaData, SPtr<Technique>& outVariation)
{
	ParsedShaderOrMixinNode parsedNode;
	BSLFXCompileResult compileResult = ParseVariation(inOutShaderMetaData.Name, source, variation, inOutShaderMetaData.Defines, parsedNode);

	if(!compileResult.ErrorMessage.empty())
		return compileResult;

	if(!inOutShaderMetaData.HasGPUProgramMetaData)
	{
		const auto passCount = (u32)parsedNode.Passes.size();
		for(u32 passIndex = 0; passIndex < passCount; passIndex++)
		{
			const ParsedShaderPassNode& parsedShaderPassNode = parsedNode.Passes[passIndex];

			// Find valid entry points and parameters
			// Note: Ideally we don't need to do a full reflection pass for each GPU program type (i.e. by adding some kind of AST caching to XShaderCompiler)
			ReflectHlsl(parsedShaderPassNode.Code, inOutShaderMetaData.ShaderInformation, inOutShaderMetaData.GPUProgramTypes, compileResult);
		}

		inOutShaderMetaData.HasGPUProgramMetaData = true;
	}

	return CompileVariation(inOutShaderMetaData.Name, parsedNode, inOutShaderMetaData, variation, language, outVariation);
}

BSLFXCompileResult BSLFXCompiler::ParseFx(ParseState* parseState, const char* source, const UnorderedMap<String, String>& defines)
{
	for(auto& define : defines)
	{
		if(define.first.size() == 0)
			continue;

		AddDefine(parseState, define.first.c_str());

		if(define.second.size() > 0)
			AddDefineExpr(parseState, define.second.c_str());
	}

	yyscan_t scanner;
	YY_BUFFER_STATE state;

	BSLFXCompileResult output;

	if(yylex_init_extra(parseState, &scanner))
	{
		output.ErrorMessage = "Internal error: Lexer failed to initialize.";
		return output;
	}

	// If debug output from lexer is needed uncomment this and add %debug option to lexer file
	// yyset_debug(true, scanner);

	// If debug output from parser is needed uncomment this and add %debug option to parser file
	// yydebug = true;

	state = yy_scan_string(source, scanner);

	bool parsingFailed = yyparse(parseState, scanner) > 0;

	if(parseState->HasError > 0)
	{
		output.ErrorMessage = parseState->ErrorMessage;
		output.ErrorLine = parseState->ErrorLine;
		output.ErrorColumn = parseState->ErrorColumn;

		if(parseState->ErrorFile != nullptr)
			output.ErrorFile = parseState->ErrorFile;

		goto cleanup;
	}
	else if(parsingFailed)
	{
		output.ErrorMessage = "Internal error: Parsing failed.";
		goto cleanup;
	}

cleanup:
	yy_delete_buffer(state, scanner);
	yylex_destroy(scanner);

	return output;
}

BSLFXCompiler::ParsedShaderMetaData BSLFXCompiler::ParseShaderMetaData(ASTFXNode* shader)
{
	ParsedShaderMetaData metaData;

	metaData.Language = "hlsl";
	metaData.IsMixin = shader->Type == NT_Mixin;

	for(int i = 0; i < shader->Options->Count; i++)
	{
		NodeOption* option = &shader->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Tags:
			{
				ASTFXNode* tagsNode = option->Value.NodePtr;
				for(int j = 0; j < tagsNode->Options->Count; j++)
				{
					NodeOption* tagOption = &tagsNode->Options->Entries[j];

					if(tagOption->Type == OT_TagValue)
						metaData.Tags.push_back(RemoveQuotes(tagOption->Value.StrValue));
				}
			}
			break;
		case OT_Variations:
			{
				ASTFXNode* variationsNode = option->Value.NodePtr;
				for(int j = 0; j < variationsNode->Options->Count; j++)
				{
					NodeOption* variationOption = &variationsNode->Options->Entries[j];

					if(variationOption->Type == OT_Variation)
						ParseVariations(metaData, variationOption->Value.NodePtr);
				}
			}
			break;
		case OT_Identifier:
			metaData.Name = option->Value.StrValue;
			break;
		case OT_Mixin:
			metaData.Includes.push_back(option->Value.StrValue);
			break;
		default:
			break;
		}
	}

	return metaData;
}

BSLFXCompileResult BSLFXCompiler::ParseMetaDataAndOptions(ASTFXNode* rootNode, Vector<std::pair<ASTFXNode*, ParsedShaderMetaData>>& shaderMetaData, ShaderCreateInformation& shaderCreateInformation)
{
	BSLFXCompileResult output;

	// Only enable for debug purposes
	// SLFXDebugPrint(parseState->rootNode, "");

	if(rootNode == nullptr || rootNode->Type != NT_Root)
	{
		output.ErrorMessage = "Root is null or not a shader.";
		return output;
	}

	// Parse global shader options & shader meta-data
	//// Go in reverse because options are added in reverse order during parsing
	for(int i = rootNode->Options->Count - 1; i >= 0; i--)
	{
		NodeOption* option = &rootNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Options:
			ParseOptions(option->Value.NodePtr, shaderCreateInformation);
			break;
		case OT_Shader:
			{
				// We initially parse only meta-data, so we can handle out-of-order mixin/shader definitions
				ParsedShaderMetaData metaData = ParseShaderMetaData(option->Value.NodePtr);
				shaderMetaData.push_back(std::make_pair(option->Value.NodePtr, metaData));

				break;
			}
		case OT_SubShader:
			// No longer supported
			break;
		default:
			break;
		}
	}

	return output;
}

void BSLFXCompiler::ParseVariations(ParsedShaderMetaData& metaData, ASTFXNode* variations)
{
	B3D_ASSERT(variations->Type == NT_Variation);

	VariationData variationData;
	for(int i = 0; i < variations->Options->Count; i++)
	{
		NodeOption* option = &variations->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Identifier:
			variationData.Identifier = option->Value.StrValue;
			break;
		case OT_VariationOption:
			variationData.Values.push_back(ParseVariationOption(option->Value.NodePtr));
			break;
		case OT_Attributes:
			{
				AttributeData attribs = ParseAttributes(option->Value.NodePtr);

				for(auto& entry : attribs.Attributes)
				{
					if(entry.first == OT_AttrName)
						variationData.Name = entry.second;
					else if(entry.first == OT_AttrShow)
						variationData.Internal = false;
				}
			}
		default:
			break;
		}
	}

	if(!variationData.Identifier.empty())
		metaData.Variations.push_back(variationData);
}

BSLFXCompiler::VariationOption BSLFXCompiler::ParseVariationOption(ASTFXNode* variationOption)
{
	B3D_ASSERT(variationOption->Type == NT_VariationOption);

	VariationOption output;
	for(int i = 0; i < variationOption->Options->Count; i++)
	{
		NodeOption* option = &variationOption->Options->Entries[i];

		switch(option->Type)
		{
		case OT_VariationValue:
			output.Value = option->Value.IntValue;
			break;
		case OT_Attributes:
			{
				AttributeData attribs = ParseAttributes(option->Value.NodePtr);

				for(auto& entry : attribs.Attributes)
				{
					if(entry.first == OT_AttrName)
						output.Name = entry.second;
				}
			}
		default:
			break;
		}
	}

	return output;
}

BSLFXCompiler::AttributeData BSLFXCompiler::ParseAttributes(ASTFXNode* attributes)
{
	B3D_ASSERT(attributes->Type == NT_Attributes);

	AttributeData attributeData;
	for(int i = 0; i < attributes->Options->Count; i++)
	{
		NodeOption* option = &attributes->Options->Entries[i];

		switch(option->Type)
		{
		case OT_AttrName:
			attributeData.Attributes.push_back(std::pair<i32, String>(OT_AttrName, RemoveQuotes(option->Value.StrValue)));
			break;
		case OT_AttrShow:
			attributeData.Attributes.push_back(std::pair<i32, String>(OT_AttrShow, ""));
			break;
		default:
			break;
		}
	}

	return attributeData;
}

QueueSortType BSLFXCompiler::ParseSortType(CullAndSortModeValue sortType)
{
	switch(sortType)
	{
	case CASV_BackToFront:
		return QueueSortType::BackToFront;
	case CASV_FrontToBack:
		return QueueSortType::FrontToBack;
	case CASV_None:
		return QueueSortType::None;
	default:
		break;
	}

	return QueueSortType::None;
}

CompareFunction BSLFXCompiler::ParseCompFunc(CompFuncValue compFunc)
{
	switch(compFunc)
	{
	case CFV_Pass:
		return CMPF_ALWAYS_PASS;
	case CFV_Fail:
		return CMPF_ALWAYS_FAIL;
	case CFV_LT:
		return CMPF_LESS;
	case CFV_LTE:
		return CMPF_LESS_EQUAL;
	case CFV_EQ:
		return CMPF_EQUAL;
	case CFV_NEQ:
		return CMPF_NOT_EQUAL;
	case CFV_GT:
		return CMPF_GREATER;
	case CFV_GTE:
		return CMPF_GREATER_EQUAL;
	}

	return CMPF_ALWAYS_PASS;
}

BlendFactor BSLFXCompiler::ParseBlendFactor(OpValue factor)
{
	switch(factor)
	{
	case OV_One:
		return BF_ONE;
	case OV_Zero:
		return BF_ZERO;
	case OV_DestColor:
		return BF_DEST_COLOR;
	case OV_SrcColor:
		return BF_SOURCE_COLOR;
	case OV_InvDestColor:
		return BF_INV_DEST_COLOR;
	case OV_InvSrcColor:
		return BF_INV_SOURCE_COLOR;
	case OV_DestAlpha:
		return BF_DEST_ALPHA;
	case OV_SrcAlpha:
		return BF_SOURCE_ALPHA;
	case OV_InvDestAlpha:
		return BF_INV_DEST_ALPHA;
	case OV_InvSrcAlpha:
		return BF_INV_SOURCE_ALPHA;
	default:
		break;
	}

	return BF_ONE;
}

BlendOperation BSLFXCompiler::ParseBlendOp(BlendOpValue op)
{
	switch(op)
	{
	case BOV_Add:
		return BO_ADD;
	case BOV_Max:
		return BO_MAX;
	case BOV_Min:
		return BO_MIN;
	case BOV_Subtract:
		return BO_SUBTRACT;
	case BOV_RevSubtract:
		return BO_REVERSE_SUBTRACT;
	}

	return BO_ADD;
}

StencilOperation BSLFXCompiler::ParseStencilOp(OpValue op)
{
	switch(op)
	{
	case OV_Keep:
		return SOP_KEEP;
	case OV_Zero:
		return SOP_ZERO;
	case OV_Replace:
		return SOP_REPLACE;
	case OV_Incr:
		return SOP_INCREMENT;
	case OV_Decr:
		return SOP_DECREMENT;
	case OV_IncrWrap:
		return SOP_INCREMENT_WRAP;
	case OV_DecrWrap:
		return SOP_DECREMENT_WRAP;
	case OV_Invert:
		return SOP_INVERT;
	default:
		break;
	}

	return SOP_KEEP;
}

CullingMode BSLFXCompiler::ParseCullMode(CullAndSortModeValue cm)
{
	switch(cm)
	{
	case CASV_None:
		return CULL_NONE;
	case CASV_CW:
		return CULL_CLOCKWISE;
	case CASV_CCW:
		return CULL_COUNTERCLOCKWISE;
	default:
		break;
	}

	return CULL_COUNTERCLOCKWISE;
}

PolygonMode BSLFXCompiler::ParseFillMode(FillModeValue fm)
{
	if(fm == FMV_Wire)
		return PM_WIREFRAME;

	return PM_SOLID;
}

void BSLFXCompiler::ParseStencilFront(DEPTH_STENCIL_STATE_DESC& desc, ASTFXNode* stencilOpNode)
{
	if(stencilOpNode == nullptr || stencilOpNode->Type != NT_StencilOp)
		return;

	for(int i = 0; i < stencilOpNode->Options->Count; i++)
	{
		NodeOption* option = &stencilOpNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Fail:
			desc.FrontStencilFailOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_ZFail:
			desc.FrontStencilZFailOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_PassOp:
			desc.FrontStencilPassOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_CompareFunc:
			desc.FrontStencilComparisonFunc = ParseCompFunc((CompFuncValue)option->Value.IntValue);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseStencilBack(DEPTH_STENCIL_STATE_DESC& desc, ASTFXNode* stencilOpNode)
{
	if(stencilOpNode == nullptr || stencilOpNode->Type != NT_StencilOp)
		return;

	for(int i = 0; i < stencilOpNode->Options->Count; i++)
	{
		NodeOption* option = &stencilOpNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Fail:
			desc.BackStencilFailOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_ZFail:
			desc.BackStencilZFailOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_PassOp:
			desc.BackStencilPassOp = ParseStencilOp((OpValue)option->Value.IntValue);
			break;
		case OT_CompareFunc:
			desc.BackStencilComparisonFunc = ParseCompFunc((CompFuncValue)option->Value.IntValue);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseColorBlendDef(RENDER_TARGET_BLEND_STATE_DESC& desc, ASTFXNode* blendDefNode)
{
	if(blendDefNode == nullptr || blendDefNode->Type != NT_BlendDef)
		return;

	for(int i = 0; i < blendDefNode->Options->Count; i++)
	{
		NodeOption* option = &blendDefNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Source:
			desc.SrcBlend = ParseBlendFactor((OpValue)option->Value.IntValue);
			break;
		case OT_Dest:
			desc.DstBlend = ParseBlendFactor((OpValue)option->Value.IntValue);
			break;
		case OT_Op:
			desc.BlendOp = ParseBlendOp((BlendOpValue)option->Value.IntValue);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseAlphaBlendDef(RENDER_TARGET_BLEND_STATE_DESC& desc, ASTFXNode* blendDefNode)
{
	if(blendDefNode == nullptr || blendDefNode->Type != NT_BlendDef)
		return;

	for(int i = 0; i < blendDefNode->Options->Count; i++)
	{
		NodeOption* option = &blendDefNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Source:
			desc.SrcBlendAlpha = ParseBlendFactor((OpValue)option->Value.IntValue);
			break;
		case OT_Dest:
			desc.DstBlendAlpha = ParseBlendFactor((OpValue)option->Value.IntValue);
			break;
		case OT_Op:
			desc.BlendOpAlpha = ParseBlendOp((BlendOpValue)option->Value.IntValue);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseRenderTargetBlendState(BLEND_STATE_DESC& desc, ASTFXNode* targetNode, u32& index)
{
	if(targetNode == nullptr || targetNode->Type != NT_Target)
		return;

	for(int i = 0; i < targetNode->Options->Count; i++)
	{
		NodeOption* option = &targetNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Index:
			index = option->Value.IntValue;
			break;
		default:
			break;
		}
	}

	if(index >= B3D_MAXIMUM_RENDER_TARGET_COUNT)
		return;

	RENDER_TARGET_BLEND_STATE_DESC& rtDesc = desc.RenderTargetDesc[index];
	for(int i = 0; i < targetNode->Options->Count; i++)
	{
		NodeOption* option = &targetNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Enabled:
			rtDesc.BlendEnable = option->Value.IntValue > 0;
			break;
		case OT_Color:
			ParseColorBlendDef(rtDesc, option->Value.NodePtr);
			break;
		case OT_Alpha:
			ParseAlphaBlendDef(rtDesc, option->Value.NodePtr);
			break;
		case OT_WriteMask:
			rtDesc.RenderTargetWriteMask = option->Value.IntValue;
			break;
		default:
			break;
		}
	}

	index++;
}

bool BSLFXCompiler::ParseBlendState(ParsedShaderPassNode& desc, ASTFXNode* blendNode)
{
	if(blendNode == nullptr || blendNode->Type != NT_Blend)
		return false;

	bool isDefault = true;
	SmallVector<ASTFXNode*, 8> targets;

	for(int i = 0; i < blendNode->Options->Count; i++)
	{
		NodeOption* option = &blendNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_AlphaToCoverage:
			desc.BlendDesc.AlphaToCoverageEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_IndependantBlend:
			desc.BlendDesc.IndependantBlendEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_Target:
			targets.Add(option->Value.NodePtr);
			isDefault = false;
			break;
		default:
			break;
		}
	}

	// Parse targets in reverse as their order matters and we want to visit them in the top-down order as defined in
	// the source code
	u32 index = 0;
	for(auto iter = targets.rbegin(); iter != targets.rend(); ++iter)
		ParseRenderTargetBlendState(desc.BlendDesc, *iter, index);

	return !isDefault;
}

bool BSLFXCompiler::ParseRasterizerState(ParsedShaderPassNode& desc, ASTFXNode* rasterNode)
{
	if(rasterNode == nullptr || rasterNode->Type != NT_Raster)
		return false;

	bool isDefault = true;

	for(int i = 0; i < rasterNode->Options->Count; i++)
	{
		NodeOption* option = &rasterNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_FillMode:
			desc.RasterizerDesc.PolygonMode = ParseFillMode((FillModeValue)option->Value.IntValue);
			isDefault = false;
			break;
		case OT_CullMode:
			desc.RasterizerDesc.CullMode = ParseCullMode((CullAndSortModeValue)option->Value.IntValue);
			isDefault = false;
			break;
		case OT_DepthBias:
			desc.RasterizerDesc.DepthBias = option->Value.FloatValue;
			isDefault = false;
			break;
		case OT_SDepthBias:
			desc.RasterizerDesc.SlopeScaledDepthBias = option->Value.FloatValue;
			isDefault = false;
			break;
		case OT_DepthClip:
			desc.RasterizerDesc.DepthClipEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_Scissor:
			desc.RasterizerDesc.ScissorEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_Multisample:
			desc.RasterizerDesc.MultisampleEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_AALine:
			desc.RasterizerDesc.AntialiasedLineEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		default:
			break;
		}
	}

	return !isDefault;
}

bool BSLFXCompiler::ParseDepthState(ParsedShaderPassNode& passData, ASTFXNode* depthNode)
{
	if(depthNode == nullptr || depthNode->Type != NT_Depth)
		return false;

	bool isDefault = true;

	for(int i = 0; i < depthNode->Options->Count; i++)
	{
		NodeOption* option = &depthNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_DepthRead:
			passData.DepthStencilDesc.DepthReadEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_DepthWrite:
			passData.DepthStencilDesc.DepthWriteEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_CompareFunc:
			passData.DepthStencilDesc.DepthComparisonFunc = ParseCompFunc((CompFuncValue)option->Value.IntValue);
			isDefault = false;
			break;
		default:
			break;
		}
	}

	return !isDefault;
}

bool BSLFXCompiler::ParseStencilState(ParsedShaderPassNode& passData, ASTFXNode* stencilNode)
{
	if(stencilNode == nullptr || stencilNode->Type != NT_Stencil)
		return false;

	bool isDefault = true;

	for(int i = 0; i < stencilNode->Options->Count; i++)
	{
		NodeOption* option = &stencilNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Enabled:
			passData.DepthStencilDesc.StencilEnable = option->Value.IntValue > 0;
			isDefault = false;
			break;
		case OT_StencilReadMask:
			passData.DepthStencilDesc.StencilReadMask = (u8)option->Value.IntValue;
			isDefault = false;
			break;
		case OT_StencilWriteMask:
			passData.DepthStencilDesc.StencilWriteMask = (u8)option->Value.IntValue;
			isDefault = false;
			break;
		case OT_StencilOpFront:
			ParseStencilFront(passData.DepthStencilDesc, option->Value.NodePtr);
			isDefault = false;
			break;
		case OT_StencilOpBack:
			ParseStencilBack(passData.DepthStencilDesc, option->Value.NodePtr);
			isDefault = false;
			break;
		case OT_StencilRef:
			passData.StencilRefValue = option->Value.IntValue;
			break;
		default:
			break;
		}
	}

	return !isDefault;
}

void BSLFXCompiler::ParseCodeBlock(ASTFXNode* codeNode, const Vector<String>& codeBlocks, ParsedShaderPassNode& passData)
{
	if(codeNode == nullptr || (codeNode->Type != NT_Code))
	{
		return;
	}

	u32 index = (u32)-1;
	for(int j = 0; j < codeNode->Options->Count; j++)
	{
		if(codeNode->Options->Entries[j].Type == OT_Index)
			index = codeNode->Options->Entries[j].Value.IntValue;
	}

	if(index != (u32)-1 && index < (u32)codeBlocks.size())
	{
		passData.Code += codeBlocks[index];
	}
}

void BSLFXCompiler::ParsePass(ASTFXNode* passNode, const Vector<String>& codeBlocks, ParsedShaderPassNode& passData)
{
	if(passNode == nullptr || passNode->Type != NT_Pass)
		return;

	for(int i = 0; i < passNode->Options->Count; i++)
	{
		NodeOption* option = &passNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Blend:
			passData.BlendIsDefault &= !ParseBlendState(passData, option->Value.NodePtr);
			break;
		case OT_Raster:
			passData.RasterizerIsDefault &= !ParseRasterizerState(passData, option->Value.NodePtr);
			break;
		case OT_Depth:
			passData.DepthStencilIsDefault &= !ParseDepthState(passData, option->Value.NodePtr);
			break;
		case OT_Stencil:
			passData.DepthStencilIsDefault &= !ParseStencilState(passData, option->Value.NodePtr);
			break;
		case OT_Code:
			ParseCodeBlock(option->Value.NodePtr, codeBlocks, passData);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseShader(ASTFXNode* shaderNode, const Vector<String>& codeBlocks, ParsedShaderOrMixinNode& shaderData)
{
	if(shaderNode == nullptr || (shaderNode->Type != NT_Shader && shaderNode->Type != NT_Mixin))
		return;

	// There must always be at least one pass
	if(shaderData.Passes.empty())
	{
		shaderData.Passes.push_back(ParsedShaderPassNode());
		shaderData.Passes.back().SeqIdx = 0;
	}

	ParsedShaderPassNode combinedCommonPassData;

	u32 nextPassIdx = 0;
	// Go in reverse because options are added in reverse order during parsing
	for(int i = shaderNode->Options->Count - 1; i >= 0; i--)
	{
		NodeOption* option = &shaderNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Pass:
			{
				u32 passIdx = nextPassIdx;
				ParsedShaderPassNode* passData = nullptr;
				for(auto& entry : shaderData.Passes)
				{
					if(entry.SeqIdx == passIdx)
						passData = &entry;
				}

				if(passData == nullptr)
				{
					shaderData.Passes.push_back(ParsedShaderPassNode());
					passData = &shaderData.Passes.back();

					passData->SeqIdx = passIdx;
				}

				nextPassIdx = std::max(nextPassIdx, passIdx) + 1;
				passData->Code = combinedCommonPassData.Code + passData->Code;

				ASTFXNode* passNode = option->Value.NodePtr;
				ParsePass(passNode, codeBlocks, *passData);
			}
			break;
		case OT_Code:
			{
				ParsedShaderPassNode commonPassData;
				ParseCodeBlock(option->Value.NodePtr, codeBlocks, commonPassData);

				for(auto& passData : shaderData.Passes)
					passData.Code += commonPassData.Code;

				combinedCommonPassData.Code += commonPassData.Code;
			}
			break;
		case OT_FeatureSet:
			shaderData.MetaData.FeatureSet = option->Value.StrValue;
			break;
		default:
			break;
		}
	}

	// Parse common pass states
	for(int i = 0; i < shaderNode->Options->Count; i++)
	{
		NodeOption* option = &shaderNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Blend:
			for(auto& passData : shaderData.Passes)
				passData.BlendIsDefault &= !ParseBlendState(passData, option->Value.NodePtr);
			break;
		case OT_Raster:
			for(auto& passData : shaderData.Passes)
				passData.RasterizerIsDefault &= !ParseRasterizerState(passData, option->Value.NodePtr);
			break;
		case OT_Depth:
			for(auto& passData : shaderData.Passes)
				passData.DepthStencilIsDefault &= !ParseDepthState(passData, option->Value.NodePtr);
			break;
		case OT_Stencil:
			for(auto& passData : shaderData.Passes)
				passData.DepthStencilIsDefault &= !ParseStencilState(passData, option->Value.NodePtr);
			break;
		default:
			break;
		}
	}
}

void BSLFXCompiler::ParseOptions(ASTFXNode* optionsNode, ShaderCreateInformation& shaderDesc)
{
	if(optionsNode == nullptr || optionsNode->Type != NT_Options)
		return;

	for(int i = optionsNode->Options->Count - 1; i >= 0; i--)
	{
		NodeOption* option = &optionsNode->Options->Entries[i];

		switch(option->Type)
		{
		case OT_Separable:
			shaderDesc.SeparablePasses = option->Value.IntValue > 1;
			break;
		case OT_Sort:
			shaderDesc.QueueSortType = ParseSortType((CullAndSortModeValue)option->Value.IntValue);
			break;
		case OT_Priority:
			shaderDesc.QueuePriority = option->Value.IntValue;
			break;
		case OT_Transparent:
			shaderDesc.Flags |= ShaderFlag::Transparent;
			break;
		case OT_Forward:
			shaderDesc.Flags |= ShaderFlag::Forward;
			break;
		default:
			break;
		}
	}
}

BSLFXCompileResult BSLFXCompiler::PopulateVariations(Vector<std::pair<ASTFXNode*, ParsedShaderMetaData>>& shaderMetaData)
{
	BSLFXCompileResult output;

	// Inherit variations from mixins
	bool* mixinWasParsed = B3DStackAllocate<bool>((u32)shaderMetaData.size());

	std::function<bool(const ParsedShaderMetaData&, ParsedShaderMetaData&)> parseInherited =
		[&](const ParsedShaderMetaData& metaData, ParsedShaderMetaData& combinedMetaData)
	{
		for(auto riter = metaData.Includes.rbegin(); riter != metaData.Includes.rend(); ++riter)
		{
			const String& include = *riter;

			u32 baseIdx = -1;
			for(u32 i = 0; i < (u32)shaderMetaData.size(); i++)
			{
				auto& entry = shaderMetaData[i];
				if(!entry.second.IsMixin)
					continue;

				if(entry.second.Name == include)
				{
					bool matches = entry.second.Language == metaData.Language || entry.second.Language == "Any";

					// We want the last matching mixin, in order to allow mixins to override each other
					if(matches)
						baseIdx = i;
				}
			}

			if(baseIdx != (u32)-1)
			{
				auto& entry = shaderMetaData[baseIdx];

				// Was already parsed previously, don't parse it multiple times (happens when multiple mixins
				// include the same mixin)
				if(mixinWasParsed[baseIdx])
					continue;

				if(!parseInherited(entry.second, combinedMetaData))
					return false;

				for(auto& variation : entry.second.Variations)
					combinedMetaData.Variations.push_back(variation);

				mixinWasParsed[baseIdx] = true;
			}
			else
			{
				output.ErrorMessage = "Mixin \"" + include + "\" cannot be found.";
				return false;
			}
		}

		return true;
	};

	for(auto& entry : shaderMetaData)
	{
		const ParsedShaderMetaData& metaData = entry.second;
		if(metaData.IsMixin)
			continue;

		B3DZeroOut(mixinWasParsed, shaderMetaData.size());
		ParsedShaderMetaData combinedMetaData = metaData;
		if(!parseInherited(metaData, combinedMetaData))
		{
			B3DStackFree(mixinWasParsed);
			return output;
		}

		entry.second = combinedMetaData;
	}

	B3DStackFree(mixinWasParsed);

	return output;
}

void BSLFXCompiler::PopulateVariationParamInfos(const ParsedShaderMetaData& shaderMetaData, ShaderCreateInformation& desc)
{
	for(auto& entry : shaderMetaData.Variations)
	{
		ShaderVariationParameterInformation paramInfo;
		paramInfo.IsInternal = entry.Internal;
		paramInfo.Name = entry.Name;
		paramInfo.Identifier = entry.Identifier;

		for(auto& value : entry.Values)
		{
			ShaderVariationParameterValue paramValue;
			paramValue.Name = value.Name;
			paramValue.Value = value.Value;

			paramInfo.Values.Add(paramValue);
		}

		desc.VariationParams.push_back(paramInfo);
	}
}

BSLFXCompileResult BSLFXCompiler::ParseMetaData(const String& source, const UnorderedMap<String, String>& defines, ShaderCreateInformation& outShaderInformation, ParsedShaderMetaData& outShaderMetaData, Vector<String>& outIncludes)
{
	ParseState* parseState = ParseStateCreate();
	BSLFXCompileResult compileResult = ParseFx(parseState, source.c_str(), defines);

	if(!compileResult.ErrorMessage.empty())
	{
		ParseStateDelete(parseState);
		return compileResult;
	}

	// Parse global shader options & shader meta-data
	Vector<pair<ASTFXNode*, ParsedShaderMetaData>> shaderMetaDataWithNodes;
	compileResult = ParseMetaDataAndOptions(parseState->RootNode, shaderMetaDataWithNodes, outShaderInformation);

	if(!compileResult.ErrorMessage.empty())
	{
		ParseStateDelete(parseState);
		return compileResult;
	}

	// Parse includes
	UnorderedSet<String> includeSet;
	IncludeLink* includeLink = parseState->Includes;
	while(includeLink != nullptr)
	{
		const String& includeFilename = includeLink->Data->Filename;
		includeSet.insert(includeFilename);

		includeLink = includeLink->Next;
	}

	for(auto& entry : includeSet)
		outIncludes.push_back(entry);

	ParseStateDelete(parseState);

	compileResult = PopulateVariations(shaderMetaDataWithNodes);

	if(!compileResult.ErrorMessage.empty())
		return compileResult;

	// Note: Must be called after populateVariations, to ensure variations from mixins are inherited
	bool foundShader = false;
	for(auto& entry : shaderMetaDataWithNodes)
	{
		if(entry.second.IsMixin)
			continue;

		if(foundShader)
		{
			compileResult.ErrorMessage = "Shader compilation failed. Multiple shader nodes found in the same file.";
			continue;
		}

		PopulateVariationParamInfos(entry.second, outShaderInformation);
		outShaderMetaData = entry.second;
		foundShader = true;
	}

	return compileResult;
}

BSLFXCompileResult BSLFXCompiler::ParseVariation(const String& name, const String& source, const ShaderVariation& variation, const UnorderedMap<String, String>& defines, ParsedShaderOrMixinNode& outParsedShader)
{
	UnorderedMap<String, String> globalDefines = defines;
	UnorderedMap<String, String> variationDefines = variation.GetDefines().GetAll();

	for(auto& define : variationDefines)
		globalDefines[define.first] = define.second;

	ParseState *const variationParseState = ParseStateCreate();
	BSLFXCompileResult compileResult = ParseFx(variationParseState, source.c_str(), globalDefines);

	if(!compileResult.ErrorMessage.empty())
	{
		ParseStateDelete(variationParseState);
		return compileResult;
	}

	Vector<String> codeBlocks;
	RawCode* codeBlock = variationParseState->RawCodeBlock[RCT_CodeBlock];
	while(codeBlock != nullptr)
	{
		while((i32)codeBlocks.size() <= codeBlock->Index)
			codeBlocks.push_back(String());

		codeBlocks[codeBlock->Index] = String(codeBlock->Code, codeBlock->Size);
		codeBlock = codeBlock->Next;
	}

	if(variationParseState->RootNode == nullptr || variationParseState->RootNode->Type != NT_Root)
	{
		ParseStateDelete(variationParseState);

		compileResult.ErrorMessage = "Unable to parse RSL shader. Root node is null or not a shader.";
		return compileResult;
	}

	Vector<std::pair<ASTFXNode*, ParsedShaderOrMixinNode>> parsedShaders;

	// Go in reverse because options are added in reverse order during parsing
	for(i32 optionIndex = variationParseState->RootNode->Options->Count - 1; optionIndex >= 0; optionIndex--)
	{
		NodeOption* const option = &variationParseState->RootNode->Options->Entries[optionIndex];

		switch(option->Type)
		{
		case OT_Shader:
			{
				// We initially parse only meta-data, so we can handle out-of-order technique definitions
				ParsedShaderMetaData variationMetaData = ParseShaderMetaData(option->Value.NodePtr);

				// Skip all techniques except the one we're parsing
				if(variationMetaData.Name != name && !variationMetaData.IsMixin)
					continue;

				parsedShaders.push_back(std::make_pair(option->Value.NodePtr, ParsedShaderOrMixinNode()));
				ParsedShaderOrMixinNode& parsedShader = parsedShaders.back().second;
				parsedShader.MetaData = variationMetaData;

				break;
			}
		default:
			break;
		}
	}

	bool* parseStatePerMixin = B3DStackAllocate<bool>((u32)parsedShaders.size());
	auto fnEnsureMixinsAreParsed = [parseStatePerMixin, &codeBlocks, &compileResult, &parsedShaders](const ParsedShaderMetaData& metaData, ParsedShaderOrMixinNode& outShader, auto& fnEnsureMixinsAreParsed) -> bool
	{
		for(auto rit = metaData.Includes.rbegin(); rit != metaData.Includes.rend(); ++rit)
		{
			const String& includes = *rit;

			u32 foundMixinIndex = ~0u;
			for(u32 index = 0; index < (u32)parsedShaders.size(); index++)
			{
				auto& entry = parsedShaders[index];
				if(!entry.second.MetaData.IsMixin)
					continue;

				if(entry.second.MetaData.Name == includes)
				{
					const bool matches = (entry.second.MetaData.Language == metaData.Language || entry.second.MetaData.Language == "Any");

					// We want the last matching mixin, in order to allow mixins to override each other
					if(matches)
						foundMixinIndex = index;
				}
			}

			if(foundMixinIndex != ~0u)
			{
				auto& entry = parsedShaders[foundMixinIndex];

				// Was already parsed previously, don't parse it multiple times (happens when multiple mixins include the same mixin)
				if(parseStatePerMixin[foundMixinIndex])
					continue;

				if(!fnEnsureMixinsAreParsed(entry.second.MetaData, outShader, fnEnsureMixinsAreParsed))
					return false;

				ParseShader(entry.first, codeBlocks, outShader);
				parseStatePerMixin[foundMixinIndex] = true;
			}
			else
			{
				compileResult.ErrorMessage = "Mixin \"" + includes + "\" cannot be found.";
				return false;
			}
		}

		return true;
	};

	// Actually parse shaders
	for(auto& parsedShader : parsedShaders)
	{
		const ParsedShaderMetaData& metaData = parsedShader.second.MetaData;
		if(metaData.IsMixin)
			continue;

		B3DZeroOut(parseStatePerMixin, parsedShaders.size());
		if(!fnEnsureMixinsAreParsed(metaData, parsedShader.second, fnEnsureMixinsAreParsed))
		{
			ParseStateDelete(variationParseState);
			B3DStackFree(parseStatePerMixin);
			return compileResult;
		}

		ParseShader(parsedShader.first, codeBlocks, parsedShader.second);
	}

	B3DStackFree(parseStatePerMixin);

	bool foundShader = false;
	for(auto& entry : parsedShaders)
	{
		if(entry.second.MetaData.IsMixin)
			continue;

		if(foundShader)
		{
			compileResult.ErrorMessage = "Shader compilation failed. Multiple shader nodes found in the same file.";
			return compileResult;
		}

		outParsedShader = std::move(entry.second);
		foundShader = true;
	}

	parsedShaders.clear();

	ParseStateDelete(variationParseState);
	return compileResult;
}

BSLFXCompileResult BSLFXCompiler::CompileVariation(const String& name, const ParsedShaderOrMixinNode& parsedShader, const BSLFXShaderMetaData& shaderMetaData, const ShaderVariation& variation, ShadingLanguageFlag language, SPtr<Technique>& outVariation)
{
	B3D_ASSERT(!parsedShader.MetaData.IsMixin);
	B3D_ASSERT(shaderMetaData.GPUProgramTypes.size() > 0);

	BSLFXCompileResult compileResult;

	CrossCompileOutput crossCompileOutputLanguage = CrossCompileOutput::VKSL45;
	String crossCompileOutputLanguageName;
	if(language == ShadingLanguageFlag::GLSL)
	{
		crossCompileOutputLanguage = CrossCompileOutput::GLSL45;
		crossCompileOutputLanguageName = "glsl";
	}
	else if(language == ShadingLanguageFlag::VKSL)
	{
		crossCompileOutputLanguage = CrossCompileOutput::VKSL45;
		crossCompileOutputLanguageName = "vksl";
	}
	else if(language == ShadingLanguageFlag::MSL)
	{
		crossCompileOutputLanguage = CrossCompileOutput::MVKSL;
		crossCompileOutputLanguageName = "mvksl";
	}
	else if(language == ShadingLanguageFlag::HLSL)
	{
		// No cross compile needed
		crossCompileOutputLanguageName = "hlsl";
	}

	struct CrossCompilePassOutput
	{
		String ProgramCodePerType[GPT_COUNT];
	};

	Map<u32, SPtr<Pass>, std::greater<u32>> passes;
	const auto passCount = (u32)parsedShader.Passes.size();
	for(u32 passIndex = 0; passIndex < passCount; passIndex++)
	{
		const ParsedShaderPassNode& parsedShaderPass = parsedShader.Passes[passIndex];

		auto fnCrossCompilePass = [&shaderMetaData, &compileResult](const ParsedShaderPassNode& parsedShaderPass, CrossCompileOutput language, CrossCompilePassOutput& crossCompiledOutput)
		{
			u32 binding = 0;
			for(auto& type : shaderMetaData.GPUProgramTypes)
			{
				B3D_ASSERT((i32)type < GPT_COUNT);
				crossCompiledOutput.ProgramCodePerType[(i32)type] = CrossCompile(parsedShaderPass.Code, type, language, binding, compileResult);

				if(!compileResult.ErrorMessage.empty())
					return;
			}
		};

		CrossCompilePassOutput crossCompilePassOutput;
		if(language == ShadingLanguageFlag::HLSL)
		{
			// Clean non-standard HLSL
			// Note: Ideally we add a full HLSL output module to XShaderCompiler, instead of using simple regex. This
			// way the syntax could be enhanced with more complex features, while still being able to output pure
			// HLSL.
			static const std::regex attrRegex(
				R"(\[\s*layout\s*\(.*\)\s*\]|\[\s*internal\s*\]|\[\s*color\s*\]|\[\s*alias\s*\(.*\)\s*\]|\[\s*spriteuv\s*\(.*\)\s*\])");
			String parsedCode = regex_replace(parsedShaderPass.Code, attrRegex, "");

			static const std::regex attr2Regex(
				R"(\[\s*hideInInspector\s*\]|\[\s*name\s*\(".*"\)\s*\]|\[\s*hdr\s*\])");
			parsedCode = regex_replace(parsedCode, attr2Regex, "");

			static const std::regex initializerRegex(
				R"((Texture2D|Texture3D)\s*(\S*)\s*=.*;)");
			parsedCode = regex_replace(parsedCode, initializerRegex, "$1 $2;");

			static const std::regex warpWithSyncRegex(
				R"(Warp(Group|Device|All)MemoryBarrierWithWarpSync)");
			parsedCode = regex_replace(parsedCode, warpWithSyncRegex, "$1MemoryBarrierWithGroupSync");

			static const std::regex warpNoSyncRegex(
				R"(Warp(Group|Device|All)MemoryBarrier)");
			parsedCode = regex_replace(parsedCode, warpNoSyncRegex, "$1MemoryBarrier");

			// Note: I'm just copying HLSL code as-is. This code will contain all entry points which could have
			// an effect on compile time. It would be ideal to remove dead code depending on program Type. This would
			// involve adding a HLSL code generator to XShaderCompiler.
			for(auto& type : shaderMetaData.GPUProgramTypes)
			{
				B3D_ASSERT((i32)type < GPT_COUNT);
				crossCompilePassOutput.ProgramCodePerType[(i32)type] = parsedCode;
			}
		}
		else // Need to cross compile to correct low-level language
		{
			fnCrossCompilePass(parsedShaderPass, crossCompileOutputLanguage, crossCompilePassOutput);

			if(!compileResult.ErrorMessage.empty())
				return compileResult;
		}

		PASS_DESC shaderPassInformation;
		shaderPassInformation.BlendStateDesc = parsedShaderPass.BlendDesc;
		shaderPassInformation.RasterizerStateDesc = parsedShaderPass.RasterizerDesc;
		shaderPassInformation.DepthStencilStateDesc = parsedShaderPass.DepthStencilDesc;

		auto fnBuildGpuProgramCreateInformation = [&name](const String& language, const String& entry, const String& code, GpuProgramType type) -> GpuProgramCreateInformation
		{
			const char* typeString;
			switch(type)
			{
			case GPT_VERTEX_PROGRAM:
				typeString = "Vertex";
				break;
			case GPT_FRAGMENT_PROGRAM:
				typeString = "Fragment";
				break;
			case GPT_GEOMETRY_PROGRAM:
				typeString = "Geometry";
				break;
			case GPT_DOMAIN_PROGRAM:
				typeString = "Domain";
				break;
			case GPT_HULL_PROGRAM:
				typeString = "Hull";
				break;
			case GPT_COMPUTE_PROGRAM:
				typeString = "Compute";
				break;
			default:
				typeString = "Unknown";
				break;
			}

			GpuProgramCreateInformation gpuProgramCreateInformation;
			gpuProgramCreateInformation.Name = StringUtil::Format("{0} ({1} Program)", name, typeString);
			gpuProgramCreateInformation.Language = language;
			gpuProgramCreateInformation.EntryPoint = entry;
			gpuProgramCreateInformation.Source = code;
			gpuProgramCreateInformation.Type = type;

			return gpuProgramCreateInformation;
		};

		const bool isHLSL = language == ShadingLanguageFlag::HLSL;
		shaderPassInformation.VertexProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "vsmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_VERTEX_PROGRAM],
			GPT_VERTEX_PROGRAM);

		shaderPassInformation.FragmentProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "fsmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_FRAGMENT_PROGRAM],
			GPT_FRAGMENT_PROGRAM);

		shaderPassInformation.GeometryProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "gsmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_GEOMETRY_PROGRAM],
			GPT_GEOMETRY_PROGRAM);

		shaderPassInformation.HullProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "hsmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_HULL_PROGRAM],
			GPT_HULL_PROGRAM);

		shaderPassInformation.DomainProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "dsmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_DOMAIN_PROGRAM],
			GPT_DOMAIN_PROGRAM);

		shaderPassInformation.ComputeProgramDesc = fnBuildGpuProgramCreateInformation(
			crossCompileOutputLanguageName,
			isHLSL ? "csmain" : "main",
			crossCompilePassOutput.ProgramCodePerType[GPT_COMPUTE_PROGRAM],
			GPT_COMPUTE_PROGRAM);

		shaderPassInformation.StencilRefValue = parsedShaderPass.StencilRefValue;

		const SPtr<Pass> pass = Pass::Create(shaderPassInformation);
		if(pass != nullptr)
		{
			passes[parsedShaderPass.SeqIdx] = pass;
		}
	}

	Vector<SPtr<Pass>> orderedPasses;
	for(auto& KVP : passes)
		orderedPasses.push_back(KVP.second);

	if(!orderedPasses.empty())
		outVariation = Technique::Create(crossCompileOutputLanguageName, parsedShader.MetaData.Tags, variation, orderedPasses);

	return compileResult;
}

String BSLFXCompiler::RemoveQuotes(const char* input)
{
	u32 len = (u32)strlen(input);
	String output(len - 2, ' ');

	for(u32 i = 0; i < (len - 2); i++)
		output[i] = input[i + 1];

	return output;
}

Vector<ShaderVariation> BSLFXCompiler::CreateShaderVariations(const ParsedShaderMetaData& shaderMetaData)
{
	if(shaderMetaData.Variations.empty())
		return { ShaderVariation() };

	Vector<ShaderVariation> variations;

	FrameScope frameScope;
	FrameVector<const VariationData*> variationsToProcess;
	for(u32 variationIndex = 0; variationIndex < (u32)shaderMetaData.Variations.size(); variationIndex++)
		variationsToProcess.push_back(&shaderMetaData.Variations[variationIndex]);

	while(!variationsToProcess.empty())
	{
		const VariationData* currentVariation = variationsToProcess.back();
		variationsToProcess.erase(variationsToProcess.end() - 1);

		// Variation parameter that's either defined or isn't
		if(currentVariation->Values.empty())
		{
			// This is the first variation parameter, register new variations
			if(variations.empty())
			{
				ShaderVariation a;
				ShaderVariation b;

				b.AddParam(ShaderVariation::Param(currentVariation->Identifier, 1));

				variations.push_back(a);
				variations.push_back(b);
			}
			else // Duplicate existing variations, and add the parameter
			{
				const u32 variationCount = (u32)variations.size();
				for(u32 variationIndex = 0; variationIndex < variationCount; variationIndex++)
				{
					// Make a copy
					variations.push_back(variations[variationIndex]);

					// Add the parameter to existing variation
					variations[variationIndex].AddParam(ShaderVariation::Param(currentVariation->Identifier, 1));
				}
			}
		}
		else // Variation parameter with multiple values
		{
			// This is the first variation parameter, register new variations
			if(variations.empty())
			{
				for(u32 variationValueIndex = 0; variationValueIndex < (u32)currentVariation->Values.size(); variationValueIndex++)
				{
					ShaderVariation variation;
					variation.AddParam(ShaderVariation::Param(currentVariation->Identifier, currentVariation->Values[variationValueIndex].Value));

					variations.push_back(variation);
				}
			}
			else // Duplicate existing variations, and add the parameter
			{
				const u32 variationCount = (u32)variations.size();
				for(u32 variationIndex = 0; variationIndex < variationCount; variationIndex++)
				{
					for(u32 variationValueIndex = 1; variationValueIndex < (u32)currentVariation->Values.size(); variationValueIndex++)
					{
						ShaderVariation copy = variations[variationIndex];
						copy.AddParam(ShaderVariation::Param(currentVariation->Identifier, currentVariation->Values[variationValueIndex].Value));

						variations.push_back(copy);
					}

					variations[variationIndex].AddParam(ShaderVariation::Param(currentVariation->Identifier, currentVariation->Values[0].Value));
				}
			}
		}
	}

	return variations;
}
