//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Resources/BsResources.h"

#include "BsApplication.h"
#include "Resources/BsResource.h"
#include "Resources/BsResourceManifest.h"
#include "Error/BsException.h"
#include "Serialization/BsFileSerializer.h"
#include "FileSystem/BsFileSystem.h"
#include "Utility/BsUUID.h"
#include "Debug/BsDebug.h"
#include "Utility/BsUtility.h"
#include "Resources/BsSavedResourceData.h"
#include "Managers/BsResourceListenerManager.h"
#include "Utility/BsCompression.h"
#include "FileSystem/BsDataStream.h"
#include "Serialization/BsBinarySerializer.h"
#include "Reflection/BsRTTIType.h"
#include "BsCoreApplication.h"
#include "Threading/BsScheduler.h"

using namespace bs;

const ResourceLoadOptions ResourceLoadOptions::kDefault;

// TODO - Doc
static bool TryAcquirePackageLockForResourceLoad(const Path& resourcePath, const char* lockReason, UPtr<PackageReadLock>& outReadLock, UUID& outResourceId)
{
	PackageManager& packageManager = GetPackageManager();

	Optional<ResourcePackagePath> maybeResourcePackagePath = packageManager.TryResolvePhysicalResourcePath(resourcePath);
	if(!maybeResourcePackagePath.has_value()) // Maybe it's a virtual path
		maybeResourcePackagePath = packageManager.TryResolveVirtualResourcePath(resourcePath);

	if(!maybeResourcePackagePath.has_value())
		return false;

	const ResourcePackagePath& resourcePackagePath = *maybeResourcePackagePath;

	AcquirePackageReadLockOptions readLockOptions(true, true, lockReason);
	const AcquirePackageLockResult lockResult = packageManager.AcquireReadLock(resourcePackagePath.PhysicalPackagePath, readLockOptions, outReadLock);
	if(!B3D_ENSURE(lockResult == AcquirePackageLockResult::Acquired && outReadLock != nullptr))
		return false;

	const SPtr<Package>& package = outReadLock->GetPackage();
	if(!B3D_ENSURE(package != nullptr))
		return nullptr;

	const SPtr<const PackageResourceMetaData>& resourceMetaData = package->GetResourceMetaData(resourcePackagePath.ResourcePathWithinPackage);
	if(resourceMetaData == nullptr)
		return false;

	outResourceId = resourceMetaData->Id;
	return true;
}

// TODO - Doc
static bool TryAcquirePackageLockForResourceLoad(const UUID& resourceId, const char* lockReason, UPtr<PackageReadLock>& outReadLock, Path& outPackagePath)
{
	PackageManager& packageManager = GetPackageManager();

	const Optional<Path> maybePackagePath = packageManager.TryGetPackagePathForResource(resourceId);
	if(!maybePackagePath.has_value())
		return false;

	outPackagePath = *maybePackagePath;

	AcquirePackageReadLockOptions readLockOptions(true, true, lockReason);
	const AcquirePackageLockResult lockResult = packageManager.AcquireReadLock(outPackagePath, readLockOptions, outReadLock);
	if(!B3D_ENSURE(lockResult == AcquirePackageLockResult::Acquired && outReadLock != nullptr))
		return false;

	return true;
}

Resources::Resources()
{
	{
		Lock lock(mDefaultManifestMutex);
		mDefaultResourceManifest = ResourceManifest::Create("Default");
		mResourceManifests.push_back(mDefaultResourceManifest);
	}
}

Resources::~Resources()
{
	UnloadAll();
}

HResource Resources::Load(const Path& resourcePath, const ResourceLoadOptions& loadOptions)
{
	UPtr<PackageReadLock> packageReadLock;
	UUID resourceId;
	if(!TryAcquirePackageLockForResourceLoad(resourcePath, "Load resource", packageReadLock, resourceId))
	{
		B3D_LOG(Warning, Resources, "Cannot load resource. File at path '{0}' doesn't exist.", resourcePath);
		return nullptr;
	}

	return Load(std::move(packageReadLock), resourceId, loadOptions);
}

