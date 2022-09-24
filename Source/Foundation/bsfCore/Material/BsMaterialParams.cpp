//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Material/BsMaterialParams.h"
#include "Private/RTTI/BsMaterialParamsRTTI.h"
#include "Material/BsShader.h"
#include "Image/BsTexture.h"
#include "Image/BsSpriteTexture.h"
#include "RenderAPI/BsGpuBuffer.h"
#include "RenderAPI/BsSamplerState.h"
#include "Image/BsColorGradient.h"
#include "Animation/BsAnimationCurve.h"
#include "Allocators/BsPoolAlloc.h"

namespace bs
{
	namespace impl
	{
		SPtr<ct::Texture> getSpriteTextureAtlas(const SPtr<ct::SpriteTexture>& spriteTexture)
		{
			if(spriteTexture)
				return spriteTexture->GetTexture();

			return nullptr;
		}

		HTexture getSpriteTextureAtlas(const HSpriteTexture& spriteTexture)
		{
			if(spriteTexture.IsLoaded())
				return spriteTexture->GetTexture();

			return HTexture();
		}
	}

	MaterialParamsBase::MaterialParamsBase(
		const Map<String, SHADER_DATA_PARAM_DESC>& dataParams,
		const Map<String, SHADER_OBJECT_PARAM_DESC>& textureParams,
		const Map<String, SHADER_OBJECT_PARAM_DESC>& bufferParams,
		const Map<String, SHADER_OBJECT_PARAM_DESC>& samplerParams,
		UINT64 initialParamVersion
	)
		: mParamVersion(initialParamVersion)
	{
		mDataSize = 0;

		for (auto& param : dataParams)
		{
			if(param.second.Type == GPDT_UNKNOWN)
				continue;

			UINT32 arraySize = param.second.ArraySize > 1 ? param.second.ArraySize : 1;
			if(param.second.Type == GPDT_STRUCT)
				mNumStructParams += arraySize;
			else
			{
				const GpuParamDataTypeInfo& typeInfo = GpuParams::PARAM_SIZES.Lookup[(int)param.second.Type];
				UINT32 paramSize = typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize;

				mDataSize += arraySize * paramSize;
				mNumDataParams += arraySize;
			}
		}

		mNumTextureParams = (UINT32)textureParams.size();
		mNumBufferParams = (UINT32)bufferParams.size();
		mNumSamplerParams = (UINT32)samplerParams.size();

		mDataParamsBuffer = mAlloc.Alloc(mDataSize);
		memset(mDataParamsBuffer, 0, mDataSize);

		mDataParams = (DataParamInfo*)mAlloc.Alloc(mNumDataParams * sizeof(DataParamInfo));
		memset(mDataParams, 0, mNumDataParams * sizeof(DataParamInfo));

		UINT32 dataParamIdx = 0;
		UINT32 dataBufferIdx = 0;
		UINT32 structParamIdx = 0;

		for (auto& entry : dataParams)
		{
			if(entry.second.Type == GPDT_UNKNOWN)
				continue;

			const auto paramIdx = (UINT32)mParams.size();
			mParams.push_back(ParamData());
			mParamLookup[entry.first] = paramIdx;

			ParamData& dataParam = mParams.back();

			const UINT32 arraySize = entry.second.ArraySize > 1 ? entry.second.ArraySize : 1;
			dataParam.ArraySize = arraySize;
			dataParam.Type = ParamType::Data;
			dataParam.DataType = entry.second.Type;
			dataParam.Version = 1;
			
			if(entry.second.Type == GPDT_STRUCT)
			{
				dataParam.Index = structParamIdx;
				structParamIdx += arraySize;
			}
			else
			{
				dataParam.Index = dataParamIdx;

				const GpuParamDataTypeInfo& typeInfo = GpuParams::PARAM_SIZES.Lookup[(int)dataParam.DataType];
				const UINT32 paramSize = typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize;
				for (UINT32 i = 0; i < arraySize; i++)
				{
					mDataParams[dataParamIdx].Offset = dataBufferIdx;
					mDataParams[dataParamIdx].SpriteTextureIdx = (UINT32)-1;

					dataBufferIdx += paramSize;
					dataParamIdx++;
				}
			}
		}

		UINT32 textureIdx = 0;
		for (auto& entry : textureParams)
		{
			UINT32 paramIdx = (UINT32)mParams.size();
			mParams.push_back(ParamData());
			mParamLookup[entry.first] = paramIdx;

			ParamData& dataParam = mParams.back();

			dataParam.ArraySize = 1;
			dataParam.Type = ParamType::Texture;
			dataParam.DataType = GPDT_UNKNOWN;
			dataParam.Index = textureIdx;
			dataParam.Version = 1;

			textureIdx++;
		}

		UINT32 bufferIdx = 0;
		for (auto& entry : bufferParams)
		{
			UINT32 paramIdx = (UINT32)mParams.size();
			mParams.push_back(ParamData());
			mParamLookup[entry.first] = paramIdx;

			ParamData& dataParam = mParams.back();

			dataParam.ArraySize = 1;
			dataParam.Type = ParamType::Buffer;
			dataParam.DataType = GPDT_UNKNOWN;
			dataParam.Index = bufferIdx;
			dataParam.Version = 1;

			bufferIdx++;
		}

		UINT32 samplerIdx = 0;
		for (auto& entry : samplerParams)
		{
			UINT32 paramIdx = (UINT32)mParams.size();
			mParams.push_back(ParamData());
			mParamLookup[entry.first] = paramIdx;

			ParamData& dataParam = mParams.back();

			dataParam.ArraySize = 1;
			dataParam.Type = ParamType::Sampler;
			dataParam.DataType = GPDT_UNKNOWN;
			dataParam.Index = samplerIdx;
			dataParam.Version = 1;

			samplerIdx++;
		}
	}

