//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Localization/BsStringTable.h"
#include "Error/BsException.h"
#include "Resources/BsResources.h"
#include "Private/RTTI/BsStringTableRTTI.h"

namespace bs
{
	const Language StringTable::DEFAULT_LANGUAGE = Language::EnglishUS;

	LocalizedStringData::~LocalizedStringData()
	{
		if(ParameterOffsets != nullptr)
			bs_deleteN(ParameterOffsets, NumParameters);
	}

	void LocalizedStringData::ConcatenateString(bs::String& outputString, bs::String* parameters, UINT32 numParameterValues) const
	{
		// A safeguard in case translated strings have different number of parameters
		UINT32 actualNumParameters = std::min(numParameterValues, NumParameters);
		
		if(parameters != nullptr)
		{
			UINT32 totalNumChars = 0;
			UINT32 prevIdx = 0;
			for(UINT32 i = 0; i < actualNumParameters; i++)
			{
				totalNumChars += (ParameterOffsets[i].Location - prevIdx) + (UINT32)parameters[ParameterOffsets[i].ParamIdx].size();;

				prevIdx = ParameterOffsets[i].Location;
			}

			totalNumChars += (UINT32)String.size() - prevIdx;

			outputString.resize(totalNumChars);
			char* strData = &outputString[0]; // String contiguity required by C++11, but this should work elsewhere as well

			prevIdx = 0;
			for(UINT32 i = 0; i < actualNumParameters; i++)
			{
				UINT32 strSize = ParameterOffsets[i].Location - prevIdx;
				memcpy(strData, &String[prevIdx], strSize * sizeof(char));
				strData += strSize;

				bs::String& param = parameters[ParameterOffsets[i].ParamIdx];
				memcpy(strData, &param[0], param.size() * sizeof(char));
				strData += param.size();

				prevIdx = ParameterOffsets[i].Location;
			}

			memcpy(strData, &String[prevIdx], (String.size() - prevIdx) * sizeof(char));
		}
		else
		{
			outputString.resize(String.size());
			char* strData = &outputString[0]; // String contiguity required by C++11, but this should work elsewhere as well

			memcpy(strData, &String[0], String.size() * sizeof(char));
		}
	}

	void LocalizedStringData::UpdateString(const bs::String& _string)
	{
		if(ParameterOffsets != nullptr)
			bs_deleteN(ParameterOffsets, NumParameters);

		Vector<ParamOffset> paramOffsets;

		INT32 lastBracket = -1;
		StringStream bracketChars;
		StringStream cleanString;
		bool escaped = false;
		UINT32 numRemovedChars = 0;
		for(UINT32 i = 0; i < (UINT32)_string.size(); i++)
		{
			if(_string[i] == '^' && !escaped)
			{
				numRemovedChars++;
				escaped = true;
				continue;
			}

			if(lastBracket == -1)
			{
				// If current char is non-escaped opening bracket start parameter definition
				if(_string[i] == '{' && !escaped)
					lastBracket = i;
				else
					cleanString<<_string[i];
			}
			else
			{
				if(isdigit(_string[i]))
					bracketChars<<_string[i];
				else
				{
					// If current char is non-escaped closing bracket end parameter definition
					UINT32 numParamChars = (UINT32)bracketChars.tellp();
					if(_string[i] == '}' && numParamChars > 0 && !escaped)
					{
						numRemovedChars += numParamChars + 2; // +2 for open and closed brackets

						UINT32 paramIdx = parseUINT32(bracketChars.str());
						paramOffsets.push_back(ParamOffset(paramIdx, i + 1 - numRemovedChars));
					}
					else
					{
						// Last bracket wasn't really a parameter
						for(UINT32 j = lastBracket; j <= i; j++)
							cleanString<<_string[j];
					}

					lastBracket = -1;

					bracketChars.str(u8"");
					bracketChars.clear();
				}
			}

			escaped = false;
		}

		String = cleanString.str();
		NumParameters = (UINT32)paramOffsets.size();

		// Try to find out of order param offsets and fix them
		std::sort(begin(paramOffsets), end(paramOffsets),
			[&] (const ParamOffset& a, const ParamOffset& b) { return a.ParamIdx < b.ParamIdx; } );

		if(paramOffsets.size() > 0)
		{
			UINT32 sequentialIdx = 0;
			UINT32 lastParamIdx = paramOffsets[0].ParamIdx;
			for(UINT32 i = 0; i < NumParameters; i++)
			{
				if(paramOffsets[i].ParamIdx == lastParamIdx)
				{
					paramOffsets[i].ParamIdx = sequentialIdx;
					continue;
				}

				lastParamIdx = paramOffsets[i].ParamIdx;
				sequentialIdx++;

				paramOffsets[i].ParamIdx = sequentialIdx;
			}
		}

		// Re-sort based on location since we find that more useful at runtime
		std::sort(begin(paramOffsets), end(paramOffsets),
			[&] (const ParamOffset& a, const ParamOffset& b) { return a.Location < b.Location; } );

		ParameterOffsets = bs_newN<ParamOffset>(NumParameters);
		for(UINT32 i = 0; i < NumParameters; i++)
			ParameterOffsets[i] = paramOffsets[i];
	}