HResource Resources::Load(const UUID& resourceId, const ResourceLoadOptions& loadOptions)
{
	UPtr<PackageReadLock> packageReadLock;
	Path packagePath;
	if(!TryAcquirePackageLockForResourceLoad(resourceId, "Load resource", packageReadLock, packagePath))
	{
		B3D_LOG(Warning, Resources, "Cannot load resource. Resource with ID '{0}' doesn't exist.", resourceId);
		return nullptr;
	}

	return Load(std::move(packageReadLock), resourceId, loadOptions);
}

bool Resources::Exists(const Path& resourcePath) const
{
	UPtr<PackageReadLock> packageReadLock;
	UUID resourceId;
	return TryAcquirePackageLockForResourceLoad(resourcePath, "Check resource exists", packageReadLock, resourceId);
}

bool Resources::Exists(const UUID& resourceId) const
{
	UPtr<PackageReadLock> packageReadLock;
	Path packagePath;
	return TryAcquirePackageLockForResourceLoad(resourceId, "Check resource exists", packageReadLock, packagePath);
}

HResource Resources::Load(UPtr<PackageReadLock> packageReadLock, const UUID& resourceId, const ResourceLoadOptions& loadOptions)
{
	const SPtr<Package>& package = packageReadLock->GetPackage();
	const SPtr<const PackageResourceMetaData>& metaData = package->GetResourceMetaData(resourceId);

	if(metaData == nullptr)
	{
		B3D_LOG(Warning, Resources, "Cannot load resource. Resource with ID: {0} cannot be found in the package.", resourceId);
		return HResource();
	}

	HResource resourceHandle = GetOrCreateResourceHandle(resourceId);

	SPtr<InProgressLoadInformation> inProgressLoadInformation;
	{
		Lock lock(mLoadedResourceMutex);

		// Is the resource and (optionally) its dependencies already loaded?
		if(const auto found = mLoadedResourceInformation.find(resourceId); found != mLoadedResourceInformation.end())
		{
			LoadedResourceInformation* const loadedResourceInformation = found->second.get();
			if(!B3D_ENSURE(loadedResourceInformation != nullptr))
				return HResource();

			if(!loadOptions.LoadDependencies || loadedResourceInformation->DependenciesLoaded)
			{
				if(loadOptions.KeepInternalReference)
				{
					loadedResourceInformation->InternalReferenceCount++;
					loadedResourceInformation->ResourceHandle.IncrementInternalReferenceCount();
				}

				return loadedResourceInformation->ResourceHandle.Lock();
			}
		}

		// If not already loaded, create structure to track in progress load for the resource and all dependencies
		inProgressLoadInformation = B3DMakeShared<InProgressLoadInformation>();
		inProgressLoadInformation->PackageReadLock = std::move(packageReadLock);
		inProgressLoadInformation->ResourceHandle = resourceHandle;
		inProgressLoadInformation->LoadOptions = loadOptions;
		inProgressLoadInformation->RemainingResourcesToLoadCount = 1;

		if(loadOptions.LoadDependencies)
		{
			const u32 dependencyCount = (u32)metaData->Dependencies.size();
			inProgressLoadInformation->DependencyResourceHandles.reserve(dependencyCount);
			
			for(const UUID& dependencyId : metaData->Dependencies)
			{
				if(dependencyId == resourceId)
					continue;

				HResource dependencyResourceHandle = GetOrCreateResourceHandle(dependencyId);
				inProgressLoadInformation->DependencyResourceHandles.push_back(dependencyResourceHandle);

				const bool isDependencyLoaded = mLoadedResourceInformation.find(dependencyId) != mLoadedResourceInformation.end();
				if(isDependencyLoaded)
					continue;

				mDependantResourceLoads[dependencyId].Add(inProgressLoadInformation);
				inProgressLoadInformation->RemainingResourcesToLoadCount++;
			}
		}

		mInProgressLoadInformation[resourceId].Add(inProgressLoadInformation);
	}

	// Issue load request for all dependencies
	if(loadOptions.LoadDependencies)
	{
		ResourceLoadOptions dependencyLoadOptions;
		dependencyLoadOptions.LoadDependencies = false;
		dependencyLoadOptions.AsynchronousLoad = loadOptions.AsynchronousLoad;

		for(const UUID& dependencyId : metaData->Dependencies)
		{
			if(dependencyId == resourceId)
				continue;

			HResource dependency = Load(dependencyId, dependencyLoadOptions);
			(void)dependency;
		}
	}

	auto fnLoadFromPackageAndFinalize = [this, inProgressLoadInformationWeak = WeakSPtr<InProgressLoadInformation>(inProgressLoadInformation), package, resourceId]()
	{
		const SPtr<InProgressLoadInformation> inProgressLoadInformation = inProgressLoadInformationWeak.lock();
		if(!B3D_ENSURE(inProgressLoadInformation != nullptr))
			return;

		const SPtr<Resource> resource = package->DeserializeResource(resourceId);

		{
			Lock lock(mLoadedResourceMutex);
			inProgressLoadInformation->ResourceHandle.SetHandleData(resource, resourceId);

			if(B3D_ENSURE(inProgressLoadInformation->RemainingResourcesToLoadCount > 0))
				inProgressLoadInformation->RemainingResourcesToLoadCount--;

			if(resource != nullptr)
				resource->SetHandle(inProgressLoadInformation->ResourceHandle.GetWeak());
		}

		TryFinalizeLoad(inProgressLoadInformation);
		inProgressLoadInformation->LoadingEvent.Signal();
	};

	bool asyncLoad = loadOptions.AsynchronousLoad;
	//if(!Resources::SupportsAsyncLoad(metaData->TypeId)) // TODO - Implement this by reading the value from a RTTI default object
	//	asyncLoad = false;

	if(asyncLoad)
	{
		const String resourceFileName = metaData->Path.GetFilename();
		GetCoreApplication().GetTaskScheduler().Post(SchedulerTask(fnLoadFromPackageAndFinalize, "Load resource", SchedulerTaskFlag::None, resourceFileName));
	}
	else 
	{
		fnLoadFromPackageAndFinalize();
	}

	return resourceHandle;
}

