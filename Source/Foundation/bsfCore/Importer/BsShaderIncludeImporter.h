//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Importer/BsSpecificImporter.h"

namespace bs
{
	/** @addtogroup Importer
	 *  @{
	 */

	/**
	 * Importer using for importing GPU program (shader) include files. Include files are just text files ending with
	 * ".bslinc" extension.
	 */
	class BS_CORE_EXPORT ShaderIncludeImporter : public SpecificImporter
	{
	public:
		/** @copydoc SpecificImporter::isExtensionSupported */
		bool IsExtensionSupported(const String& ext) const ;

		/** @copydoc SpecificImporter::isMagicNumberSupported */
		bool IsMagicNumberSupported(const UINT8* magicNumPtr, UINT32 numBytes) const ;

		/** @copydoc SpecificImporter::import */
		SPtr<Resource> Import(const Path& filePath, SPtr<const ImportOptions> importOptions) ;
	};

	/** @} */
}
