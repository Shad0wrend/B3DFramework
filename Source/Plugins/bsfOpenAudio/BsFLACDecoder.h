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
		SPtr<DataStream> stream;
		UINT32 streamOffset = 0;
		AudioDataInfo info;
		UINT8* output = nullptr;
		Vector<UINT8> overflow;
		UINT32 samplesToRead = 0;
		bool error = false;
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
