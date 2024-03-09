//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Resources/BsResourceManifest.h"
#include "Private/RTTI/BsResourceManifestRTTI.h"
#include "Serialization/BsFileSerializer.h"
#include "Error/BsException.h"

using namespace bs;

ResourceManifest::ResourceManifest(const ConstructPrivately& dummy)
{
}

ResourceManifest::ResourceManifest(const String& name)
	: mName(name)
{
}

SPtr<ResourceManifest> ResourceManifest::Create(const String& name)
{
	return B3DMakeShared<ResourceManifest>(name);
}

SPtr<ResourceManifest> ResourceManifest::CreateEmpty()
{
	return B3DMakeShared<ResourceManifest>(ConstructPrivately());
}

void ResourceManifest::RegisterResource(const UUID& uuid, const Path& filePath)
{
	auto iterFind = mUUIDToFilePath.find(uuid);

	if(iterFind != mUUIDToFilePath.end())
	{
		if(iterFind->second != filePath)
		{
			mFilePathToUUID.erase(iterFind->second);

			mUUIDToFilePath[uuid] = filePath;
			mFilePathToUUID[filePath] = uuid;
		}
	}
	else
	{
		auto iterFind2 = mFilePathToUUID.find(filePath);
		if(iterFind2 != mFilePathToUUID.end())
			mUUIDToFilePath.erase(iterFind2->second);

		mUUIDToFilePath[uuid] = filePath;
		mFilePathToUUID[filePath] = uuid;
	}
}

void ResourceManifest::UnregisterResource(const UUID& uuid)
{
	if(auto foundPath = mUUIDToFilePath.find(uuid); foundPath != mUUIDToFilePath.end())
	{
		mFilePathToUUID.erase(foundPath->second);
		mUUIDToFilePath.erase(uuid);
	}

	if(auto foundVirtualPath = mUUIDToVirtualFilePath.find(uuid); foundVirtualPath != mUUIDToVirtualFilePath.end())
	{
		mVirtualFilePathToUUID.erase(foundVirtualPath->second);
		mUUIDToVirtualFilePath.erase(uuid);
	}
}

bool ResourceManifest::UUIDToPhysicalFilePath(const UUID& uuid, Path& filePath) const
{
	if(auto foundPath = mUUIDToFilePath.find(uuid); foundPath != mUUIDToFilePath.end())
	{
		filePath = foundPath->second;
		return true;
	}
	else
	{
		filePath = Path::kBlank;
		return false;
	}
}

bool ResourceManifest::PhysicalFilePathToUUID(const Path& filePath, UUID& outUUID) const
{
	if(auto foundPath = mFilePathToUUID.find(filePath); foundPath != mFilePathToUUID.end())
	{
		outUUID = foundPath->second;
		return true;
	}

	outUUID = UUID::kEmpty;
	return false;
}

bool ResourceManifest::VirtualFilePathToUUID(const Path& filePath, UUID& outUUID) const
{
	if(auto foundVirtualPath = mVirtualFilePathToUUID.find(filePath); foundVirtualPath != mVirtualFilePathToUUID.end())
	{
		outUUID = foundVirtualPath->second;
		return true;
	}

	outUUID = UUID::kEmpty;
	return false;
}

bool ResourceManifest::UuidExists(const UUID& uuid) const
{
	auto iterFind = mUUIDToFilePath.find(uuid);

	return iterFind != mUUIDToFilePath.end();
}

bool ResourceManifest::FilePathExists(const Path& filePath) const
{
	if(mFilePathToUUID.find(filePath) != mFilePathToUUID.end())
		return true;

	return mVirtualFilePathToUUID.find(filePath) != mVirtualFilePathToUUID.end();
}

bool ResourceManifest::VirtualToPhysicalPath(const Path& virtualPath, Path& outPhysicalPath) const
{
	if(!mVirtualPathPrefix.Includes(virtualPath))
		return false;

	outPhysicalPath = virtualPath;
	outPhysicalPath.MakeRelative(mVirtualPathPrefix);
	outPhysicalPath.MakeAbsolute(mPhysicalPathPrefix);
	
	return true;
}

void ResourceManifest::Save(const SPtr<ResourceManifest>& manifest, const Path& path, const Path& physicalPathPrefix)
{
	if(physicalPathPrefix.IsEmpty())
	{
		FileEncoder fs(path);
		fs.Encode(manifest.get());
	}
	else
	{
		SPtr<ResourceManifest> copy = Create(manifest->mName);

		for(auto& elem : manifest->mFilePathToUUID)
		{
			if(!physicalPathPrefix.Includes(elem.first))
			{
				B3D_EXCEPT(InvalidStateException, "Path in resource manifest cannot be made relative to: \"" + physicalPathPrefix.ToString() + "\". Path: \"" + elem.first.ToString() + "\"");
			}

			Path elementRelativePath = elem.first.GetRelative(physicalPathPrefix);

			copy->mFilePathToUUID[elementRelativePath] = elem.second;
		}

		for(auto& elem : manifest->mUUIDToFilePath)
		{
			if(!physicalPathPrefix.Includes(elem.second))
			{
				B3D_EXCEPT(InvalidStateException, "Path in resource manifest cannot be made relative to: \"" + physicalPathPrefix.ToString() + "\". Path: \"" + elem.second.ToString() + "\"");
			}

			Path elementRelativePath = elem.second.GetRelative(physicalPathPrefix);

			copy->mUUIDToFilePath[elem.first] = elementRelativePath;
		}

		FileEncoder fs(path);
		fs.Encode(copy.get());
	}
}

SPtr<ResourceManifest> ResourceManifest::Load(const Path& path, const Path& physicalPathPrefix, const Path& virtualPathPrefix)
{
	FileDecoder fs(path);
	SPtr<ResourceManifest> manifest = std::static_pointer_cast<ResourceManifest>(fs.Decode());

	if(physicalPathPrefix.IsEmpty() && virtualPathPrefix.IsEmpty())
		return manifest;

	SPtr<ResourceManifest> copy = Create(manifest->mName);
	copy->mPhysicalPathPrefix = physicalPathPrefix;
	copy->mVirtualPathPrefix = virtualPathPrefix;

	if(!physicalPathPrefix.IsEmpty())
	{
		for(auto& elem : manifest->mFilePathToUUID)
		{
			Path absPath = elem.first.GetAbsolute(physicalPathPrefix);
			copy->mFilePathToUUID[absPath] = elem.second;
		}

		for(auto& elem : manifest->mUUIDToFilePath)
		{
			Path absPath = elem.second.GetAbsolute(physicalPathPrefix);
			copy->mUUIDToFilePath[elem.first] = absPath;
		}
	}

	if(!virtualPathPrefix.IsEmpty())
	{
		for(const auto& pair : manifest->mFilePathToUUID)
		{
			const Path absoluteVirtualPath = pair.first.GetAbsolute(virtualPathPrefix);
			copy->mVirtualFilePathToUUID[absoluteVirtualPath] = pair.second;
		}

		for(const auto& pair : manifest->mUUIDToFilePath)
		{
			const Path absoluteVirtualPath = pair.second.GetAbsolute(virtualPathPrefix);
			copy->mUUIDToVirtualFilePath[pair.first] = absoluteVirtualPath;
		}
	}

	return copy;
}

RTTITypeBase* ResourceManifest::GetRttiStatic()
{
	return ResourceManifestRTTI::Instance();
}

RTTITypeBase* ResourceManifest::GetRtti() const
{
	return ResourceManifest::GetRttiStatic();
}
