//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Text/BsTextData.h"
#include "Text/BsFont.h"
#include "Math/BsVector2.h"
#include "Debug/BsDebug.h"

using namespace bs;

const int kSpaceChar = 32;
const int kTabChar = 9;

void TextDataBase::TextWord::Initialize(bool isSpacer)
{
	mWidth = 0.0f;
	mHeight = 0.0f;
	mIsSpacer = isSpacer;
	mSpaceWidth = 0.0f;
	mCharacterStartIndex = 0;
	mCharacterEndIndex = 0;
	mLastCharacter = nullptr;
}

// Assumes characterIndex is an index right after last char in the list (if any). All chars need to be sequential.
float TextDataBase::TextWord::AddCharacter(u32 characterIndex, const CharacterInformation& characterInformation)
{
	const float charWidth = CalculateCharacterWidth(mLastCharacter, characterInformation);

	mWidth += charWidth;
	mHeight = std::max(mHeight, characterInformation.Height);

	if(mLastCharacter == nullptr) // First char
		mCharacterStartIndex = mCharacterEndIndex = characterIndex;
	else
		mCharacterEndIndex = characterIndex;

	mLastCharacter = &characterInformation;

	return charWidth;
}

float TextDataBase::TextWord::CalculateWidthWithCharacter(const CharacterInformation& characterInformation) const
{
	return mWidth + CalculateCharacterWidth(mLastCharacter, characterInformation);
}

float TextDataBase::TextWord::CalculateCharacterWidth(const CharacterInformation* previousCharacter, const CharacterInformation& currentCharacter)
{
	float characterWidth = currentCharacter.XAdvance;
	if(previousCharacter != nullptr)
	{
		float kerning = 0.0f;
		for(size_t j = 0; j < previousCharacter->KerningPairs.size(); j++)
		{
			if(previousCharacter->KerningPairs[j].OtherCharId == currentCharacter.CharId)
			{
				kerning = previousCharacter->KerningPairs[j].Amount;
				break;
			}
		}

		characterWidth += kerning;
	}

	return characterWidth;
}

void TextDataBase::TextWord::AddSpace(float spaceWidth)
{
	mSpaceWidth += spaceWidth;
	mWidth = mSpaceWidth;
	mHeight = 0;
}

void TextDataBase::TextLine::Initialize(TextDataBase* textData)
{
	mWidth = 0.0f;
	mHeight = 0.0f;
	mIsEmpty = true;
	mTextData = textData;
	mWordStartIndex = 0;
	mWordEndIndex = 0;
}

void TextDataBase::TextLine::Finalize(bool hasNewlineChar)
{
	mHasNewline = hasNewlineChar;
}

void TextDataBase::TextLine::Add(u32 characterIndex, const CharacterInformation& characterInformation)
{
	float characterWidth = 0.0f;
	if(mIsEmpty)
	{
		mWordStartIndex = mWordEndIndex = MemBuffer->AllocWord(false);
		mIsEmpty = false;
	}
	else
	{
		if(MemBuffer->WordBuffer[mWordEndIndex].IsSpacer())
			mWordEndIndex = MemBuffer->AllocWord(false);
	}

	TextWord& lastWord = MemBuffer->WordBuffer[mWordEndIndex];
	characterWidth = lastWord.AddCharacter(characterIndex, characterInformation);

	mWidth += characterWidth;
	mHeight = std::max(mHeight, lastWord.GetHeight());
}

void TextDataBase::TextLine::AddSpace(float spaceWidth)
{
	if(mIsEmpty)
	{
		mWordStartIndex = mWordEndIndex = MemBuffer->AllocWord(true);
		mIsEmpty = false;
	}
	else
		mWordEndIndex = MemBuffer->AllocWord(true); // Each space is counted as its own word, to make certain operations easier

	TextWord& lastWord = MemBuffer->WordBuffer[mWordEndIndex];
	lastWord.AddSpace(spaceWidth);

	mWidth += spaceWidth;
}

// Assumes wordIndex is an index right after last word in the list (if any). All words need to be sequential.
void TextDataBase::TextLine::AddWord(u32 wordIndex, const TextWord& word)
{
	if(mIsEmpty)
	{
		mWordStartIndex = mWordEndIndex = wordIndex;
		mIsEmpty = false;
	}
	else
		mWordEndIndex = wordIndex;

	mWidth += word.GetWidth();
	mHeight = std::max(mHeight, word.GetHeight());
}