	StringTable::StringTable()
		:Resource(false), mActiveLanguageData(nullptr), mDefaultLanguageData(nullptr), mAllLanguages(nullptr)
	{
		mAllLanguages = bs_newN<LanguageData>((UINT32)Language::Count);

		mDefaultLanguageData = &(mAllLanguages[(UINT32)DEFAULT_LANGUAGE]);
		mActiveLanguageData = mDefaultLanguageData;
		mActiveLanguage = DEFAULT_LANGUAGE;
	}
	
	StringTable::~StringTable()
	{
		bs_deleteN(mAllLanguages, (UINT32)Language::Count);
	}

	void StringTable::SetActiveLanguage(Language language)
	{
		if(language == mActiveLanguage)
			return;

		mActiveLanguageData = &(mAllLanguages[(UINT32)language]);
		mActiveLanguage = language;
	}

	bool StringTable::Contains(const String& identifier)
	{
		return mIdentifiers.find(identifier) == mIdentifiers.end();
	}

	Vector<String> StringTable::GetIdentifiers() const
	{
		Vector<String> output;
		for (auto& entry : mIdentifiers)
			output.push_back(entry);

		return output;
	}

	void StringTable::SetString(const String& identifier, Language language, const String& value)
	{
		LanguageData* curLanguage = &(mAllLanguages[(UINT32)language]);

		auto iterFind = curLanguage->Strings.find(identifier);

		SPtr<LocalizedStringData> stringData;
		if(iterFind == curLanguage->Strings.end())
		{
			stringData = bs_shared_ptr_new<LocalizedStringData>();
			curLanguage->Strings[identifier] = stringData;
		}
		else
		{
			stringData = iterFind->second;
		}

		mIdentifiers.insert(identifier);
		stringData->UpdateString(value);
	}

	String StringTable::GetString(const String& identifier, Language language)
	{
		LanguageData* curLanguage = &(mAllLanguages[(UINT32)language]);

		auto iterFind = curLanguage->Strings.find(identifier);
		if (iterFind != curLanguage->Strings.end())
			return iterFind->second->String;
			
		return identifier;
	}

	void StringTable::RemoveString(const String& identifier)
	{
		for(UINT32 i = 0; i < (UINT32)Language::Count; i++)
		{
			mAllLanguages[i].Strings.erase(identifier);
		}

		mIdentifiers.erase(identifier);
	}

	SPtr<LocalizedStringData> StringTable::GetStringData(const String& identifier, bool insertIfNonExisting)
	{
		return GetStringData(identifier, mActiveLanguage, insertIfNonExisting);
	}

	SPtr<LocalizedStringData> StringTable::GetStringData(const String& identifier, Language language, bool insertIfNonExisting)
	{
		LanguageData* curLanguage = &(mAllLanguages[(UINT32)language]);

		auto iterFind = curLanguage->Strings.find(identifier);
		if(iterFind != curLanguage->Strings.end())
			return iterFind->second;

		auto defaultIterFind = mDefaultLanguageData->Strings.find(identifier);
		if(defaultIterFind != mDefaultLanguageData->Strings.end())
			return defaultIterFind->second;

		if(insertIfNonExisting)
		{
			SetString(identifier, DEFAULT_LANGUAGE, identifier);

			auto defaultIterFind = mDefaultLanguageData->Strings.find(identifier);
			if(defaultIterFind != mDefaultLanguageData->Strings.end())
				return defaultIterFind->second;
		}

		BS_EXCEPT(InvalidParametersException, "There is no string data for the provided identifier.");
		return nullptr;
	}

	HStringTable StringTable::Create()
	{
		return static_resource_cast<StringTable>(gResources().CreateResourceHandleInternal(CreatePtrInternal()));
	}

	SPtr<StringTable> StringTable::CreatePtrInternal()
	{
		SPtr<StringTable> scriptCodePtr = bs_core_ptr<StringTable>(
			new (bs_alloc<StringTable>()) StringTable());
		scriptCodePtr->SetThisPtrInternal(scriptCodePtr);
		scriptCodePtr->Initialize();

		return scriptCodePtr;
	}

	RTTITypeBase* StringTable::GetRttiStatic()
	{
		return StringTableRTTI::Instance();
	}

	RTTITypeBase* StringTable::GetRtti() const
	{
		return StringTable::GetRttiStatic();
	}
}
