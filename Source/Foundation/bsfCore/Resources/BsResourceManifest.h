//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "BsCorePrerequisites.h"
#include "Reflection/BsIReflectable.h"

namespace bs
{
	/** @addtogroup Resources-Internal
	 *  @{
	 */

	/**
	 * Serializable class that contains UUID <-> file path mapping for resources.
	 *
	 * @note
	 * This class allows you to reference resources between sessions. At the end of a session save the resource manifest,
	 * and then restore it at the start of a new session. This way ensures that resource UUIDs stay consistent and anything
	 * referencing them can find the resources.
	 * @note
	 * Thread safe.
	 */
	class B3D_CORE_EXPORT B3D_SCRIPT_EXPORT(DocumentationGroup(Resources), API(Framework)) ResourceManifest : public IReflectable
	{
		struct ConstructPrivately
		{};

	public:
		explicit ResourceManifest(const ConstructPrivately& dummy);
		ResourceManifest(const String& name);

		/**	Returns an unique name of the resource manifest. */
		B3D_SCRIPT_EXPORT(Property(Getter), ExportName(Name))

		const String& GetName() const { return mName; }

		/**	Registers a new resource in the manifest. */
		B3D_SCRIPT_EXPORT()
		void RegisterResource(const UUID& uuid, const Path& filePath);

		/**	Removes a resource from the manifest. */
		B3D_SCRIPT_EXPORT()
		void UnregisterResource(const UUID& uuid);

		/**
		 * Attempts to find a resource with the provided UUID and outputs the absolute physical path to the resource if found. Returns true
		 * if UUID was found, false otherwise.
		 */
		B3D_SCRIPT_EXPORT()
		bool UUIDToPhysicalFilePath(const UUID& uuid, Path& filePath) const;

		/**
		 * Attempts to find a resource with the provided physical path and outputs the UUID to the resource if found. Returns true
		 * if path was found, false otherwise.
		 */
		B3D_SCRIPT_EXPORT()
		bool PhysicalFilePathToUUID(const Path& filePath, UUID& outUUID) const;

		/**
		 * Attempts to find a resource with the provided virtual path and outputs the UUID to the resource if found. Returns true
		 * if path was found, false otherwise.
		 */
		B3D_SCRIPT_EXPORT()
		bool VirtualFilePathToUUID(const Path& filePath, UUID& outUUID) const;

		/**	Checks if provided UUID exists in the manifest. */
		B3D_SCRIPT_EXPORT()
		bool UuidExists(const UUID& uuid) const;

		/**	Checks if the provided path exists in the manifest. */
		B3D_SCRIPT_EXPORT()
		bool FilePathExists(const Path& filePath) const;

		/**
		 * Converts a virtual path to a physical one. Note the existance of the path in the manifest will not be checked,
		 * the only requirement is that the virtual path begins with virtual path prefix. This means you can use this method
		 * for translating virtual -> physical paths before they are added to the manifest. Returns false if
		 * @p virtualPath doesn't start with the manifest's virtual path prefix.
		 */
		B3D_SCRIPT_EXPORT()
		bool VirtualToPhysicalPath(const Path& virtualPath, Path& outPhysicalPath) const;

		/**
		 * Saves the resource manifest to the specified location.
		 *
		 * @param	manifest			Manifest to save.
		 * @param	path				Full pathname of the file to save the manifest in.
		 * @param	physicalPathPrefix	If not empty, all pathnames in the manifest will be stored as if relative to this path.
		 */
		B3D_SCRIPT_EXPORT()
		static void Save(const SPtr<ResourceManifest>& manifest, const Path& path, const Path& physicalPathPrefix);

		/**
		 * Loads the resource manifest from the specified location.
		 *
		 * @param	path					Full pathname of the file to load the manifest from.
		 * @param	physicalPathPrefix		If not empty, all loaded path names will have this path prepended.
		 * @param	virtualPathPrefix		If not empty, adds an additional set of paths that the resource can be referenced from. 
		 */
		B3D_SCRIPT_EXPORT()
		static SPtr<ResourceManifest> Load(const Path& path, const Path& physicalPathPrefix, const Path& virtualPathPrefix = Path::kBlank);

		/** Creates a new empty resource manifest. Provided name should be unique among manifests. */
		B3D_SCRIPT_EXPORT(ExtensionConstructorForType(ResourceManifest))
		static SPtr<ResourceManifest> Create(const String& name);

	private:
		String mName;
		Path mPhysicalPathPrefix;
		Path mVirtualPathPrefix;

		UnorderedMap<UUID, Path> mUUIDToFilePath;
		UnorderedMap<Path, UUID> mFilePathToUUID;

		UnorderedMap<UUID, Path> mUUIDToVirtualFilePath;
		UnorderedMap<Path, UUID> mVirtualFilePathToUUID;

		/************************************************************************/
		/* 								RTTI		                     		*/
		/************************************************************************/

		/**	Creates a new empty resource manifest. */
		static SPtr<ResourceManifest> CreateEmpty();

	public:
		friend class ResourceManifestRTTI;
		static RTTIType* GetRttiStatic();
		RTTIType* GetRtti() const override;
	};

	/** @} */
} // namespace bs
