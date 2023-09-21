//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsGUIStyleSheet.h"
#include "BsPrerequisites.h"
#include "BsGUIStyleSheetLexer.h"
#include "Image/BsColor.h"
#include "Utility/BsBitfield.h"
#include "Utility/BsRectOffset.h"

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	// TODO - Doc
	class GUIStyleSheetParser
	{
		using Token = GUIStyleSheetToken;
		using TokenType = GUIStyleSheetTokenTypes;
		using Lexer = GUIStyleSheetLexer;
	public:
		GUIStyleSheetParser();
		bool Parse(const SPtr<SourceCode>& sourceCode);
		const String& GetErrors() const { return mErrors; }
		
	private:
		enum class ValueType
		{
			Undefined,
			Integer,
			Pixel,
			Decimal,
			Percent,
			Color,
			String,
			BorderStyle,
			TextAlign,
			VerticalAlign,
			Multiple
		};

		struct VariableValue
		{
			union
			{
				u32 UnsignedInteger;
				i32 SignedInteger;
				float Float;
				Color Color;
			};

			ValueType Type = ValueType::Undefined;

			VariableValue()
				: UnsignedInteger(0)
			{ }

			VariableValue(u32 value, ValueType type)
				: UnsignedInteger(value), Type(type)
			{ }

			VariableValue(i32 value, ValueType type)
				: SignedInteger(value), Type(type)
			{ }

			VariableValue(float value, ValueType type)
				: Float(value), Type(type)
			{ }

			VariableValue(const class Color& value, ValueType type)
				: Color(value), Type(type)
			{ }

			void GetValue(u32& outValue) const
			{
				B3D_ASSERT(Type == ValueType::Integer || Type == ValueType::Pixel || Type == ValueType::String);
				outValue = UnsignedInteger;
			}

			void GetValue(i32& outValue) const
			{
				B3D_ASSERT(Type == ValueType::Integer || Type == ValueType::Pixel || Type == ValueType::String);
				outValue = SignedInteger;
			}

			void GetValue(float& outValue) const
			{
				B3D_ASSERT(Type == ValueType::Decimal || Type == ValueType::Percent);
				outValue = Float;
			}

			void GetValue(class Color& outValue) const
			{
				B3D_ASSERT(Type == ValueType::Color);
				outValue = Color;
			}

			void GetValue(GUIBorderElementStyle& outValue)
			{
				B3D_ASSERT(Type == ValueType::BorderStyle);
				outValue = (GUIBorderElementStyle)UnsignedInteger;
			}

			void GetValue(GUIHorizontalTextAlignment& outValue)
			{
				B3D_ASSERT(Type == ValueType::TextAlign);
				outValue = (GUIHorizontalTextAlignment)UnsignedInteger;
			}

			void GetValue(GUIVerticalTextAlignment& outValue)
			{
				B3D_ASSERT(Type == ValueType::VerticalAlign);
				outValue = (GUIVerticalTextAlignment)UnsignedInteger;
			}
		};

		struct VariableContext
		{
			UnorderedMap<String, VariableValue> Variables;
		};

		struct PropertyInformation
		{
			PropertyInformation(GUIStyleSheetPropertyType propertyType = GUIStyleSheetPropertyType::Undefined, ValueType valueType = ValueType::Undefined)
				: PropertyType(propertyType), ValueType(valueType) {}

			GUIStyleSheetPropertyType PropertyType = GUIStyleSheetPropertyType::Undefined;
			ValueType ValueType = ValueType::Undefined;
		};

		// Property value (Literals)
		bool TryParseIntegerLiteral(i32& outValue);
		bool TryParseIntegerLiteral(u32& outValue);
		bool TryParsePixelLiteral(i32& outValue);
		bool TryParsePixelLiteral(u32& outValue);
		bool TryParseDecimalLiteral(float& outValue);
		bool TryParsePercentLiteral(float& outValue);
		bool TryParseStringLiteral(String& outValue);

		// Property value (Complex)
		bool TryParseColor(Color& outValue);
		bool TryParseRectOffset(RectOffset& outValue);
		bool TryParseBorderStyle(GUIBorderElementStyle& outValue);
		bool TryParseBorderStyle(GUIBorderElementStyle& outTop, GUIBorderElementStyle& outRight, GUIBorderElementStyle& outBottom, GUIBorderElementStyle& outLeft);
		bool TryParseBorderWidth(u32& outTop, u32& outRight, u32& outBottom, u32& outLeft);
		bool TryParseBorderColor(Color& outTop, Color& outRight, Color& outBottom, Color& outLeft);
		bool TryParseTextAlign(GUIHorizontalTextAlignment& outValue);
		bool TryParseVerticalAlign(GUIVerticalTextAlignment& outValue);
		bool TryParseBorderElement(GUIBorderElement& outValue);
		bool TryParseBorderRadius(u32& outTopLeft, u32& outTopRight, u32& outBottomLeft, u32& outBottomRight);
		bool TryParseAndLookupVariableValue(ValueType expectedType, VariableValue& outValue);

		// Higher level constructs
		bool TryParseProperty(GUIStyleSheetStateStyle& inOutValue);
		bool TryParseVariable(VariableContext& inOutVariableContext);
		bool TryParseSelector();

		// Parse helpers
		template<class T>
		bool TryParsePropertyValueSides(ValueType valueType, T& outTop, T& outRight, T& outBottom, T& outLeft);

		template<class T>
		bool TryParsePropertyValue(ValueType valueType, T& outValue);

		template<class T>
		bool TryParseAndLookupVariableValue(ValueType expectedType, T& outValue);
		bool TryParseAndLookupVariableValue(ValueType expectedType, String& outValue);

		bool TryParseInteger(const StringView& toParse, i32& outValue) const;
		bool TryParseFloat(const StringView& toParse, float& outValue) const;
		bool TryParseHexColor(const StringView& toParse, Color& outValue) const;

		// Token iteration and lookup
		bool IsCurrentToken(TokenType type) const;
		bool IsCurrentToken(TokenType type, const String& spelling) const;

		TokenType GetCurrentTokenType() const { return mCurrentToken.has_value() ? mCurrentToken->GetType() : GUIStyleSheetTokenTypes::Undefined; }
		Optional<Token> GetCurrentToken() const { return mCurrentToken; }
		Optional<Token> GetCurrentTokenAndAdvance();
		Optional<Token> GetCurrentTokenAndAdvance(TokenType expectedType);
		Optional<Token> GetCurrentTokenAndAdvance(TokenType expectedType, const String& spelling);
		void SkipToken(TokenType type);

		// Error handling
		Optional<Token> Error(const String& message);
		Optional<Token> ErrorUnexpected();
		Optional<Token> ErrorUnexpected(TokenType expectedTokenType);
		Optional<Token> ErrorUnexpected(const String& expectedTokenSpelling);

		static const char* ValueTypeToString(ValueType type);

		SPtr<SourceCode> mSourceCode;
		GUIStyleSheetLexer mLexer;
		Optional<Token> mCurrentToken;
		VariableContext mGlobalVariableContext;
		VariableContext mLocalVariableContext;
		Vector<String> mStringLiterals;

		String mErrors;

		UnorderedMap<String, PropertyInformation> mPropertyKeywords;
	};


	/** @} */
} // namespace bs