u32 TextDataBase::TextLine::RemoveLastWord()
{
	if(mIsEmpty)
	{
		B3D_ASSERT(false);
		return 0;
	}

	u32 lastWord = mWordEndIndex--;
	if(mWordStartIndex == lastWord)
	{
		mIsEmpty = true;
		mWordStartIndex = mWordEndIndex = 0;
	}

	CalculateBounds();

	return lastWord;
}

float TextDataBase::TextLine::CalculateWidthWithChararacter(const CharacterInformation& characterInformation) const
{
	float characterWidth = 0.0f;

	if(!mIsEmpty)
	{
		TextWord& lastWord = MemBuffer->WordBuffer[mWordEndIndex];
		if(lastWord.IsSpacer())
			characterWidth = TextWord::CalculateCharacterWidth(nullptr, characterInformation);
		else
			characterWidth = lastWord.CalculateWidthWithCharacter(characterInformation) - lastWord.GetWidth();
	}
	else
	{
		characterWidth = TextWord::CalculateCharacterWidth(nullptr, characterInformation);
	}

	return mWidth + characterWidth;
}

bool TextDataBase::TextLine::IsAtWordBoundary() const
{
	return mIsEmpty || MemBuffer->WordBuffer[mWordEndIndex].IsSpacer();
}

u32 TextDataBase::TextLine::FillBuffer(u32 page, Vector2* vertices, Vector2* uvs, u32* indexes, u32 offset, u32 size) const
{
	u32 quadCount = 0;

	if(mIsEmpty)
		return quadCount;

	float penX = 0;
	for(u32 wordIndex = mWordStartIndex; wordIndex <= mWordEndIndex; wordIndex++)
	{
		const TextWord& word = mTextData->GetWord(wordIndex);

		if(word.IsSpacer())
		{
			// We store invisible space quads in the first page. Even though they aren't needed
			// for rendering and we could just leave an empty space, they are needed for intersection tests
			// for things like determining caret placement and selection areas
			if(page == 0)
			{
				float curX = penX;
				float curY = 0.0f;

				u32 curVert = offset * 4;
				u32 curIndex = offset * 6;

				vertices[curVert + 0] = Vector2(curX, curY);
				vertices[curVert + 1] = Vector2((curX + word.GetWidth()), curY);
				vertices[curVert + 2] = Vector2(curX, curY + mTextData->GetLineHeight());
				vertices[curVert + 3] = Vector2((curX + word.GetWidth()), curY + mTextData->GetLineHeight());

				if(uvs != nullptr)
				{
					uvs[curVert + 0] = Vector2(0.0f, 0.0f);
					uvs[curVert + 1] = Vector2(0.0f, 0.0f);
					uvs[curVert + 2] = Vector2(0.0f, 0.0f);
					uvs[curVert + 3] = Vector2(0.0f, 0.0f);
				}

				// Triangles are back-facing which makes them invisible
				if(indexes != nullptr)
				{
					indexes[curIndex + 0] = curVert + 0;
					indexes[curIndex + 1] = curVert + 2;
					indexes[curIndex + 2] = curVert + 1;
					indexes[curIndex + 3] = curVert + 1;
					indexes[curIndex + 4] = curVert + 2;
					indexes[curIndex + 5] = curVert + 3;
				}

				offset++;
				quadCount++;

				if(offset > size)
					B3D_EXCEPT(InternalErrorException, "Out of buffer bounds. Buffer size: " + ToString(size));
			}

			penX += word.GetWidth();
		}
		else
		{
			float kerning = 0.0f;
			for(u32 characterIndex = word.GetStartCharacterIndex(); characterIndex <= word.GetEndCharacterIndex(); characterIndex++)
			{
				const CharacterInformation& currentCharacterInformation = mTextData->GetCharacter(characterIndex);

				float curX = penX + currentCharacterInformation.XOffset;
				float curY = mTextData->GetBaselineOffset() - currentCharacterInformation.YOffset;

				penX += currentCharacterInformation.XAdvance + kerning;

				kerning = 0.0f;
				if((characterIndex + 1) <= word.GetEndCharacterIndex())
				{
					const CharacterInformation& nextChar = mTextData->GetCharacter(characterIndex + 1);
					for(size_t j = 0; j < currentCharacterInformation.KerningPairs.size(); j++)
					{
						if(currentCharacterInformation.KerningPairs[j].OtherCharId == nextChar.CharId)
						{
							kerning = currentCharacterInformation.KerningPairs[j].Amount;
							break;
						}
					}
				}

				if(currentCharacterInformation.Page != page)
					continue;

				u32 curVert = offset * 4;
				u32 curIndex = offset * 6;

				vertices[curVert + 0] = Vector2(curX, curY);
				vertices[curVert + 1] = Vector2((curX + currentCharacterInformation.Width), curY);
				vertices[curVert + 2] = Vector2(curX, curY + currentCharacterInformation.Height);
				vertices[curVert + 3] = Vector2(curX + currentCharacterInformation.Width, curY + currentCharacterInformation.Height);

				if(uvs != nullptr)
				{
					uvs[curVert + 0] = Vector2(currentCharacterInformation.UvX, currentCharacterInformation.UvY);
					uvs[curVert + 1] = Vector2(currentCharacterInformation.UvX + currentCharacterInformation.UvWidth, currentCharacterInformation.UvY);
					uvs[curVert + 2] = Vector2(currentCharacterInformation.UvX, currentCharacterInformation.UvY + currentCharacterInformation.UvHeight);
					uvs[curVert + 3] = Vector2(currentCharacterInformation.UvX + currentCharacterInformation.UvWidth, currentCharacterInformation.UvY + currentCharacterInformation.UvHeight);
				}

				if(indexes != nullptr)
				{
					indexes[curIndex + 0] = curVert + 0;
					indexes[curIndex + 1] = curVert + 1;
					indexes[curIndex + 2] = curVert + 2;
					indexes[curIndex + 3] = curVert + 1;
					indexes[curIndex + 4] = curVert + 3;
					indexes[curIndex + 5] = curVert + 2;
				}

				offset++;
				quadCount++;

				if(offset > size)
					B3D_EXCEPT(InternalErrorException, "Out of buffer bounds. Buffer size: " + ToString(size));
			}
		}
	}

	return quadCount;
}

