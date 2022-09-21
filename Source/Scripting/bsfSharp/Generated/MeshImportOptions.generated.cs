//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
#if !IS_B3D
	/** @addtogroup Importer
	 *  @{
	 */

	/// <summary>
	/// Contains import options you may use to control how is a mesh imported from some external format into engine format.
	/// </summary>
	[ShowInInspector]
	public partial class MeshImportOptions : ImportOptions
	{
		private MeshImportOptions(bool __dummy0) { }

		/// <summary>Creates a new import options object that allows you to customize how are meshes imported.</summary>
		public MeshImportOptions()
		{
			Internal_Create(this);
		}

		/// <summary>Determines whether the texture data is also stored in CPU memory.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool CpuCached
		{
			get { return Internal_GetcpuCached(mCachedPtr); }
			set { Internal_SetcpuCached(mCachedPtr, value); }
		}

		/// <summary>Determines should mesh normals be imported if available.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportNormals
		{
			get { return Internal_GetimportNormals(mCachedPtr); }
			set { Internal_SetimportNormals(mCachedPtr, value); }
		}

		/// <summary>Determines should mesh tangents and bitangents be imported if available.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportTangents
		{
			get { return Internal_GetimportTangents(mCachedPtr); }
			set { Internal_SetimportTangents(mCachedPtr, value); }
		}

		/// <summary>Determines should mesh blend shapes be imported if available.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportBlendShapes
		{
			get { return Internal_GetimportBlendShapes(mCachedPtr); }
			set { Internal_SetimportBlendShapes(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should mesh skin data like bone weights, indices and bind poses be imported if available.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportSkin
		{
			get { return Internal_GetimportSkin(mCachedPtr); }
			set { Internal_SetimportSkin(mCachedPtr, value); }
		}

		/// <summary>Determines should animation clips be imported if available.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportAnimation
		{
			get { return Internal_GetimportAnimation(mCachedPtr); }
			set { Internal_SetimportAnimation(mCachedPtr, value); }
		}

		/// <summary>
		/// Enables or disables keyframe reduction. Keyframe reduction will reduce the number of key-frames in an animation clip 
		/// by removing identical keyframes, and therefore reducing the size of the clip.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ReduceKeyFrames
		{
			get { return Internal_GetreduceKeyFrames(mCachedPtr); }
			set { Internal_SetreduceKeyFrames(mCachedPtr, value); }
		}

		/// <summary>
		/// Enables or disables import of root motion curves. When enabled, any animation curves in imported animations affecting 
		/// the root bone will be available through a set of separate curves in AnimationClip, and they won&apos;t be evaluated 
		/// through normal animation process. Instead it is expected that the user evaluates the curves manually and applies them 
		/// as required.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ImportRootMotion
		{
			get { return Internal_GetimportRootMotion(mCachedPtr); }
			set { Internal_SetimportRootMotion(mCachedPtr, value); }
		}

		/// <summary>Uniformly scales the imported mesh by the specified value.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float ImportScale
		{
			get { return Internal_GetimportScale(mCachedPtr); }
			set { Internal_SetimportScale(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines what type (if any) of collision mesh should be imported. If enabled the collision mesh will be available 
		/// as a sub-resource returned by the importer (along with the normal mesh).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public CollisionMeshType CollisionMeshType
		{
			get { return Internal_GetcollisionMeshType(mCachedPtr); }
			set { Internal_SetcollisionMeshType(mCachedPtr, value); }
		}

		/// <summary>
		/// Animation split infos that determine how will the source animation clip be split. If no splits are present the data 
		/// will be imported as one clip, but if splits are present the data will be split according to the split infos. Split 
		/// infos only affect the primary animation clip, other clips will not be split.
		/// </summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AnimationSplitInfo[] AnimationSplits
		{
			get { return Internal_GetanimationSplits(mCachedPtr); }
			set { Internal_SetanimationSplits(mCachedPtr, value); }
		}

		/// <summary>Set of events that will be added to the animation clip, if animation import is enabled.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ImportedAnimationEvents[] AnimationEvents
		{
			get { return Internal_GetanimationEvents(mCachedPtr); }
			set { Internal_SetanimationEvents(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetcpuCached(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcpuCached(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportNormals(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportNormals(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportTangents(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportTangents(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportBlendShapes(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportBlendShapes(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportSkin(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportSkin(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportAnimation(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportAnimation(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetreduceKeyFrames(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetreduceKeyFrames(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetimportRootMotion(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportRootMotion(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetimportScale(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimportScale(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CollisionMeshType Internal_GetcollisionMeshType(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcollisionMeshType(IntPtr thisPtr, CollisionMeshType value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AnimationSplitInfo[] Internal_GetanimationSplits(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetanimationSplits(IntPtr thisPtr, AnimationSplitInfo[] value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ImportedAnimationEvents[] Internal_GetanimationEvents(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetanimationEvents(IntPtr thisPtr, ImportedAnimationEvents[] value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create(MeshImportOptions managedInstance);
	}

	/** @} */
#endif
}
