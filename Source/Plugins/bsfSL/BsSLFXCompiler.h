//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsSLPrerequisites.h"
#include "Material/BsShader.h"
#include "RenderAPI/BsGpuProgram.h"
#include "RenderAPI/BsRasterizerState.h"
#include "RenderAPI/BsDepthStencilState.h"
#include "RenderAPI/BsBlendState.h"
#include "Importer/BsShaderImportOptions.h"

extern "C" {
#include "BsASTFX.h"
}

namespace bs
{
	/** @addtogroup bsfSL
	 *  @{
	 */

	/**	Contains the results of compilation returned from the BSLFXCompiler. */
	struct BSLFXCompileResult
	{
		String ErrorMessage; /**< Error message if compilation failed. */
		int ErrorLine = 0; /**< Line of the error if one occurred. */
		int ErrorColumn = 0; /**< Column of the error if one occurred. */
		String ErrorFile; /**< File in which the error occurred. Empty if root file. */
	};

	/** Meta-data for a shader compiled from BSL. Can be used for compiling specific variations of the shader. */
	struct BSLFXShaderMetaData
	{
		String Name;
		ShaderCreateInformation ShaderInformation; /**< Information used to create the Shader object. **/
		SmallVector<GpuProgramType, 2> GPUProgramTypes; /**< Types of GPU programs used by the shader. */
		Vector<ShaderVariation> Variations; /**< Sets of defines controlling which variations of the shader are present. */
		Vector<String> Includes; /**< All shader header files referenced by the shader. */
		UnorderedMap<String, String> Defines; /**< Optional list of defines to provide when compiling the shader variations. This is added along with the shader variation defines. */
		bool HasGPUProgramMetaData = true; /**< True if GPU program information has been initialized. This will only be true after the first variation for the shader has been compiled. */
	};

	/**	Transforms a source file written in BSL FX syntax into a Shader object. */
	class BSLFXCompiler
	{
		/**	Possible types of code blocks within a shader. */
		enum class CodeBlockType
		{
			Vertex,
			Fragment,
			Geometry,
			Hull,
			Domain,
			Compute,
			Common
		};

		/**	Temporary data describing a pass during parsing. */
		struct ParsedShaderPassNode
		{
			BLEND_STATE_DESC BlendDesc;
			RASTERIZER_STATE_DESC RasterizerDesc;
			DEPTH_STENCIL_STATE_DESC DepthStencilDesc;
			u32 StencilRefValue = 0;
			u32 SeqIdx = 0;

			bool BlendIsDefault = true;
			bool RasterizerIsDefault = true;
			bool DepthStencilIsDefault = true;

			String Code; // Parsed code block
		};

		/** A set of attributes describing a BSL construct. */
		struct AttributeData
		{
			Vector<std::pair<i32, String>> Attributes;
		};

		/** Value of a single variation option along with an optional name. */
		struct VariationOption
		{
			String Name;
			u32 Value;
		};

		/** Information about different variations of a single shader. */
		struct VariationData
		{
			String Name;
			String Identifier;
			bool Internal = true;
			Vector<VariationOption> Values;
		};

		/** Information describing a shader/mixin node, without the actual contents. */
		struct ParsedShaderMetaData
		{
			String Name;
			Vector<String> Includes;
			bool IsMixin;

			String Language;
			String FeatureSet;

			Vector<StringID> Tags;
			Vector<VariationData> Variations;
		};

		/** Temporary data for describing a shader/mixin node during parsing. */
		struct ParsedShaderOrMixinNode
		{
			ParsedShaderMetaData MetaData;
			Vector<ParsedShaderPassNode> Passes;
		};

	public:
		/**
		 * Compiles the shader from BSL and outputs a Shader object with all possible variations compiled and ready to use.
		 *
		 * @param		name				Name used to identify the shader.
		 * @param		source				BSL source to compile.
		 * @param		defines				An optional set of defines to set during compilation.
		 * @param		languages			Low-level languages to compile individual variations for. Each language will result in another set of variations.
		 * @param		outShader			Shader if the compilation is successful, null otherwise.
		 * @return							A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult Compile(const String& name, const String& source, const UnorderedMap<String, String>& defines, ShadingLanguageFlags languages, SPtr<Shader>& outShader);

		/**
		 * Parses the BSL source and outputs meta-data that may be used for constructing the Shader object and compiling individual variations. After creating the Shader this way, you must manually compile the techniques by calling CompileVariation().
		 *
		 * @param		source				BSL source to parse.
		 * @param		defines				An optional set of defines to set before parsing the source.
		 * @param		outShaderMetaData	Shader meta-data if the parsing is successful, null otherwise.
		 * @return							A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult CompileMetaData(const String& source, const UnorderedMap<String, String>& defines, SPtr<BSLFXShaderMetaData>& outShaderMetaData);

		/**
		 * Compiles a particular shader variation.
		 *
		 * @param		source					BSL source to compile.
		 * @param		variation				Specific variation to compile.
		 * @param		language				Language to compile the variation for. Must be a single language, rather than a mask of multiple languages.
		 * @param		inOutShaderMetaData		Shader meta-data as returned from CompileMetaData(). Mutable as internally it caches values so that it can speed up compilation of other variations.
		 * @param		outVariation			Compiled variation if successful, null otherwise.
		 * @return								A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult CompileVariation(const String& source, const ShaderVariation& variation, ShadingLanguageFlag language, BSLFXShaderMetaData& inOutShaderMetaData, SPtr<Technique>& outVariation);

	private:
		/** Converts the provided source into an abstract syntax tree using the lexer & parser for BSL FX syntax. */
		static BSLFXCompileResult ParseFx(ParseState* parseState, const char* source, const UnorderedMap<String, String>& defines);