u32 TextDataBase::TextLine::GetCharacterCount() const
{
	if(mIsEmpty)
		return 0;

	u32 characterCount = 0;
	for(u32 i = mWordStartIndex; i <= mWordEndIndex; i++)
	{
		TextWord& word = MemBuffer->WordBuffer[i];

		if(word.IsSpacer())
			characterCount++;
		else
			characterCount += word.GetCharacterCount();
	}

	return characterCount;
}

void TextDataBase::TextLine::CalculateBounds()
{
	mWidth = 0;
	mHeight = 0;

	if(mIsEmpty)
		return;

	for(u32 wordIndex = mWordStartIndex; wordIndex <= mWordEndIndex; wordIndex++)
	{
		TextWord& word = MemBuffer->WordBuffer[wordIndex];

		mWidth += word.GetWidth();
		mHeight = std::max(mHeight, word.GetHeight());
	}
}

TextDataBase::TextDataBase(const U32String& text, const HFont& font, u32 fontSize, u32 width, u32 height, bool wordWrap, bool wordBreak)
	: mChars(nullptr), mNumChars(0), mWords(nullptr), mNumWords(0), mLines(nullptr), mLineCount(0), mPageInfos(nullptr), mPageCount(0), mFont(font), mFontBitmapInformation(nullptr)
{
	// In order to reduce number of memory allocations algorithm first calculates data into temporary buffers and then copies the results
	InitAlloc();

	if(font != nullptr)
	{
		font->RenderGlyphs(fontSize, TArrayView((u32*)text.data(), text.size()));
		mFontBitmapInformation = font->GetBitmap(fontSize);
	}

	if(mFontBitmapInformation == nullptr)
		return;

	bool widthIsLimited = width > 0;
	mFont = font;

	u32 curLineIdx = MemBuffer->AllocLine(this);
	float curHeight = mFontBitmapInformation->LineHeight;
	u32 charIdx = 0;

	while(true)
	{
		if(charIdx >= text.size())
			break;

		u32 charId = text[charIdx];
		const CharacterInformation& charDesc = mFontBitmapInformation->GetCharacterInformation(charId);

		TextLine* curLine = &MemBuffer->LineBuffer[curLineIdx];

		if(text[charIdx] == '\n' || text[charIdx] == '\r')
		{
			curLine->Finalize(true);

			curLineIdx = MemBuffer->AllocLine(this);
			curLine = &MemBuffer->LineBuffer[curLineIdx];

			curHeight += mFontBitmapInformation->LineHeight;

			charIdx++;

			// Check for \r\n
			if(text[charIdx - 1] == '\r' && charIdx < text.size())
			{
				if(text[charIdx] == '\n')
					charIdx++;
			}

			continue;
		}

		if(widthIsLimited && wordWrap)
		{
			float widthWithChar = 0.0f;
			if(charIdx == kSpaceChar)
				widthWithChar = curLine->GetWidth() + GetSpaceWidth();
			else if(charIdx == kTabChar)
				widthWithChar = curLine->GetWidth() + GetSpaceWidth() * 4;
			else
				widthWithChar = curLine->CalculateWidthWithChararacter(charDesc);

			if(widthWithChar > (float)width && !curLine->IsEmpty())
			{
				bool atWordBoundary = charId == kSpaceChar || charId == kTabChar || curLine->IsAtWordBoundary();

				if(!atWordBoundary) // Need to break word into multiple pieces, or move it to next line
				{
					u32 lastWordIdx = curLine->RemoveLastWord();
					TextWord& lastWord = MemBuffer->WordBuffer[lastWordIdx];

					bool wordFits = lastWord.CalculateWidthWithCharacter(charDesc) <= (float)width;
					if(wordFits && !curLine->IsEmpty())
					{
						curLine->Finalize(false);

						curLineIdx = MemBuffer->AllocLine(this);
						curLine = &MemBuffer->LineBuffer[curLineIdx];

						curHeight += mFontBitmapInformation->LineHeight;

						curLine->AddWord(lastWordIdx, lastWord);
					}
					else
					{
						if(wordBreak)
						{
							curLine->AddWord(lastWordIdx, lastWord);
							curLine->Finalize(false);

							curLineIdx = MemBuffer->AllocLine(this);
							curLine = &MemBuffer->LineBuffer[curLineIdx];

							curHeight += mFontBitmapInformation->LineHeight;
						}
						else
						{
							if(!curLine->IsEmpty()) // Add new line unless current line is empty (to avoid constantly moving the word to new lines)
							{
								curLine->Finalize(false);

								curLineIdx = MemBuffer->AllocLine(this);
								curLine = &MemBuffer->LineBuffer[curLineIdx];

								curHeight += mFontBitmapInformation->LineHeight;
							}

							curLine->AddWord(lastWordIdx, lastWord);
						}
					}
				}
				else if(charId != kSpaceChar && charId != kTabChar) // If current char is whitespace add it to the existing line even if it doesn't fit
				{
					curLine->Finalize(false);

					curLineIdx = MemBuffer->AllocLine(this);
					curLine = &MemBuffer->LineBuffer[curLineIdx];

					curHeight += mFontBitmapInformation->LineHeight;
				}
			}
		}

		if(charId == kSpaceChar)
		{
			curLine->AddSpace(GetSpaceWidth());
			MemBuffer->AddCharToPage(0, *mFontBitmapInformation);
		}
		else if(charId == kTabChar)
		{
			curLine->AddSpace(GetSpaceWidth() * 4);
			MemBuffer->AddCharToPage(0, *mFontBitmapInformation);
		}
		else
		{
			curLine->Add(charIdx, charDesc);
			MemBuffer->AddCharToPage(charDesc.Page, *mFontBitmapInformation);
		}

		charIdx++;
	}

	MemBuffer->LineBuffer[curLineIdx].Finalize(true);

	// Now that we have all the data we need, allocate the permanent buffers and copy the data
	mNumChars = (u32)text.size();
	mNumWords = MemBuffer->NextFreeWord;
	mLineCount = MemBuffer->NextFreeLine;
	mPageCount = MemBuffer->NextFreePageInfo;
}

