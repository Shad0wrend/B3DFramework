//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Rendering
	 *  @{
	 */

	/// <summary>Settings that control the motion blur effect.</summary>
	[ShowInInspector]
	public partial class MotionBlurSettings : ScriptObject
	{
		private MotionBlurSettings(bool __dummy0) { }

		public MotionBlurSettings()
		{
			Internal_MotionBlurSettings(this);
		}

		/// <summary>Enables or disables the motion blur effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Enabled
		{
			get { return Internal_Getenabled(mCachedPtr); }
			set { Internal_Setenabled(mCachedPtr, value); }
		}

		/// <summary>Determines which parts of the scene will trigger motion blur.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public MotionBlurDomain Domain
		{
			get { return Internal_Getdomain(mCachedPtr); }
			set { Internal_Setdomain(mCachedPtr, value); }
		}

		/// <summary>Type of filter to use when filtering samples contributing to a blurred pixel.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public MotionBlurFilter Filter
		{
			get { return Internal_Getfilter(mCachedPtr); }
			set { Internal_Setfilter(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the number of samples to take during motion blur filtering. Increasing this value will yield higher 
		/// quality blur at the cost of the performance.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public MotionBlurQuality Quality
		{
			get { return Internal_Getquality(mCachedPtr); }
			set { Internal_Setquality(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the maximum radius over which the blur samples are allowed to be taken, in percent of the screen width 
		/// (e.g. with 1% radius, on 1920x1028 resolution the maximum radius in pixels will be 1920 * 0.01 = 20px). This clamps 
		/// the maximum velocity that can affect the blur, as higher velocities require higher radius. Very high values can 
		/// adversely affect performance as cache accesses become more random.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float MaximumRadius
		{
			get { return Internal_GetmaximumRadius(mCachedPtr); }
			set { Internal_SetmaximumRadius(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_MotionBlurSettings(MotionBlurSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getenabled(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setenabled(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern MotionBlurDomain Internal_Getdomain(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setdomain(IntPtr thisPtr, MotionBlurDomain value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern MotionBlurFilter Internal_Getfilter(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setfilter(IntPtr thisPtr, MotionBlurFilter value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern MotionBlurQuality Internal_Getquality(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setquality(IntPtr thisPtr, MotionBlurQuality value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetmaximumRadius(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaximumRadius(IntPtr thisPtr, float value);
	}

	/** @} */
}
