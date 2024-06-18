//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
#if !IS_B3D
	/** @addtogroup Resources
	 *  @{
	 */

	/// <summary>
	/// Manager for dealing with all engine resources. It allows you to save new resources and load existing ones.
	/// </summary>
	[ShowInInspector]
	public partial class Resources : ScriptObject
	{
		private Resources(bool __dummy0) { }
		protected Resources() { }

		/// <summary>Called when the resource has been successfully loaded.</summary>
		public static event Action<RRefBase> OnResourceLoaded;

		/// <summary>Called when the resource has been destroyed. Provides UUID of the destroyed resource.</summary>
		public static event Action<UUID> OnResourceDestroyed;

		/// <summary>Called when the internal resource the handle is pointing to has changed.</summary>
		public static event Action<RRefBase> OnResourceModified;

		/// <summary>
		/// Releases an internal reference to the resource held by the resources system. This allows the resource to be unloaded 
		/// when it goes out of scope, if the resource was loaded with <see cref="KeepInternalReference"/> option.
		///
		/// Alternatively you can also skip manually calling ReleaseInternalReference() and call UnloadAllUnused() which will 
		/// unload all resources that do not have any external references, but you lose the fine grained control of what will be 
		/// unloaded.
		/// </summary>
		public static void ReleaseInternalReference(RRefBase resource)
		{
			Internal_ReleaseInternalReference(resource);
		}

		/// <summary>Finds all resources that aren&apos;t being referenced outside of the resources system and unloads them.</summary>
		public static void UnloadAllUnused()
		{
			Internal_UnloadAllUnused();
		}

		/// <summary>Forces unload of all resources, whether they are being used or not.</summary>
		public static void UnloadAll()
		{
			Internal_UnloadAll();
		}

		/// <summary>Checks is the resource with the specified UUID loaded.</summary>
		/// <param name="uuid">UUID of the resource to check.</param>
		/// <param name="checkInProgress">
		/// Should this method also check resources that are in progress of being asynchronously loaded.
		/// </param>
		/// <returns>True if loaded or loading in progress, false otherwise.</returns>
		public static bool IsLoaded(UUID uuid, bool checkInProgress = true)
		{
			return Internal_IsLoaded(ref uuid, checkInProgress);
		}

		/// <summary>Returns the loading progress of a resource that&apos;s being loaded</summary>
		/// <param name="resource">Resource whose load progress to check.</param>
		/// <returns>Load progress in range [0, 1].</returns>
		public static float GetLoadProgress(RRefBase resource)
		{
			return Internal_GetLoadProgress(resource);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ReleaseInternalReference(RRefBase resource);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_UnloadAllUnused();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_UnloadAll();
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_IsLoaded(ref UUID uuid, bool checkInProgress);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetLoadProgress(RRefBase resource);
		private static void Internal_OnResourceLoaded(RRefBase p0)
		{
			OnResourceLoaded?.Invoke(p0);
		}
		private static void Internal_OnResourceDestroyed(ref UUID p0)
		{
			OnResourceDestroyed?.Invoke(p0);
		}
		private static void Internal_OnResourceModified(RRefBase p0)
		{
			OnResourceModified?.Invoke(p0);
		}
	}

	/** @} */
#endif
}
