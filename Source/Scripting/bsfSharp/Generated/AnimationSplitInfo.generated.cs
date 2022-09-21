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

	/// <summary>Information about how to split an AnimationClip into multiple separate clips.</summary>
	[ShowInInspector]
	public partial class AnimationSplitInfo : ScriptObject
	{
		private AnimationSplitInfo(bool __dummy0) { }

		public AnimationSplitInfo()
		{
			Internal_AnimationSplitInfo(this);
		}

		public AnimationSplitInfo(string name, int startFrame, int endFrame, bool isAdditive = false)
		{
			Internal_AnimationSplitInfo0(this, name, startFrame, endFrame, isAdditive);
		}

		[ShowInInspector]
		[NativeWrapper]
		public string Name
		{
			get { return Internal_Getname(mCachedPtr); }
			set { Internal_Setname(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public int StartFrame
		{
			get { return Internal_GetstartFrame(mCachedPtr); }
			set { Internal_SetstartFrame(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public int EndFrame
		{
			get { return Internal_GetendFrame(mCachedPtr); }
			set { Internal_SetendFrame(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public bool IsAdditive
		{
			get { return Internal_GetisAdditive(mCachedPtr); }
			set { Internal_SetisAdditive(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_AnimationSplitInfo(AnimationSplitInfo managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_AnimationSplitInfo0(AnimationSplitInfo managedInstance, string name, int startFrame, int endFrame, bool isAdditive);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern string Internal_Getname(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setname(IntPtr thisPtr, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetstartFrame(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetstartFrame(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetendFrame(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetendFrame(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetisAdditive(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetisAdditive(IntPtr thisPtr, bool value);
	}

	/** @} */
#endif
}