void Resources::TryFinalizeLoad(const SPtr<InProgressLoadInformation>& inProgressLoadInformation)
{
	const UUID& resourceId = inProgressLoadInformation->ResourceHandle.GetId();

	TInlineArray<SPtr<InProgressLoadInformation>, 4> dependantLoads;
	{
		Lock lock(mLoadedResourceMutex);

		bool isReadyToFinalizeLoad = inProgressLoadInformation->RemainingResourcesToLoadCount == 0 && !inProgressLoadInformation->LoadFinished;
		if(!isReadyToFinalizeLoad)
			return;

		// Mark the load as complete so we could have multiple in-progress loads for the same resource, so we don't finalize twice
		inProgressLoadInformation->LoadFinished = true;

		// Remove from in-progress map
		if(auto found = mInProgressLoadInformation.find(resourceId); B3D_ENSURE(found != mInProgressLoadInformation.end()))
		{
			TInlineArray<SPtr<InProgressLoadInformation>, 1>& loadsPerResource = found->second;
			for(auto it = loadsPerResource.begin(); it != loadsPerResource.end(); ++it)
			{
				if((*it) == inProgressLoadInformation)
				{
					loadsPerResource.erase(it);
					break;
				}
			}

			if(loadsPerResource.Empty())
				mInProgressLoadInformation.erase(found);
		}

		// Add or update the loaded resource map
		LoadedResourceInformation* loadedResourceInformation;
		if(const auto foundLoadedResource = mLoadedResourceInformation.find(resourceId); foundLoadedResource != mLoadedResourceInformation.end())
		{
			loadedResourceInformation = foundLoadedResource->second.get();

			if(loadedResourceInformation->ResourceHandle == nullptr)
				loadedResourceInformation->ResourceHandle = inProgressLoadInformation->ResourceHandle.GetWeak();
		}
		else
		{
			UPtr<LoadedResourceInformation> newLoadedResourceInformation = B3DMakeUnique<LoadedResourceInformation>();
			newLoadedResourceInformation->ResourceHandle = inProgressLoadInformation->ResourceHandle.GetWeak();

			loadedResourceInformation = newLoadedResourceInformation.get();

			mLoadedResourceInformation[resourceId] = std::move(newLoadedResourceInformation);
		}

		if(inProgressLoadInformation->LoadOptions.LoadDependencies)
			loadedResourceInformation->DependenciesLoaded = true;

		if(inProgressLoadInformation->LoadOptions.KeepInternalReference)
		{
			loadedResourceInformation->InternalReferenceCount = 1;
			loadedResourceInformation->ResourceHandle.IncrementInternalReferenceCount();
		}

		inProgressLoadInformation->ResourceHandle.NotifyLoadComplete();

		// Record any dependants we need to notify, and decrement their load counts
		if(const auto found = mDependantResourceLoads.find(resourceId); found != mDependantResourceLoads.end())
		{
			dependantLoads = found->second;
			mDependantResourceLoads.erase(found);
		}

		for(const auto& dependantLoad : dependantLoads)
		{
			if(B3D_ENSURE(dependantLoad->RemainingResourcesToLoadCount > 0))
				dependantLoad->RemainingResourcesToLoadCount--;
		}
	}

	// Notify external code
	if(inProgressLoadInformation->ResourceHandle.IsLoaded(false))
		OnResourceLoaded(inProgressLoadInformation->ResourceHandle);

	if(!inProgressLoadInformation->LoadOptions.AsynchronousLoad && GetCoreApplication().GetMainThreadId() == B3D_CURRENT_THREAD_ID)
		ResourceListenerManager::Instance().NotifyListeners(resourceId);

	// See if any dependant load's remaining resource count reached 0, and try to finalize them
	for(const auto& dependantLoad : dependantLoads)
		TryFinalizeLoad(dependantLoad);
}