	MaterialParamsBase::~MaterialParamsBase()
	{
		for(UINT32 i = 0; i < mNumDataParams; i++)
		{
			DataParamInfo& paramInfo = mDataParams[i];

			if (paramInfo.FloatCurve)
			{
				bs_pool_free(paramInfo.FloatCurve);
				paramInfo.FloatCurve = nullptr;
			}

			if (paramInfo.ColorGradient)
			{
				bs_pool_free(paramInfo.ColorGradient);
				paramInfo.ColorGradient = nullptr;
			}
		}

		mAlloc.Free(mDataParamsBuffer);
		mAlloc.Free(mDataParams);
		
		mAlloc.Clear();
	}

	const ColorGradientHDR& MaterialParamsBase::GetColorGradientParam(const String& name, UINT32 arrayIdx) const
	{
		static ColorGradientHDR EMPTY_GRADIENT;

		const ParamData* param = nullptr;
		auto result = GetParamData(name, ParamType::Data, GPDT_COLOR, arrayIdx, &param);
		if (result != GetParamResult::Success)
			return EMPTY_GRADIENT;

		return GetColorGradientParam(*param, arrayIdx);
	}

	void MaterialParamsBase::SetColorGradientParam(const String& name, UINT32 arrayIdx, const ColorGradientHDR& input) const
	{
		const ParamData* param = nullptr;
		auto result = GetParamData(name, ParamType::Data, GPDT_COLOR, arrayIdx, &param);
		if (result != GetParamResult::Success)
			return;

		SetColorGradientParam(*param, arrayIdx, input);
	}

	const ColorGradientHDR& MaterialParamsBase::GetColorGradientParam(const ParamData& param, UINT32 arrayIdx) const
	{
		const DataParamInfo& paramInfo = mDataParams[param.Index + arrayIdx];
		if (paramInfo.ColorGradient)
			return *paramInfo.ColorGradient;

		static ColorGradientHDR EMPTY_GRADIENT;
		return EMPTY_GRADIENT;
	}

	void MaterialParamsBase::SetColorGradientParam(const ParamData& param, UINT32 arrayIdx, const ColorGradientHDR& input) const
	{
		DataParamInfo& paramInfo = mDataParams[param.Index + arrayIdx];
		if (paramInfo.ColorGradient)
			bs_pool_free(paramInfo.ColorGradient);

		paramInfo.ColorGradient = bs_pool_new<ColorGradientHDR>(input);

		param.Version = ++mParamVersion;
	}

	UINT32 MaterialParamsBase::GetParamIndex(const String& name) const
	{
		auto iterFind = mParamLookup.find(name);
		if (iterFind == mParamLookup.end())
			return (UINT32)-1;

		return iterFind->second;
	}

	MaterialParamsBase::GetParamResult MaterialParamsBase::GetParamIndex(const String& name, ParamType type,
		GpuParamDataType dataType, UINT32 arrayIdx, UINT32& output) const
	{
		auto iterFind = mParamLookup.find(name);
		if (iterFind == mParamLookup.end())
			return GetParamResult::NotFound;

		UINT32 index = iterFind->second;
		const ParamData& param = mParams[index];
		
		if (param.Type != type || (type == ParamType::Data && param.DataType != dataType))
			return GetParamResult::InvalidType;

		if (arrayIdx >= param.ArraySize)
			return GetParamResult::IndexOutOfBounds;

		output = index;
		return GetParamResult::Success;
	}

	MaterialParamsBase::GetParamResult MaterialParamsBase::GetParamData(const String& name, ParamType type,
		GpuParamDataType dataType, UINT32 arrayIdx, const ParamData** output) const
	{
		auto iterFind = mParamLookup.find(name);
		if (iterFind == mParamLookup.end())
			return GetParamResult::NotFound;

		UINT32 index = iterFind->second;
		const ParamData& param = mParams[index];
		*output = &param;

		if (param.Type != type || (type == ParamType::Data && param.DataType != dataType))
			return GetParamResult::InvalidType;

		if (arrayIdx >= param.ArraySize)
			return GetParamResult::IndexOutOfBounds;

		return GetParamResult::Success;
	}

	void MaterialParamsBase::ReportGetParamError(GetParamResult errorCode, const String& name, UINT32 arrayIdx) const
	{
		switch (errorCode)
		{
		case GetParamResult::NotFound:
			BS_LOG(Warning, Material, "Material doesn't have a parameter named {0}.", name);
			break;
		case GetParamResult::InvalidType:
			BS_LOG(Warning, Material, "Parameter \"{0}\" is not of the requested type.", name);
			break;
		case GetParamResult::IndexOutOfBounds:
			BS_LOG(Warning, Material, "Parameter \"{0}\" array index {1} out of range.", name, arrayIdx);
			break;
		default:
			break;
		}
	}

	RTTITypeBase* MaterialParamStructData::GetRttiStatic()
	{
		return MaterialParamStructDataRTTI::Instance();
	}

	RTTITypeBase* MaterialParamStructData::GetRtti() const
	{
		return GetRttiStatic();
	}