		/** Parses the shader/mixin node and outputs the relevant meta-data. */
		static ParsedShaderMetaData ParseShaderMetaData(ASTFXNode* shader);

		/**
		 * Parses the root AST node and outputs a list of all mixins/shaders and their meta-data, sub-shader meta-data,
		 * as well as any global shader options.
		 */
		static BSLFXCompileResult ParseMetaDataAndOptions(ASTFXNode* rootNode, Vector<std::pair<ASTFXNode*, ParsedShaderMetaData>>& metaData, ShaderCreateInformation& shaderCreateInformation);

		/** Parses shader variations and writes them to the provided meta-data object. */
		static void ParseVariations(ParsedShaderMetaData& metaData, ASTFXNode* variations);

		/** Parses a single variation option node. */
		static VariationOption ParseVariationOption(ASTFXNode* variationOption);

		/** Parses BSL attributes. */
		static AttributeData ParseAttributes(ASTFXNode* attributes);

		/**	Maps BSL queue sort type enum into in-engine queue sort type mode. */
		static QueueSortType ParseSortType(CullAndSortModeValue sortType);

		/**	Maps BSL comparison function enum into in-engine compare function. */
		static CompareFunction ParseCompFunc(CompFuncValue compFunc);

		/**	Maps BSL operation to in-engine blend factor. */
		static BlendFactor ParseBlendFactor(OpValue factor);

		/**	Maps BSL blend operation to in-engine blend operation. */
		static BlendOperation ParseBlendOp(BlendOpValue op);

		/**	Maps BSL operation to in-engine stencil operation. */
		static StencilOperation ParseStencilOp(OpValue op);

		/**	Maps BSL cull mode enum to in-engine cull mode. */
		static CullingMode ParseCullMode(CullAndSortModeValue cm);

		/**	Maps BSL fill mode enum to in-engine fill mode. */
		static PolygonMode ParseFillMode(FillModeValue fm);

		/**
		 * Populates the front facing operation portion of the depth-stencil state descriptor from the provided stencil-op
		 * AST node.
		 */
		static void ParseStencilFront(DEPTH_STENCIL_STATE_DESC& desc, ASTFXNode* stencilOpNode);

		/**
		 * Populates the back backing operation portion of the depth-stencil state descriptor from the provided stencil-op
		 * AST node.
		 */
		static void ParseStencilBack(DEPTH_STENCIL_STATE_DESC& desc, ASTFXNode* stencilOpNode);

		/** Populates the color (RGB) portion of the blend state descriptor from the provided blend definition AST node. */
		static void ParseColorBlendDef(RENDER_TARGET_BLEND_STATE_DESC& desc, ASTFXNode* blendDefNode);

		/** Populates the alpha portion of the blend state descriptor from the provided blend definition AST node. */
		static void ParseAlphaBlendDef(RENDER_TARGET_BLEND_STATE_DESC& desc, ASTFXNode* blendDefNode);

		/**
		 * Populates blend state descriptor for a single render target from the provided AST node. Which target gets
		 * updated depends on the index set in the AST node.
		 */
		static void ParseRenderTargetBlendState(BLEND_STATE_DESC& desc, ASTFXNode* targetNode, u32& index);

		/**
		 * Parses the blend state AST node and populates the pass' blend state descriptor. Returns false if the descriptor
		 * wasn't modified.
		 */
		static bool ParseBlendState(ParsedShaderPassNode& passData, ASTFXNode* blendNode);

		/**
		 * Parses the rasterizer state AST node and populates the pass' rasterizer state descriptor. Returns false if the
		 * descriptor wasn't modified.
		 */
		static bool ParseRasterizerState(ParsedShaderPassNode& passData, ASTFXNode* rasterNode);

		/**
		 * Parses the depth state AST node and populates the pass' depth-stencil state descriptor. Returns false if the
		 * descriptor wasn't modified.
		 */
		static bool ParseDepthState(ParsedShaderPassNode& passData, ASTFXNode* depthNode);

