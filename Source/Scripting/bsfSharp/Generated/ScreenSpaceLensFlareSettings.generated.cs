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

	/// <summary>Settings that control the screen-space lens flare effect.</summary>
	[ShowInInspector]
	public partial class ScreenSpaceLensFlareSettings : ScriptObject
	{
		private ScreenSpaceLensFlareSettings(bool __dummy0) { }

		public ScreenSpaceLensFlareSettings()
		{
			Internal_ScreenSpaceLensFlareSettings(this);
		}

		/// <summary>Enables or disables the lens flare effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Enabled
		{
			get { return Internal_Getenabled(mCachedPtr); }
			set { Internal_Setenabled(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines how many times to downsample the scene texture before using it for lens flare effect. Lower values will 
		/// use higher resolution texture for calculating lens flare, at the cost of lower performance. Valid range is [1, 6], 
		/// default is 4.
		/// </summary>
		[ShowInInspector]
		[Range(1f, 6f, false)]
		[NativeWrapper]
		public int DownsampleCount
		{
			get { return Internal_GetdownsampleCount(mCachedPtr); }
			set { Internal_SetdownsampleCount(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the minimal threshold of pixel luminance to be included in the lens flare calculations. Any pixel with 
		/// luminance below this value will be ignored for the purposes of lens flare. Set to zero or negative to disable the 
		/// threshold and include all pixels in the calculations.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Threshold
		{
			get { return Internal_Getthreshold(mCachedPtr); }
			set { Internal_Setthreshold(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the number of ghost features to appear, shown as blurred blobs of bright areas of the scene.
		/// </summary>
		[ShowInInspector]
		[Range(1f, 10f, false)]
		[NativeWrapper]
		public int GhostCount
		{
			get { return Internal_GetghostCount(mCachedPtr); }
			set { Internal_SetghostCount(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the distance between ghost features. Value is in normalized screen space, in range [0,1] where 1 
		/// represents the full screen length.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float GhostSpacing
		{
			get { return Internal_GetghostSpacing(mCachedPtr); }
			set { Internal_SetghostSpacing(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the brightness of the lens flare effect. Value of 1 means the lens flare will be displayed at the same 
		/// intensity as the scene it was derived from. In range [0, 1], default being 0.05.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float Brightness
		{
			get { return Internal_Getbrightness(mCachedPtr); }
			set { Internal_Setbrightness(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the size of the filter when blurring the lens flare features. Larger values yield a blurrier image and 
		/// will require more performance.
		/// </summary>
		[ShowInInspector]
		[Range(0.01f, 1f, false)]
		[NativeWrapper]
		public float FilterSize
		{
			get { return Internal_GetfilterSize(mCachedPtr); }
			set { Internal_SetfilterSize(mCachedPtr, value); }
		}

		/// <summary>Determines if a halo effect should be rendered as part of the lens flare.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Halo
		{
			get { return Internal_Gethalo(mCachedPtr); }
			set { Internal_Sethalo(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines how far away from the screen center does the halo appear, in normalized screen space (range [0,1]) where 
		/// 0.5 represents half screen length.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float HaloRadius
		{
			get { return Internal_GethaloRadius(mCachedPtr); }
			set { Internal_SethaloRadius(mCachedPtr, value); }
		}

		/// <summary>Determines the thickness of the halo ring. In normalized screen space (range [0.01,0.5]).</summary>
		[ShowInInspector]
		[Range(0.01f, 0.5f, false)]
		[NativeWrapper]
		public float HaloThickness
		{
			get { return Internal_GethaloThickness(mCachedPtr); }
			set { Internal_SethaloThickness(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the minimal threshold of pixel luminance to be included for halo generation. Any pixel with luminance 
		/// below this value will be ignored for the purposes of halo generation.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float HaloThreshold
		{
			get { return Internal_GethaloThreshold(mCachedPtr); }
			set { Internal_SethaloThreshold(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the shape of the halo. Set to value other than 1 to make the halo an oval rather than a circle.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 2f, false)]
		[NativeWrapper]
		public float HaloAspectRatio
		{
			get { return Internal_GethaloAspectRatio(mCachedPtr); }
			set { Internal_SethaloAspectRatio(mCachedPtr, value); }
		}

		/// <summary>
		/// Enables or disables chromatic aberration of the lens flare and halo features. Chromatic aberration separates the 
		/// values of red, green and blue channels according to a user provided offset.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool ChromaticAberration
		{
			get { return Internal_GetchromaticAberration(mCachedPtr); }
			set { Internal_SetchromaticAberration(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the distance between pixels within which to sample different channels. The value is in UV coordinates, 
		/// range [0, 1].
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float ChromaticAberrationOffset
		{
			get { return Internal_GetchromaticAberrationOffset(mCachedPtr); }
			set { Internal_SetchromaticAberrationOffset(mCachedPtr, value); }
		}

		/// <summary>
		/// Uses a higher quality upscaling when blending the lens flare features with scene color. Results in less blocky 
		/// artifacts at a cost to performance.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool BicubicUpsampling
		{
			get { return Internal_GetbicubicUpsampling(mCachedPtr); }
			set { Internal_SetbicubicUpsampling(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ScreenSpaceLensFlareSettings(ScreenSpaceLensFlareSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getenabled(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setenabled(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetdownsampleCount(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetdownsampleCount(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getthreshold(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setthreshold(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetghostCount(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetghostCount(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetghostSpacing(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetghostSpacing(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getbrightness(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setbrightness(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilterSize(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilterSize(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Gethalo(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Sethalo(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GethaloRadius(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SethaloRadius(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GethaloThickness(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SethaloThickness(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GethaloThreshold(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SethaloThreshold(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GethaloAspectRatio(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SethaloAspectRatio(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetchromaticAberration(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetchromaticAberration(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetchromaticAberrationOffset(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetchromaticAberrationOffset(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetbicubicUpsampling(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetbicubicUpsampling(IntPtr thisPtr, bool value);
	}

	/** @} */
}