	RTTITypeBase* MaterialParamTextureData::GetRttiStatic()
	{
		return MaterialParamTextureDataRTTI::Instance();
	}

	RTTITypeBase* MaterialParamTextureData::GetRtti() const
	{
		return GetRttiStatic();
	}

	template<bool Core>
	TMaterialParams<Core>::TMaterialParams(const ShaderType& shader, UINT64 initialParamVersion)
		:MaterialParamsBase(
			shader->GetDataParams(),
			shader->GetTextureParams(),
			shader->GetBufferParams(),
			shader->GetSamplerParams(),
			initialParamVersion
		)
	{
		mStructParams = mAlloc.Construct<ParamStructDataType>(mNumStructParams);
		mTextureParams = mAlloc.Construct<ParamTextureDataType>(mNumTextureParams);
		mBufferParams = mAlloc.Construct<ParamBufferDataType>(mNumBufferParams);
		mSamplerStateParams = mAlloc.Construct<ParamSamplerStateDataType>(mNumSamplerParams);
		mDefaultTextureParams = mAlloc.Construct<TextureType>(mNumTextureParams);
		mDefaultSamplerStateParams = mAlloc.Construct<SamplerType>(mNumSamplerParams);

		auto& textureParams = shader->GetTextureParams();
		UINT32 textureIdx = 0;
		for (auto& entry : textureParams)
		{
			ParamTextureDataType& param = mTextureParams[textureIdx];
			param.IsLoadStore = false;

			if (entry.second.DefaultValueIdx != (UINT32)-1)
				mDefaultTextureParams[textureIdx] = shader->GetDefaultTexture(entry.second.DefaultValueIdx);

			textureIdx++;
		}

		auto& samplerParams = shader->GetSamplerParams();
		UINT32 samplerIdx = 0;
		for (auto& entry : samplerParams)
		{
			if (entry.second.DefaultValueIdx != (UINT32)-1)
				mDefaultSamplerStateParams[samplerIdx] = shader->GetDefaultSampler(entry.second.DefaultValueIdx);

			samplerIdx++;
		}

		// Note: Make sure to process data parameters after textures, in order to handle SpriteUV data parameters
		auto& dataParams = shader->GetDataParams();
		auto& paramAttributes = shader->GetParamAttributes();
		UINT32 structIdx = 0;
		for (auto& entry : dataParams)
		{
			if(entry.second.Type == GPDT_STRUCT)
			{
				UINT32 arraySize = entry.second.ArraySize > 1 ? entry.second.ArraySize : 1;
				for (UINT32 i = 0; i < arraySize; i++)
				{
					ParamStructDataType& param = mStructParams[structIdx];
					param.DataSize = entry.second.ElementSize;
					param.Data = mAlloc.Alloc(param.DataSize);

					structIdx++;
				}
			}
			else
			{
				// Check for SpriteUV attribute
				UINT32 attribIdx = entry.second.AttribIdx;
				while (attribIdx != (UINT32)-1)
				{
					const SHADER_PARAM_ATTRIBUTE& attrib = paramAttributes[attribIdx];
					if (attrib.Type == ShaderParamAttributeType::SpriteUV)
					{
						// Find referenced texture
						const auto findIterTex = mParamLookup.find(attrib.Value);
						const auto findIterParam = mParamLookup.find(entry.first);
						if (findIterTex != mParamLookup.end() && findIterParam != mParamLookup.end())
						{
							ParamData& paramData = mParams[findIterParam->second];

							DataParamInfo& dataParamInfo = mDataParams[paramData.Index];
							dataParamInfo.SpriteTextureIdx = findIterTex->second;
						}
					}

					attribIdx = attrib.NextParamIdx;
				}
			}
		}
	}