		/**
		 * Parses the stencil state AST node and populates the pass' depth-stencil state descriptor. Returns false if the
		 * descriptor wasn't modified.
		 */
		static bool ParseStencilState(ParsedShaderPassNode& passData, ASTFXNode* stencilNode);

		/**
		 * Parses a code AST node and outputs the result in one of the streams within the provided pass data.
		 *
		 * @param[in]	codeNode	AST node to parse
		 * @param[in]	codeBlocks	GPU program source code.
		 * @param[in]	passData	Pass data containing temporary pass data, including the code streams that the code
		 *							block code will be written to.
		 */
		static void ParseCodeBlock(ASTFXNode* codeNode, const Vector<String>& codeBlocks, ParsedShaderPassNode& passData);

		/**
		 * Parses the pass AST node and populates the provided @p passData with all relevant pass parameters.
		 *
		 * @param[in]	passNode		Node to parse.
		 * @param[in]	codeBlocks		GPU program source code.
		 * @param[out]	passData		Will contain pass data after parsing.
		 */
		static void ParsePass(ASTFXNode* passNode, const Vector<String>& codeBlocks, ParsedShaderPassNode& passData);

		/**
		 * Parses the shader AST node and generates a single shader object.
		 *
		 * @param[in]	shaderNode		Node to parse.
		 * @param[in]	codeBlocks		GPU program source code.
		 * @param[out]	shaderData		Will contain shader data after parsing.
		 */
		static void ParseShader(ASTFXNode* shaderNode, const Vector<String>& codeBlocks, ParsedShaderOrMixinNode& shaderData);

		/**
		 * Parser the options AST node that contains global shader options.
		 *
		 * @param[in]	optionsNode		Node to parse.
		 * @param[in]	shaderDesc		Descriptor to apply the found options to.
		 */
		static void ParseOptions(ASTFXNode* optionsNode, ShaderCreateInformation& shaderDesc);

		/**
		 * Parses the BSL source and outputs meta-data for every shader and mixin node.
		 *
		 * @param	source					BSL source to parse.
		 * @param	defines					An optional set of defines to set before parsing the source.
		 * @param	inOutShaderInformation	Object to append shader reflection data to.
		 * @param	outShaderMetaData		Parsed shader meta-data if parse was successful.
		 * @param	outIncludes				A list of all includes files included by the BSL source.
		 * @return							A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult ParseMetaData(const String& source, const UnorderedMap<String, String>& defines, ShaderCreateInformation& inOutShaderInformation, ParsedShaderMetaData& outShaderMetaData, Vector<String>& outIncludes);

		/**
		 * Parses the BSL shader source for a specific variation.
		 *
		 * @param	name				Name used to identify the shader.
		 * @param	source				BSL source to parse.
		 * @param	variation			Variation to enable when parsing the source.
		 * @param	defines				Optional set of defines to enable during parse.
		 * @param	outParsedShader		Output information about the parsed variation if succesful.
		 * @return						A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult ParseVariation(const String& name, const String& source, const ShaderVariation& variation, const UnorderedMap<String, String>& defines, ParsedShaderOrMixinNode& outParsedShader);

		/**
		 * Iterates over all provided mixins/shaders and inherits any variations. The variations are written in-place, to
		 * the @p shaderMetaData array, for any non-mixins.
		 */
		static BSLFXCompileResult PopulateVariations(Vector<std::pair<ASTFXNode*, ParsedShaderMetaData>>& shaderMetaData);

		/** Populates the information about variation parameters and their values. */
		static void PopulateVariationParamInfos(const ParsedShaderMetaData& shaderMetaData, ShaderCreateInformation& desc);

		/**
		 * Compiles a particular shader variation.
		 *
		 * @param	name			Name used to identify the shader.
		 * @param	parsedShader	Parsed shader information, as output by the BSL parser.
		 * @param	shaderMetaData	Meta-data about the shader we're compiling the variation for.
		 * @param	variation		Variation to compile for. Must be the same variation that was provided when parsing the shader.
		 * @param	language		Language to compile the variation for. Must be a single language, rather than a mask of multiple languages.
		 * @param	outVariation	Compiled variation if successful, null otherwise.
		 * @return					A result object containing an error message if not successful.
		 */
		static BSLFXCompileResult CompileVariation(const String& name, const ParsedShaderOrMixinNode& parsedShader, const BSLFXShaderMetaData& shaderMetaData, const ShaderVariation& variation, ShadingLanguageFlag language, SPtr<Technique>& outVariation);

		/**
		 * Converts a null-terminated string into a standard string, and eliminates quotes that are assumed to be at the
		 * first and last index.
		 */
		static String RemoveQuotes(const char* input);

		/** Converts internal variation representations in the shader meta-data into a set of ShaderVariation objects. */
		static Vector<ShaderVariation> CreateShaderVariations(const ParsedShaderMetaData& shaderMetaData);
	};

	/** @} */
} // namespace bs