void Resources::ReleaseInternalReference(ResourceHandle& resource)
{
	const UUID& uuid = resource.GetId();

	{
		bool loadInProgress = false;

		{
			Lock inProgressLock(mInProgressResourcesMutex);
			auto iterFind2 = mInProgressResources.find(uuid);
			if(iterFind2 != mInProgressResources.end())
				loadInProgress = true;
		}

		// Technically we should be able to just cancel a load in progress instead of blocking until it finishes.
		// However that would mean the last reference could get lost on whatever thread did the loading, which
		// isn't something that's supported. If this ends up being a problem either make handle counting atomic
		// or add a separate queue for objects destroyed from the load threads.
		if(loadInProgress)
			resource.BlockUntilLoaded();

		bool lostLastRef = false;
		{
			Lock loadedLock(mLoadedResourceMutex);
			auto iterFind = mLoadedResources.find(uuid);
			if(iterFind != mLoadedResources.end())
			{
				LoadedResourceData& resData = iterFind->second;

				B3D_ASSERT(resData.NumInternalRefs > 0);
				resData.NumInternalRefs--;
				resource.DecrementInternalReferenceCount();

				std::uint32_t refCount = resource.GetHandleData()->ReferenceCount.load(std::memory_order_relaxed);
				lostLastRef = refCount == 0;
			}
		}

		if(lostLastRef)
			Destroy(resource);
	}
}

void Resources::UnloadAllUnused()
{
	Vector<HResource> resourcesToUnload;

	{
		Lock lock(mLoadedResourceMutex);
		for(auto iter = mLoadedResources.begin(); iter != mLoadedResources.end(); ++iter)
		{
			const LoadedResourceData& resData = iter->second;

			std::uint32_t refCount = resData.Resource.mData->ReferenceCount.load(std::memory_order_relaxed);
			B3D_ASSERT(refCount > 0); // No references but kept in mLoadedResources list?

			if(refCount == resData.NumInternalRefs) // Only internal references exist, free it
				resourcesToUnload.push_back(resData.Resource.Lock());
		}
	}

	// Note: When unloading multiple resources it's possible that unloading one will also unload
	// another resource in "resourcesToUnload". This is fine because "unload" deals with invalid
	// handles gracefully.
	for(auto iter = resourcesToUnload.begin(); iter != resourcesToUnload.end(); ++iter)
	{
		ReleaseInternalReference(*iter);
	}
}