	template<bool Core>
	TMaterialParams<Core>::~TMaterialParams()
	{
		if (mStructParams != nullptr)
		{
			for (UINT32 i = 0; i < mNumStructParams; i++)
				mAlloc.Free(mStructParams[i].Data);
		}

		mAlloc.Destruct(mStructParams, mNumStructParams);
		mAlloc.Destruct(mTextureParams, mNumTextureParams);
		mAlloc.Destruct(mBufferParams, mNumBufferParams);
		mAlloc.Destruct(mSamplerStateParams, mNumSamplerParams);

		if(mDefaultTextureParams != nullptr)
			mAlloc.Destruct(mDefaultTextureParams, mNumTextureParams);

		if (mDefaultSamplerStateParams != nullptr)
			mAlloc.Destruct(mDefaultSamplerStateParams, mNumSamplerParams);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetStructData(const String& name, void* value, UINT32 size, UINT32 arrayIdx) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Data, GPDT_STRUCT, arrayIdx, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, arrayIdx);
			return;
		}

		GetStructData(*param,  value, size, arrayIdx);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetStructData(const String& name, const void* value, UINT32 size, UINT32 arrayIdx)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Data, GPDT_STRUCT, arrayIdx, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, arrayIdx);
			return;
		}

		SetStructData(*param, value, size, arrayIdx);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetTexture(const String& name, TextureType& value, TextureSurface& surface) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		GetTexture(*param, value, surface);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetTexture(const String& name, const TextureType& value, const TextureSurface& surface)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		SetTexture(*param, value, surface);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetSpriteTexture(const String& name, SpriteTextureType& value) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		GetSpriteTexture(*param, value);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetSpriteTexture(const String& name, const SpriteTextureType& value)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		SetSpriteTexture(*param, value);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetLoadStoreTexture(const String& name, TextureType& value, TextureSurface& surface) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		GetLoadStoreTexture(*param, value, surface);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetLoadStoreTexture(const String& name, const TextureType& value, const TextureSurface& surface)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Texture, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		SetLoadStoreTexture(*param, value, surface);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetBuffer(const String& name, BufferType& value) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Buffer, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		GetBuffer(*param, value);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetBuffer(const String& name, const BufferType& value)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Buffer, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		SetBuffer(*param, value);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetSamplerState(const String& name, SamplerType& value) const
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Sampler, GPDT_UNKNOWN, 0, &param);
		if (result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		GetSamplerState(*param, value);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetSamplerState(const String& name, const SamplerType& value)
	{
		const ParamData* param = nullptr;
		GetParamResult result = GetParamData(name, ParamType::Sampler, GPDT_UNKNOWN, 0, &param);
		if(result != GetParamResult::Success)
		{
			ReportGetParamError(result, name, 0);
			return;
		}

		SetSamplerState(*param, value);
	}

	template<bool Core>
	bool TMaterialParams<Core>::IsAnimated(const String& name, UINT32 arrayIdx)
	{
		auto iterFind = mParamLookup.find(name);
		if (iterFind == mParamLookup.end())
			return false;

		UINT32 index = iterFind->second;
		const ParamData& param = mParams[index];

		if (param.Type != ParamType::Data)
			return false;

		if (arrayIdx >= param.ArraySize)
			return false;

		return IsAnimated(param, arrayIdx);
	}

	template<bool Core>
	void TMaterialParams<Core>::GetStructData(const ParamData& param, void* value, UINT32 size, UINT32 arrayIdx) const
	{
		const ParamStructDataType& structParam = mStructParams[param.Index + arrayIdx];
		if (structParam.DataSize != size)
		{
			BS_LOG(Warning, Material, "Size mismatch when writing to a struct. Provided size was {0} bytes but the struct "
				"size is {1} bytes", size, structParam.DataSize);
			return;
		}

		memcpy(value, structParam.Data, structParam.DataSize);
	}

	template<bool Core>
	void TMaterialParams<Core>::SetStructData(const ParamData& param, const void* value, UINT32 size, UINT32 arrayIdx)
	{
		const ParamStructDataType& structParam = mStructParams[param.Index + arrayIdx];
		if (structParam.DataSize != size)
		{
			BS_LOG(Warning, Material, "Size mismatch when writing to a struct. Provided size was {0} bytes but the struct "
				"size is {1} bytes", size, structParam.DataSize);
			return;
		}

		memcpy(structParam.Data, value, structParam.DataSize);
		param.Version = ++mParamVersion;
	}

	template<bool Core>
	UINT32 TMaterialParams<Core>::GetStructSize(const ParamData& param) const
	{
		const ParamStructDataType& structParam = mStructParams[param.Index];
		return structParam.DataSize;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetTexture(const ParamData& param, TextureType& value, TextureSurface& surface) const
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];

		if(textureParam.Texture)
			value = textureParam.Texture;
		else if(textureParam.SpriteTexture)
			value = impl::getSpriteTextureAtlas(textureParam.SpriteTexture);

		surface = textureParam.Surface;
	}
	
	template<bool Core>
	void TMaterialParams<Core>::SetTexture(const ParamData& param, const TextureType& value, const TextureSurface& surface)
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];
		textureParam.Texture = value;
		textureParam.SpriteTexture = nullptr;
		textureParam.IsLoadStore = false;
		textureParam.Surface = surface;

		param.Version = ++mParamVersion;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetSpriteTexture(const ParamData& param, SpriteTextureType& value) const
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];
		value = textureParam.SpriteTexture;
	}
	
	template<bool Core>
	void TMaterialParams<Core>::SetSpriteTexture(const ParamData& param, const SpriteTextureType& value)
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];
		textureParam.Texture = nullptr;
		textureParam.SpriteTexture = value;
		textureParam.IsLoadStore = false;
		textureParam.Surface = TextureSurface::COMPLETE;

		param.Version = ++mParamVersion;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetBuffer(const ParamData& param, BufferType& value) const
	{
		value = mBufferParams[param.Index].Value;
	}

	template<bool Core>
	void TMaterialParams<Core>::SetBuffer(const ParamData& param, const BufferType& value)
	{
		mBufferParams[param.Index].Value = value;

		param.Version = ++mParamVersion;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetLoadStoreTexture(const ParamData& param, TextureType& value, TextureSurface& surface) const
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];
		value = textureParam.Texture;
		surface = textureParam.Surface;
	}

	template<bool Core>
	void TMaterialParams<Core>::SetLoadStoreTexture(const ParamData& param, const TextureType& value, const TextureSurface& surface)
	{
		ParamTextureDataType& textureParam = mTextureParams[param.Index];
		textureParam.Texture = value;
		textureParam.SpriteTexture = nullptr;
		textureParam.IsLoadStore = true;
		textureParam.Surface = surface;

		param.Version = ++mParamVersion;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetSamplerState(const ParamData& param, SamplerType& value) const
	{
		value = mSamplerStateParams[param.Index].Value;
	}

	template<bool Core>
	void TMaterialParams<Core>::SetSamplerState(const ParamData& param, const SamplerType& value)
	{
		mSamplerStateParams[param.Index].Value = value;

		param.Version = ++mParamVersion;
	}

	template<bool Core>
	MateralParamTextureType TMaterialParams<Core>::GetTextureType(const ParamData& param) const
	{
		if(mTextureParams[param.Index].IsLoadStore)
			return MateralParamTextureType::LoadStore;

		if(mTextureParams[param.Index].SpriteTexture)
			return MateralParamTextureType::Sprite;

		return MateralParamTextureType::Normal;
	}

	template<bool Core>
	bool TMaterialParams<Core>::IsAnimated(const ParamData& param, UINT32 arrayIdx) const
	{
		const DataParamInfo& paramInfo = mDataParams[param.Index + arrayIdx];

		return paramInfo.FloatCurve || paramInfo.ColorGradient || paramInfo.SpriteTextureIdx != (UINT32)-1;
	}

	template<bool Core>
	typename TMaterialParams<Core>::SpriteTextureType TMaterialParams<Core>::GetOwningSpriteTexture(const ParamData& param) const
	{
		SpriteTextureType output;

		const DataParamInfo& paramInfo = mDataParams[param.Index];
		if (paramInfo.SpriteTextureIdx == (UINT32)-1)
			return output;

		const ParamData* spriteTexParamData = GetParamData(paramInfo.SpriteTextureIdx);
		if(spriteTexParamData)
			GetSpriteTexture(*spriteTexParamData, output);

		return output;
	}

	template<bool Core>
	void TMaterialParams<Core>::GetDefaultTexture(const ParamData& param, TextureType& value) const
	{
		value = mDefaultTextureParams[param.Index];
	}

	template<bool Core>
	void TMaterialParams<Core>::GetDefaultSamplerState(const ParamData& param, SamplerType& value) const
	{
		value = mDefaultSamplerStateParams[param.Index];
	}

	template class TMaterialParams<true>;
	template class TMaterialParams<false>;

	MaterialParams::MaterialParams(const HShader& shader, UINT64 initialParamVersion)
		:TMaterialParams(shader, initialParamVersion), mLastSyncVersion(1)
	{ }

	void MaterialParams::GetSyncData(UINT8* buffer, UINT32& size, bool forceAll)
	{
		// Note: Not syncing struct data

		UINT32 numDirtyDataParams = 0;
		UINT32 numDirtyStructParams = 0;
		UINT32 numDirtyTextureParams = 0;
		UINT32 numDirtyBufferParams = 0;
		UINT32 numDirtySamplerParams = 0;

		UINT32 dataParamSize = 0;
		UINT32 structParamSize = 0;
		for(auto& param : mParams)
		{
			if (param.Version <= mLastSyncVersion && !forceAll)
				continue;

			switch(param.Type)
			{
			case ParamType::Data:
			{
				const UINT32 arraySize = param.ArraySize > 1 ? param.ArraySize : 1;

				if(param.DataType == GPDT_STRUCT)
				{
					// Param index
					structParamSize += sizeof(UINT32);

					// Param data
					structParamSize += arraySize * mStructParams[param.Index].DataSize;

					numDirtyStructParams++;
				}
				else
				{
					const GpuParamDataTypeInfo& typeInfo = GpuParams::PARAM_SIZES.Lookup[(int)param.DataType];
					const UINT32 paramSize = typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize;

					// Param index
					dataParamSize += sizeof(UINT32);

					// Param data
					dataParamSize += arraySize * paramSize;

					// Param curves
					dataParamSize += sizeof(UINT32);
					for (UINT32 i = 0; i < arraySize; i++)
					{
						const DataParamInfo& paramInfo = mDataParams[param.Index + i];
						if (paramInfo.FloatCurve && param.DataType == GPDT_FLOAT1)
						{
							// Array index
							dataParamSize += sizeof(UINT32);

							// Curve data
							dataParamSize += rtti_size(*paramInfo.FloatCurve).Bytes;
						}
						else if (paramInfo.ColorGradient && param.DataType == GPDT_COLOR)
						{
							// Array index
							dataParamSize += sizeof(UINT32);

							// Curve data
							dataParamSize += rtti_size(*paramInfo.ColorGradient).Bytes;
						}
					}

					numDirtyDataParams++;
				}
			}
				break;
			case ParamType::Texture:
				numDirtyTextureParams++;
				break;
			case ParamType::Buffer:
				numDirtyBufferParams++;
				break;
			case ParamType::Sampler:
				numDirtySamplerParams++;
				break;
			}
		}

		const UINT64 textureEntrySize = sizeof(MaterialParamTextureDataCore) + sizeof(UINT32);
		const UINT64 bufferEntrySize = sizeof(MaterialParamBufferDataCore) + sizeof(UINT32);
		const UINT64 samplerStateEntrySize = sizeof(MaterialParamSamplerStateDataCore) + sizeof(UINT32);

		const UINT64 dataParamsOffset = sizeof(UINT32) * 5;
		const UINT64 textureParamsOffset = dataParamsOffset + dataParamSize;
		const UINT64 bufferParamsOffset = textureParamsOffset + textureEntrySize * numDirtyTextureParams;
		const UINT64 samplerStateParamsOffset = bufferParamsOffset + bufferEntrySize * numDirtyBufferParams;
		const UINT64 structParamsOffset = samplerStateParamsOffset + samplerStateEntrySize * numDirtySamplerParams;

		const UINT32 totalSize = (UINT32)structParamsOffset + structParamSize;

		if (buffer == nullptr)
		{
			size = totalSize;
			return;
		}

		if(size != totalSize)
		{
			BS_LOG(Error, Material, "Invalid buffer size provided, ignoring.");
			return;
		}

		Bitstream stream((uint8_t*)buffer, size);

		// Dirty counts for each parameter type
		rtti_write(numDirtyDataParams, stream);
		rtti_write(numDirtyTextureParams, stream);
		rtti_write(numDirtyBufferParams, stream);
		rtti_write(numDirtySamplerParams, stream);
		rtti_write(numDirtyStructParams, stream);

		UINT64 dirtyDataParamOffset = 0;
		UINT64 dirtyTextureParamIdx = 0;
		UINT64 dirtyBufferParamIdx = 0;
		UINT64 dirtySamplerParamIdx = 0;
		UINT64 dirtyStructParamOffset = 0;

		for(UINT32 i = 0; i < (UINT32)mParams.size(); i++)
		{
			ParamData& param = mParams[i];
			if (param.Version <= mLastSyncVersion && !forceAll)
				continue;

			switch (param.Type)
			{
			case ParamType::Data:
			{
				const UINT32 arraySize = param.ArraySize > 1 ? param.ArraySize : 1;

				if (param.DataType == GPDT_STRUCT)
				{
					const ParamStructDataType& paramData = mStructParams[param.Index];

					// Param index
					stream.Seek((structParamsOffset + dirtyStructParamOffset) * 8);
					dirtyStructParamOffset += rtti_write(i, stream).Bytes;

					// Param data
					for (UINT32 j = 0; j < arraySize; j++)
					{
						stream.WriteBytes(mStructParams[param.Index + j].Data, paramData.DataSize);
						dirtyStructParamOffset += paramData.DataSize;
					}
				}
				else
				{
					const GpuParamDataTypeInfo& typeInfo = GpuParams::PARAM_SIZES.Lookup[(int)param.DataType];
					const UINT32 paramSize = typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize;

					const UINT32 dataSize = arraySize * paramSize;
					const DataParamInfo& paramInfo = mDataParams[param.Index];

					// Param index
					stream.Seek((dataParamsOffset + dirtyDataParamOffset) * 8);
					dirtyDataParamOffset += rtti_write(i, stream).Bytes;

					// Param data
					// Note: This relies on the fact that all data params in the array are sequential
					stream.WriteBytes((uint8_t*)&mDataParamsBuffer[paramInfo.Offset], dataSize);
					dirtyDataParamOffset += dataSize;

					// Param curves
					UINT64 numDirtyCurvesWriteDst = stream.Tell();
					stream.WriteBytes(0);
					dirtyDataParamOffset += sizeof(UINT32);

					UINT32 numDirtyCurves = 0;
					for (UINT32 j = 0; j < arraySize; j++)
					{
						const DataParamInfo& arrParamInfo = mDataParams[param.Index + j];
						if (arrParamInfo.FloatCurve && param.DataType == GPDT_FLOAT1)
						{
							// Array index
							dirtyDataParamOffset += rtti_write(j, stream).Bytes;

							// Curve data
							dirtyDataParamOffset += rtti_write(*arrParamInfo.FloatCurve, stream).Bytes;

							numDirtyCurves++;
						}
						else if (arrParamInfo.ColorGradient && param.DataType == GPDT_COLOR)
						{
							// Array index
							dirtyDataParamOffset += rtti_write(j, stream).Bytes;

							// Curve data
							dirtyDataParamOffset += rtti_write(*arrParamInfo.ColorGradient, stream).Bytes;

							numDirtyCurves++;
						}
					}

					stream.Seek(numDirtyCurvesWriteDst);
					stream.WriteBytes(numDirtyCurves);
				}
			}
			break;
			case ParamType::Texture:
			{
				stream.Seek((textureParamsOffset + dirtyTextureParamIdx * textureEntrySize) * 8);
				rtti_write(i, stream);

				const MaterialParamTextureData& textureData = mTextureParams[param.Index];
				MaterialParamTextureDataCore* coreTexData = (MaterialParamTextureDataCore*)stream.Cursor();
				new (coreTexData) MaterialParamTextureDataCore();

				coreTexData->IsLoadStore = textureData.IsLoadStore;
				coreTexData->Surface = textureData.Surface;

				if (textureData.Texture.IsLoaded())
					coreTexData->Texture = textureData.Texture->GetCore();

				if (textureData.SpriteTexture.IsLoaded())
					coreTexData->SpriteTexture = textureData.SpriteTexture->GetCore();

				dirtyTextureParamIdx++;
			}
				break;
			case ParamType::Buffer:
			{
				stream.Seek((bufferParamsOffset + dirtyBufferParamIdx * bufferEntrySize) * 8);
				rtti_write(i, stream);

				const MaterialParamBufferData& bufferData = mBufferParams[param.Index];
				MaterialParamBufferDataCore* coreBufferData = (MaterialParamBufferDataCore*)stream.Cursor();
				new (coreBufferData) MaterialParamBufferDataCore();

				if(bufferData.Value != nullptr)
					coreBufferData->Value = bufferData.Value->GetCore();

				dirtyBufferParamIdx++;
			}
				break;
			case ParamType::Sampler:
			{
				stream.Seek((samplerStateParamsOffset + dirtySamplerParamIdx * samplerStateEntrySize) * 8);
				rtti_write(i, stream);

				const MaterialParamSamplerStateData& samplerData = mSamplerStateParams[param.Index];
				MaterialParamSamplerStateDataCore* coreSamplerData = (MaterialParamSamplerStateDataCore*)stream.Cursor();
				new (coreSamplerData) MaterialParamSamplerStateDataCore();

				if (samplerData.Value != nullptr)
					coreSamplerData->Value = samplerData.Value->GetCore();

				dirtySamplerParamIdx++;
			}
				break;
			}
		}

		mLastSyncVersion = mParamVersion;
	}

	void MaterialParams::GetResourceDependencies(Vector<HResource>& resources)
	{
		for (UINT32 i = 0; i < (UINT32)mParams.size(); i++)
		{
			ParamData& param = mParams[i];
			if (param.Type != ParamType::Texture)
				continue;

			const MaterialParamTextureData& textureData = mTextureParams[param.Index];
			if (textureData.Texture != nullptr)
				resources.push_back(textureData.Texture);

			if (textureData.SpriteTexture != nullptr)
				resources.push_back(textureData.SpriteTexture);
		}
	}

	void MaterialParams::GetCoreObjectDependencies(Vector<CoreObject*>& coreObjects)
	{
		for (UINT32 i = 0; i < (UINT32)mParams.size(); i++)
		{
			ParamData& param = mParams[i];

			switch (param.Type)
			{
			case ParamType::Texture:
			{
				const MaterialParamTextureData& textureData = mTextureParams[param.Index];

				if (textureData.Texture.IsLoaded())
					coreObjects.push_back(textureData.Texture.Get());

				if (textureData.SpriteTexture.IsLoaded())
					coreObjects.push_back(textureData.SpriteTexture.Get());
			}
			break;
			case ParamType::Buffer:
			{
				const MaterialParamBufferData& bufferData = mBufferParams[param.Index];

				if (bufferData.Value != nullptr)
					coreObjects.push_back(bufferData.Value.get());
			}
			break;
			case ParamType::Sampler:
			{

				const MaterialParamSamplerStateData& samplerData = mSamplerStateParams[param.Index];

				if (samplerData.Value != nullptr)
					coreObjects.push_back(samplerData.Value.get());
			}
			break;
			default:
				break;
			}
		}
	}

	RTTITypeBase* MaterialParams::GetRttiStatic()
	{
		return MaterialParamsRTTI::Instance();
	}

	RTTITypeBase* MaterialParams::GetRtti() const
	{
		return MaterialParams::GetRttiStatic();
	}

	namespace ct
	{
	MaterialParams::MaterialParams(const SPtr<Shader>& shader, UINT64 initialParamVersion)
		:TMaterialParams(shader, initialParamVersion)
	{ }

	MaterialParams::MaterialParams(const SPtr<Shader>& shader, const SPtr<bs::MaterialParams>& params)
		: TMaterialParams(shader, 1)
	{
		memcpy(mDataParamsBuffer, params->mDataParamsBuffer, mDataSize);

		for (auto& param : mParams)
		{
			switch (param.Type)
			{
			case ParamType::Data:
				{
					const UINT32 arraySize = param.ArraySize > 1 ? param.ArraySize : 1;

					if(param.DataType == GPDT_STRUCT)
					{
						for (UINT32 i = 0; i < arraySize; i++)
						{
							const MaterialParamStructData& srcParamInfo = params->mStructParams[param.Index + i];
							MaterialParamStructDataCore& dstParamInfo = mStructParams[param.Index + i];

							memcpy(dstParamInfo.Data, srcParamInfo.Data, srcParamInfo.DataSize);
						}
					}
					else
					{
						for (UINT32 i = 0; i < arraySize; i++)
						{
							DataParamInfo& srcParamInfo = params->mDataParams[param.Index + i];
							DataParamInfo& dstParamInfo = mDataParams[param.Index + i];

							if (srcParamInfo.FloatCurve)
								dstParamInfo.FloatCurve = bs_pool_new<TAnimationCurve<float>>(*srcParamInfo.FloatCurve);

							if (srcParamInfo.ColorGradient)
								dstParamInfo.ColorGradient = bs_pool_new<ColorGradientHDR>(*srcParamInfo.ColorGradient);
						}
					}
				}
				break;
			case ParamType::Texture:
			{
				HTexture texture = params->mTextureParams[param.Index].Texture;
				SPtr<Texture> textureCore;
				if (texture.IsLoaded())
					textureCore = texture->GetCore();

				mTextureParams[param.Index].Texture = textureCore;

				HSpriteTexture spriteTexture = params->mTextureParams[param.Index].SpriteTexture;
				SPtr<SpriteTexture> spriteTextureCore;
				if (spriteTexture.IsLoaded())
					spriteTextureCore = spriteTexture->GetCore();

				mTextureParams[param.Index].SpriteTexture = spriteTextureCore;

				mTextureParams[param.Index].IsLoadStore = params->mTextureParams[param.Index].IsLoadStore;
				mTextureParams[param.Index].Surface = params->mTextureParams[param.Index].Surface;
			}
				break;
			case ParamType::Buffer:
			{
				SPtr<bs::GpuBuffer> buffer = params->mBufferParams[param.Index].Value;
				SPtr<GpuBuffer> bufferCore;
				if (buffer != nullptr)
					bufferCore = buffer->GetCore();

				mBufferParams[param.Index].Value = bufferCore;
			}
				break;
			case ParamType::Sampler:
			{
				SPtr<bs::SamplerState> sampState = params->mSamplerStateParams[param.Index].Value;
				SPtr<SamplerState> sampStateCore;
				if (sampState != nullptr)
					sampStateCore = sampState->GetCore();

				mSamplerStateParams[param.Index].Value = sampStateCore;
			}
				break;
			default:
				break;
			}
		}
	}

	void MaterialParams::SetSyncData(UINT8* buffer, UINT32 size)
	{
		Bitstream stream((uint8_t*)buffer, size);

		UINT32 numDirtyDataParams = 0;
		UINT32 numDirtyTextureParams = 0;
		UINT32 numDirtyBufferParams = 0;
		UINT32 numDirtySamplerParams = 0;
		UINT32 numDirtyStructParams = 0;

		rtti_read(numDirtyDataParams, stream);
		rtti_read(numDirtyTextureParams, stream);
		rtti_read(numDirtyBufferParams, stream);
		rtti_read(numDirtySamplerParams, stream);
		rtti_read(numDirtyStructParams, stream);

		mParamVersion++;

		for(UINT32 i = 0; i < numDirtyDataParams; i++)
		{
			// Param index
			UINT32 paramIdx = 0;
			rtti_read(paramIdx, stream);

			ParamData& param = mParams[paramIdx];
			param.Version = mParamVersion;

			const UINT32 arraySize = param.ArraySize > 1 ? param.ArraySize : 1;
			const GpuParamDataTypeInfo& typeInfo = bs::GpuParams::PARAM_SIZES.Lookup[(int)param.DataType];
			const UINT32 paramSize = typeInfo.NumColumns * typeInfo.NumRows * typeInfo.BaseTypeSize;

			const DataParamInfo& paramInfo = mDataParams[param.Index];

			const UINT32 dataParamSize = arraySize * paramSize;

			// Param data
			// Note: This relies on the fact that all data params in the array are sequential
			stream.ReadBytes(&mDataParamsBuffer[paramInfo.Offset], dataParamSize);

			// Param curves
			UINT32 numDirtyCurves = 0;
			rtti_read(numDirtyCurves, stream);
			for(UINT32 j = 0; j < numDirtyCurves; j++)
			{
				UINT32 localIdx = 0;
				rtti_read(localIdx, stream);

				DataParamInfo& arrParamInfo = mDataParams[param.Index + localIdx];
				if (param.DataType == GPDT_FLOAT1)
				{
					if(arrParamInfo.FloatCurve)
						bs_pool_free(arrParamInfo.FloatCurve);

					arrParamInfo.FloatCurve = bs_pool_new<TAnimationCurve<float>>();
					rtti_read(*arrParamInfo.FloatCurve, stream);
				}
				else if (param.DataType == GPDT_COLOR)
				{
					if(arrParamInfo.ColorGradient)
						bs_pool_free(arrParamInfo.ColorGradient);

					arrParamInfo.ColorGradient = bs_pool_new<ColorGradientHDR>();
					rtti_read(*arrParamInfo.ColorGradient, stream);
				}
			}
		}

		for(UINT32 i = 0; i < numDirtyTextureParams; i++)
		{
			UINT32 paramIdx = 0;
			rtti_read(paramIdx, stream);

			ParamData& param = mParams[paramIdx];
			param.Version = mParamVersion;

			MaterialParamTextureDataCore* sourceTexData = (MaterialParamTextureDataCore*)stream.Cursor();
			stream.SkipBytes(sizeof(MaterialParamTextureDataCore));

			mTextureParams[param.Index] = *sourceTexData;
			sourceTexData->~MaterialParamTextureDataCore();
		}

		for (UINT32 i = 0; i < numDirtyBufferParams; i++)
		{
			UINT32 paramIdx = 0;
			rtti_read(paramIdx, stream);

			ParamData& param = mParams[paramIdx];
			param.Version = mParamVersion;

			MaterialParamBufferDataCore* sourceBufferData = (MaterialParamBufferDataCore*)stream.Cursor();
			stream.SkipBytes(sizeof(MaterialParamBufferDataCore));

			mBufferParams[param.Index] = *sourceBufferData;
			sourceBufferData->~MaterialParamBufferDataCore();
		}

		for (UINT32 i = 0; i < numDirtySamplerParams; i++)
		{
			UINT32 paramIdx = 0;
			rtti_read(paramIdx, stream);

			ParamData& param = mParams[paramIdx];
			param.Version = mParamVersion;

			MaterialParamSamplerStateDataCore* sourceSamplerStateData = (MaterialParamSamplerStateDataCore*)stream.Cursor();
			stream.SkipBytes(sizeof(MaterialParamSamplerStateDataCore));

			mSamplerStateParams[param.Index] = *sourceSamplerStateData;
			sourceSamplerStateData->~MaterialParamSamplerStateDataCore();
		}

		for(UINT32 i = 0; i < numDirtyStructParams; i++)
		{
			// Param index
			UINT32 paramIdx = 0;
			rtti_read(paramIdx, stream);

			ParamData& param = mParams[paramIdx];
			param.Version = mParamVersion;

			const UINT32 arraySize = param.ArraySize > 1 ? param.ArraySize : 1;
			const ParamStructDataType& paramData = mStructParams[param.Index];

			// Param data
			for (UINT32 j = 0; j < arraySize; j++)
				stream.ReadBytes(mStructParams[param.Index + j].Data, paramData.DataSize);
		}
	}
	}
}
