//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsOAPrerequisites.h"
#include "BsAudioDecoder.h"
#include "FLAC/stream_decoder.h"

namespace bs
{
	/** @addtogroup OpenAudio
	 *  @{
	 */

	/** Data used by the FLAC decoder. */
	struct FLACDecoderData
	{
		SPtr<DataStream> Stream;
		UINT32 StreamOffset = 0;
		AudioDataInfo Info;
		UINT8* Output = nullptr;
		Vector<UINT8> Overflow;
		UINT32 SamplesToRead = 0;
		bool Error = false;
	};

	/** Decodes FLAC audio data into raw PCM samples. */
	class FLACDecoder : public AudioDecoder
	{
	public:
		FLACDecoder() = default;
		~FLACDecoder();

		/** @copydoc AudioDecoder::open */
		bool Open(const SPtr<DataStream>& stream, AudioDataInfo& info, UINT32 offset = 0) ;

		/** @copydoc AudioDecoder::seek */
		void Seek(UINT32 offset) ;

		/** @copydoc AudioDecoder::read */
		UINT32 Read(UINT8* samples, UINT32 numSamples) ;

		/** @copydoc AudioDecoder::isValid */
		bool IsValid(const SPtr<DataStream>& stream, UINT32 offset = 0) ;
	private:
		/** Cleans up the FLAC decoder. */
		void Close();

		FLAC__StreamDecoder* mDecoder = nullptr;
		FLACDecoderData mData;
	};

	/** @} */
}