void Resources::UnloadAll()
{
	// Unload and invalidate all resources
	UnorderedMap<UUID, LoadedResourceData> loadedResourcesCopy;

	{
		Lock lock(mLoadedResourceMutex);
		loadedResourcesCopy = mLoadedResources;
	}

	for(auto& loadedResourcePair : loadedResourcesCopy)
		Destroy(loadedResourcePair.second.Resource);
}

void Resources::Destroy(ResourceHandle& resource)
{
	if(resource.mData == nullptr)
		return;

	// TODO - Begin deprecated
	RecursiveLock lock(mDestroyMutex);

	// If load in progress, first wait until it completes
	const UUID& resourceId = resource.GetId();
	if(!resource.IsLoaded(false))
	{
		bool loadInProgress = false;
		{
			Lock lock(mInProgressResourcesMutex);
			auto iterFind2 = mInProgressResources.find(resourceId);
			if(iterFind2 != mInProgressResources.end())
				loadInProgress = true;
		}

		if(loadInProgress) // If it's still loading wait until that finishes
			resource.BlockUntilLoaded();
		else
			return; // Already unloaded
	}

	// At this point resource is guaranteed to be loaded and this state cannot change by some other thread because of
	// the mDestroyMutex lock
	// TODO - End deprecated

	// Notify external systems before we actually destroy it
	OnResourceDestroyed(resourceId);

	{
		Lock lock(mLoadedResourceMutex);
		auto iterFind = mLoadedResources.find(resourceId);
		if(iterFind != mLoadedResources.end())
		{
			LoadedResourceData& resData = iterFind->second;
			while(resData.NumInternalRefs > 0)
			{
				resData.NumInternalRefs--;
				resData.Resource.DecrementInternalReferenceCount();
			}

			mLoadedResources.erase(iterFind);
		}
		else
		{
			B3D_ASSERT(false); // This should never happen but in case it does fail silently in release mode
		}

		// TODO - Do I need to wait for in-progress loads to finish above?
		UPtr<LoadedResourceInformation> loadedResourceInformation;
		if(auto found = mLoadedResourceInformation.find(resourceId); found != mLoadedResourceInformation.end())
		{
			loadedResourceInformation = std::move(found->second);
			mLoadedResourceInformation.erase(found);
		}

		if(loadedResourceInformation != nullptr)
		{
			while(loadedResourceInformation->InternalReferenceCount > 0)
			{
				loadedResourceInformation->InternalReferenceCount--;
				loadedResourceInformation->ResourceHandle.DecrementInternalReferenceCount();
			}
		}

		PackageManager& packageManager = GetPackageManager();
		if(const auto& packagePath = packageManager.TryGetPackagePathForResource(resourceId); packagePath.has_value())
		{
			AcquirePackageReadLockOptions readLockOptions(false, true, "Destroy resource");
			UPtr<PackageReadLock> packageReadLock;
			const AcquirePackageLockResult lockResult = packageManager.AcquireReadLock(*packagePath, readLockOptions, packageReadLock);
			if(lockResult != AcquirePackageLockResult::Acquired || packageReadLock == nullptr)
				return;

			const SPtr<Package>& package = packageReadLock->GetPackage();
			if(!B3D_ENSURE(package != nullptr))
				return;

			package->UnloadResource(resourceId);
		}
	}

	resource.mData->Object->Destroy();
	resource.ClearHandleData();
}

void Resources::SaveAsSinglePackage(const HResource& resource, const Path& folder, const String& name, const ResourceSaveOptions& saveOptions)
{
	if(folder.IsEmpty())
	{
		B3D_LOG(Warning, Resources, "Cannot save resource. Provided folder is empty.");
		return;
	}

	if(name.empty())
	{
		B3D_LOG(Warning, Resources, "Cannot save resource. Provided name is empty.");
		return;
	}

	if(!folder.IsAbsolute())
	{
		B3D_LOG(Warning, Resources, "Cannot save resource. Provided folder is not absolute.");
		return;
	}

	if(!resource.IsLoaded(false))
	{
		B3D_LOG(Warning, Resources, "Cannot save resource at path {0}/{1}.b3d. Provided resource is null or not loaded.", folder, name);
		return;
	}

	const String& packageFilename = name + Package::kPackageExtension;
	const Path packagePath = Path::Combine(folder, packageFilename);

	const SPtr<Package> package = Package::Create(name);
	package->AddResource(name, resource);

	SavePackageOptions packageSaveOptions;
	packageSaveOptions.Compress = saveOptions.Compress;
	packageSaveOptions.Overwrite = saveOptions.Overwrite;
	packageSaveOptions.VirtualPathPrefix = saveOptions.VirtualPathPrefix;

	PackageManager& packageManager = GetPackageManager();
	packageManager.SavePackage(package, packagePath, packageSaveOptions);
}