void TextDataBase::GeneratePersistentData(const U32String& text, u8* buffer, u32& size, bool freeTemporary)
{
	u32 charArraySize = mNumChars * sizeof(const CharacterInformation*);
	u32 wordArraySize = mNumWords * sizeof(TextWord);
	u32 lineArraySize = mLineCount * sizeof(TextLine);
	u32 pageInfoArraySize = mPageCount * sizeof(PageInfo);

	if(buffer == nullptr)
	{
		size = charArraySize + wordArraySize + lineArraySize + pageInfoArraySize;
		;
		return;
	}

	u8* dataPtr = (u8*)buffer;
	mChars = (const CharacterInformation**)dataPtr;

	for(u32 i = 0; i < mNumChars; i++)
	{
		u32 charId = text[i];
		const CharacterInformation& charDesc = mFontBitmapInformation->GetCharacterInformation(charId);

		mChars[i] = &charDesc;
	}

	dataPtr += charArraySize;
	mWords = (TextWord*)dataPtr;
	memcpy(mWords, &MemBuffer->WordBuffer[0], wordArraySize);

	dataPtr += wordArraySize;
	mLines = (TextLine*)dataPtr;
	memcpy(mLines, &MemBuffer->LineBuffer[0], lineArraySize);

	dataPtr += lineArraySize;
	mPageInfos = (PageInfo*)dataPtr;
	memcpy((void*)mPageInfos, (void*)&MemBuffer->PageBuffer[0], pageInfoArraySize);

	if(freeTemporary)
		MemBuffer->DeallocAll();
}