void Resources::UpdateHandle(HResource& handle, const SPtr<Resource>& resource)
{
	const UUID& uuid = handle.GetId();
	handle.SetHandleData(resource, uuid);
	handle.NotifyLoadComplete();

	OnResourceModified(handle);

	// Notify listeners immediately if on main thread
	if(GetCoreApplication().GetMainThreadId() == B3D_CURRENT_THREAD_ID)
		ResourceListenerManager::Instance().NotifyListeners(uuid);
}

void Resources::UpdateResourcesFromPackage(const UPtr<PackageWriteLock>& packageWriteLock)
{
	if(!B3D_ENSURE(packageWriteLock != nullptr))
		return;

	const SPtr<Package>& package = packageWriteLock->GetPackage();
	if(package == nullptr)
		return;

	const Vector<UUID> packageResourceIds = package->CreateResourceIdList();
	for(const auto& resourceId : packageResourceIds)
	{
		HResource resourceHandle;
		{
			Lock lock(mLoadedResourceMutex);

			if(const auto found = mLoadedResourceInformation.find(resourceId); found != mLoadedResourceInformation.end())
				resourceHandle = found->second->ResourceHandle.Lock();
		}

		if(resourceHandle == nullptr)
		{
			package->UnloadResource(resourceId);
			continue;
		}

		const SPtr<Resource> resource = package->LoadResource(resourceId);
		if(resource == nullptr)
		{
			B3D_LOG(Warning, Resources, "Failed to update resource '{0}' with new data from the package. Unknown error.", resourceId);
			continue;
		}

		if(resourceHandle.GetShared() != resource)
			UpdateHandle(resourceHandle, resource);
	}
}

Vector<UUID> Resources::GetDependencies(const Path& filePath)
{
	SPtr<SavedResourceData> savedResourceData;
	if(!filePath.IsEmpty())
	{
		FileDecoder fs(filePath);
		savedResourceData = std::static_pointer_cast<SavedResourceData>(fs.Decode());
	}

	return savedResourceData->GetDependencies();
}

void Resources::RegisterResourceManifest(const SPtr<ResourceManifest>& manifest)
{
	auto findIter = std::find(mResourceManifests.begin(), mResourceManifests.end(), manifest);
	if(findIter == mResourceManifests.end())
		mResourceManifests.push_back(manifest);
	else
		*findIter = manifest;
}

void Resources::UnregisterResourceManifest(const SPtr<ResourceManifest>& manifest)
{
	if(manifest->GetName() == "Default")
		return;

	auto findIter = std::find(mResourceManifests.begin(), mResourceManifests.end(), manifest);
	if(findIter != mResourceManifests.end())
		mResourceManifests.erase(findIter);
}

SPtr<ResourceManifest> Resources::GetResourceManifest(const String& name) const
{
	for(auto iter = mResourceManifests.rbegin(); iter != mResourceManifests.rend(); ++iter)
	{
		if(name == (*iter)->GetName())
			return (*iter);
	}

	return nullptr;
}

bool Resources::IsLoaded(const UUID& uuid, bool checkInProgress)
{
	if(checkInProgress)
	{
		Lock inProgressLock(mInProgressResourcesMutex);
		auto iterFind2 = mInProgressResources.find(uuid);
		if(iterFind2 != mInProgressResources.end())
		{
			return true;
		}
	}

	{
		Lock loadedLock(mLoadedResourceMutex);
		auto iterFind = mLoadedResources.find(uuid);
		if(iterFind != mLoadedResources.end())
		{
			return true;
		}

		if(auto found = mLoadedResourceInformation.find(uuid); found != mLoadedResourceInformation.end())
			return true;

		if(auto found = mInProgressLoadInformation.find(uuid); checkInProgress && found != mInProgressLoadInformation.end())
			return true;
	}

	return false;
}

float Resources::GetLoadProgress(const HResource& resource, bool includeDependencies)
{
	const UUID& uuid = resource.GetId();
	if(uuid.Empty())
		return 0.0f;

	Lock inProgressLock(mInProgressResourcesMutex);
	Lock loadedLock(mLoadedResourceMutex);

	// Fully loaded
	auto iterFind = mLoadedResources.find(uuid);
	if(iterFind != mLoadedResources.end())
		return 1.0f;

	// Not loaded nor being loaded
	auto iterFind2 = mInProgressResources.find(uuid);
	if(iterFind2 == mInProgressResources.end())
		return 0.0f;

	ResourceLoadData* loadData = iterFind2->second;

	// Don't care about dependencies, just report own progress directly
	if(!includeDependencies)
		return loadData->Progress.load(std::memory_order_relaxed);

	// Dependencies that are already fully loaded will just have their loaded sizes in 'dependencyLoadedAmount', while
	// for those still in progress we need to check their load data
	float totalBytesLoaded = (float)loadData->DependencyLoadedAmount;
	for(auto& entry : loadData->Dependencies)
	{
		auto iterFind3 = mInProgressResources.find(entry.GetId());
		if(iterFind3 == mInProgressResources.end())
			continue;

		ResourceLoadData* dependencyLoadData = iterFind3->second;
		totalBytesLoaded += dependencyLoadData->ResData.Size * dependencyLoadData->Progress.load(std::memory_order_relaxed);
	}

	totalBytesLoaded += loadData->ResData.Size * loadData->Progress.load(std::memory_order_relaxed);

	float totalBytesToLoad = (float)(loadData->DependencySize + loadData->ResData.Size);
	B3D_ASSERT(totalBytesLoaded <= totalBytesToLoad);

	return std::min(1.0f, totalBytesLoaded / totalBytesToLoad);
}

float Resources::GetLoadProgress2(const HResource& resource)
{
	UnorderedMap<UUID, LoadProgress> loadProgressMap;
	GetLoadProgressRecursive(resource, loadProgressMap);

	u64 totalSize = 0;
	u64 loadedSize = 0;
	for(const auto& entry : loadProgressMap)
	{
		totalSize += entry.second.TotalSize;
		loadedSize += (u64)((double)entry.second.TotalSize * entry.second.Progress);
	}

	return totalSize != 0 ? (float)((double)loadedSize / (double)totalSize) : 0.0f;
}

void Resources::GetLoadProgressRecursive(const HResource& resource, UnorderedMap<UUID, LoadProgress>& loadProgressMap)
{
	const UUID& resourceId = resource.GetId();
	if(resourceId.Empty())
		return;

	auto fnGetLoadProgress = [](const UUID& resourceId) -> LoadProgress
	{
		PackageManager& packageManager = GetPackageManager();

		Optional<Path> maybePackagePath = packageManager.TryGetPackagePathForResource(resourceId);
		if(!maybePackagePath.has_value())
			return LoadProgress();

		const Path& packagePath = *maybePackagePath;

		AcquirePackageReadLockOptions readLockOptions(false, true, "GetLoadProgress");
		UPtr<PackageReadLock> packageReadLock;
		const AcquirePackageLockResult lockResult = packageManager.AcquireReadLock(packagePath, readLockOptions, packageReadLock);
		if(lockResult != AcquirePackageLockResult::Acquired || packageReadLock == nullptr)
			return LoadProgress();

		const SPtr<Package>& package = packageReadLock->GetPackage();
		if(!B3D_ENSURE(package != nullptr))
			return LoadProgress();

		const float progress = package->GetResourceLoadProgress(resourceId);
		const u64 size = package->GetResourceSizeInDataStream(resourceId);

		return LoadProgress(size, progress);
	};

	LoadProgress selfLoadProgress = fnGetLoadProgress(resourceId);

	UnorderedMap<UUID, HResource> uniqueDependencies;

	{
		Lock lock(mLoadedResourceMutex);

		if(const auto found = mInProgressLoadInformation.find(resourceId); found != mInProgressLoadInformation.end())
		{
			const TInlineArray<SPtr<InProgressLoadInformation>, 1>& inProgressLoadsPerResource = found->second;
			for(const SPtr<InProgressLoadInformation>& entry : inProgressLoadsPerResource)
			{
				if(!B3D_ENSURE(entry != nullptr))
					continue;

				for(const auto& dependency : entry->DependencyResourceHandles)
					uniqueDependencies[dependency.GetId()] = dependency;
			}
		}
		else
		{
			// Either fully loaded or not being loaded at all, we don't even care about dependencies
			if(const auto foundLoaded = mLoadedResourceInformation.find(resourceId); foundLoaded != mLoadedResourceInformation.end())
				selfLoadProgress.Progress = 1.0f;
			else
				selfLoadProgress.Progress = 0.0f;
		}
	}

	for(const auto& dependency : uniqueDependencies)
		GetLoadProgressRecursive(dependency.second, loadProgressMap);

	loadProgressMap[resourceId] = selfLoadProgress;
}