const HTexture& TextDataBase::GetTextureForPage(u32 page) const
{
	return mFontBitmapInformation->TexturePages[page].Texture;
}

float TextDataBase::GetBaselineOffset() const
{
	return mFontBitmapInformation->BaselineOffset;
}

float TextDataBase::GetLineHeight() const
{
	return mFontBitmapInformation->LineHeight;
}

float TextDataBase::GetSpaceWidth() const
{
	return mFontBitmapInformation->SpaceWidth;
}

void TextDataBase::InitAlloc()
{
	if(MemBuffer == nullptr)
		MemBuffer = B3DNew<BufferData>();
}

B3D_THREADLOCAL TextDataBase::BufferData* TextDataBase::MemBuffer = nullptr;

TextDataBase::BufferData::BufferData()
{
	WordBufferSize = 2000;
	LineBufferSize = 500;
	PageBufferSize = 20;

	NextFreeWord = 0;
	NextFreeLine = 0;
	NextFreePageInfo = 0;

	WordBuffer = B3DNewMultiple<TextWord>(WordBufferSize);
	LineBuffer = B3DNewMultiple<TextLine>(LineBufferSize);
	PageBuffer = B3DNewMultiple<PageInfo>(PageBufferSize);
}

TextDataBase::BufferData::~BufferData()
{
	B3DDeleteMultiple(WordBuffer, WordBufferSize);
	B3DDeleteMultiple(LineBuffer, LineBufferSize);
	B3DDeleteMultiple(PageBuffer, PageBufferSize);
}

u32 TextDataBase::BufferData::AllocWord(bool spacer)
{
	if(NextFreeWord >= WordBufferSize)
	{
		u32 newBufferSize = WordBufferSize * 2;
		TextWord* newBuffer = B3DNewMultiple<TextWord>(newBufferSize);
		memcpy(WordBuffer, newBuffer, WordBufferSize);

		B3DDeleteMultiple(WordBuffer, WordBufferSize);
		WordBuffer = newBuffer;
		WordBufferSize = newBufferSize;
	}

	WordBuffer[NextFreeWord].Initialize(spacer);

	return NextFreeWord++;
}

u32 TextDataBase::BufferData::AllocLine(TextDataBase* textData)
{
	if(NextFreeLine >= LineBufferSize)
	{
		u32 newBufferSize = LineBufferSize * 2;
		TextLine* newBuffer = B3DNewMultiple<TextLine>(newBufferSize);
		memcpy(LineBuffer, newBuffer, LineBufferSize);

		B3DDeleteMultiple(LineBuffer, LineBufferSize);
		LineBuffer = newBuffer;
		LineBufferSize = newBufferSize;
	}

	LineBuffer[NextFreeLine].Initialize(textData);

	return NextFreeLine++;
}

void TextDataBase::BufferData::DeallocAll()
{
	NextFreeWord = 0;
	NextFreeLine = 0;
	NextFreePageInfo = 0;
}

void TextDataBase::BufferData::AddCharToPage(u32 page, const FontBitmapInformation& fontData)
{
	if(NextFreePageInfo >= PageBufferSize)
	{
		u32 newBufferSize = PageBufferSize * 2;
		PageInfo* newBuffer = B3DNewMultiple<PageInfo>(newBufferSize);
		memcpy((void*)PageBuffer, (void*)newBuffer, PageBufferSize);

		B3DDeleteMultiple(PageBuffer, PageBufferSize);
		PageBuffer = newBuffer;
		PageBufferSize = newBufferSize;
	}

	while(page >= NextFreePageInfo)
	{
		PageBuffer[NextFreePageInfo].QuadCount = 0;

		NextFreePageInfo++;
	}

	PageBuffer[page].QuadCount++;
}

float TextDataBase::GetWidth() const
{
	float width = 0.0f;

	for(u32 lineIndex = 0; lineIndex < mLineCount; lineIndex++)
		width = std::max(width, mLines[lineIndex].GetWidth());

	return width;
}

float TextDataBase::GetHeight() const
{
	float height = 0.0f;

	for(u32 lineIndex = 0; lineIndex < mLineCount; lineIndex++)
		height += mLines[lineIndex].GetHeight();

	return height;
}