HResource Resources::CreateResourceHandle(const SPtr<Resource>& resource)
{
	if(resource == nullptr)
		return nullptr;

	const UUID& uuid = resource->GetId().Empty() ? UUIDGenerator::GenerateRandom() : resource->GetId();
	return CreateResourceHandle(resource, uuid);
}

HResource Resources::CreateResourceHandle(const SPtr<Resource>& resource, const UUID& resourceId)
{
	HResource newHandle(resource, resourceId);

	{
		Lock handleLock(mResourceHandleMutex);

		if(resource)
		{
			resource->SetHandle(newHandle.GetWeak());

			Lock lock(mLoadedResourceMutex);

			LoadedResourceData& resData = mLoadedResources[resourceId];
			resData.Resource = newHandle.GetWeak();

			// TODO - Add to mLoadedResourceInformation. Update other usages of mLoadedResources
		}

		mHandles[resourceId] = newHandle.GetWeak(); // TODO - Need to free entry from this map if the handle data goes out of scope
	}

	return newHandle;
}

HResource Resources::GetOrCreateResourceHandle(const UUID& resourceId)
{
	Lock handleLock(mResourceHandleMutex);
	if(auto found = mHandles.find(resourceId); found != mHandles.end()) // Not loaded, but handle does exist
		return found->second.Lock();

	// Create new handle
	HResource handle(resourceId);
	mHandles[resourceId] = handle.GetWeak();

	return handle;
}

bool Resources::GetFilePathFromUuid(const UUID& uuid, Path& filePath) const
{
	// Default manifest is at 0th index but all other take priority since Default manifest could
	// contain obsolete data.
	for(auto iter = mResourceManifests.rbegin(); iter != mResourceManifests.rend(); ++iter)
	{
		if((*iter)->UUIDToPhysicalFilePath(uuid, filePath))
			return true;
	}

	return false;
}

bool Resources::GetUUIDFromFilePath(const Path& path, UUID& outUUID) const
{
	Path absolutePhysicalPath = path;
	if(!absolutePhysicalPath.IsAbsolute())
		absolutePhysicalPath.MakeAbsolute(FileSystem::GetWorkingDirectoryPath());

	for(auto iter = mResourceManifests.rbegin(); iter != mResourceManifests.rend(); ++iter)
	{
		const ResourceManifest& manifest = *iter->get();

		if(manifest.PhysicalFilePathToUUID(path, outUUID))
			return true;
	}

	return false;
}

Path Resources::EnsurePhysicalPath(const Path& path) const
{
	// Check if a virtual file path first
	for(auto iter = mResourceManifests.rbegin(); iter != mResourceManifests.rend(); ++iter)
	{
		const ResourceManifest& manifest = *iter->get();

		Path physicalPath;
		if(manifest.VirtualToPhysicalPath(path, physicalPath))
			return physicalPath;
	}

	// Not a virtual path? Returns the path as-is.
	return path;
}

namespace bs
{
B3D_CORE_EXPORT Resources& GetResources()
{
	return Resources::Instance();
}
} // namespace bs
