//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/// <summary>Settings that control the depth-of-field effect.</summary>
	[ShowInInspector]
	public partial class DepthOfFieldSettings : ScriptObject
	{
		private DepthOfFieldSettings(bool __dummy0) { }

		public DepthOfFieldSettings()
		{
			Internal_DepthOfFieldSettings(this);
		}

		/// <summary>Texture to use for the bokeh shape. Only relevant when using Bokeh depth of field.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public RRef<Texture> BokehShape
		{
			get { return Internal_GetbokehShape(mCachedPtr); }
			set { Internal_SetbokehShape(mCachedPtr, value); }
		}

		/// <summary>Enables or disables the depth of field effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Enabled
		{
			get { return Internal_Getenabled(mCachedPtr); }
			set { Internal_Setenabled(mCachedPtr, value); }
		}

		/// <summary>Type of depth of field effect to use.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public DepthOfFieldType Type
		{
			get { return Internal_Gettype(mCachedPtr); }
			set { Internal_Settype(mCachedPtr, value); }
		}

		/// <summary>
		/// Distance from the camera at which the focal plane is located in. Objects at this distance will be fully in focus. In 
		/// world units (meters).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FocalDistance
		{
			get { return Internal_GetfocalDistance(mCachedPtr); }
			set { Internal_SetfocalDistance(mCachedPtr, value); }
		}

		/// <summary>
		/// Range within which the objects remain fully in focus. This range is applied relative to the focal distance. This 
		/// parameter should usually be non-zero when using the Gaussian depth of field effect. When using other types of 
		/// depth-of-field you can set this to zero for a more physically-based effect, or keep it non-zero for more artistic 
		/// control. In world units (meters).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FocalRange
		{
			get { return Internal_GetfocalRange(mCachedPtr); }
			set { Internal_SetfocalRange(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the size of the range within which objects transition from focused to fully unfocused, at the near  plane. 
		/// Only relevant for Gaussian and Bokeh depth of field. In world units (meters).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float NearTransitionRange
		{
			get { return Internal_GetnearTransitionRange(mCachedPtr); }
			set { Internal_SetnearTransitionRange(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the size of the range within which objects transition from focused to fully unfocused, at the far  plane. 
		/// Only relevant for Gaussian and Bokeh depth of field. In world units (meters).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FarTransitionRange
		{
			get { return Internal_GetfarTransitionRange(mCachedPtr); }
			set { Internal_SetfarTransitionRange(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the amount of blur to apply to fully unfocused objects that are closer to camera than the in-focus zone. 
		/// Set to zero to disable near-field blur. Only relevant for Gaussian depth of field.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float NearBlurAmount
		{
			get { return Internal_GetnearBlurAmount(mCachedPtr); }
			set { Internal_SetnearBlurAmount(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the amount of blur to apply to fully unfocused objects that are farther away from camera than the in-focus 
		/// zone. Set to zero to disable far-field blur. Only relevant for Gaussian depth of field.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FarBlurAmount
		{
			get { return Internal_GetfarBlurAmount(mCachedPtr); }
			set { Internal_SetfarBlurAmount(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the maximum size of the blur kernel, in percent of view size. Larger values cost more performance.  Only 
		/// relevant when using Bokeh depth of field.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float MaxBokehSize
		{
			get { return Internal_GetmaxBokehSize(mCachedPtr); }
			set { Internal_SetmaxBokehSize(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the maximum color difference between surrounding pixels allowed (as a sum of all channels) before higher 
		/// fidelity sampling is triggered. Increasing this value can improve performance as less higher fidelity samples will be 
		/// required, but may decrease quality of the effect. Only relevant when using Bokeh depth of field.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 10f, false)]
		[NativeWrapper]
		public float AdaptiveColorThreshold
		{
			get { return Internal_GetadaptiveColorThreshold(mCachedPtr); }
			set { Internal_SetadaptiveColorThreshold(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the minimum circle of confusion size before higher fidelity sampling is triggered. Small values trigger 
		/// high fidelity sampling because they can otherwise produce aliasing, and they are small enough so they don&apos;t cost 
		/// much. Increasing this value can improve performance as less higher fidelity samples will be required,  but may 
		/// decrease quality of the effect. Only relevant when using Bokeh depth of field.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 1f, false)]
		[NativeWrapper]
		public float AdaptiveRadiusThreshold
		{
			get { return Internal_GetadaptiveRadiusThreshold(mCachedPtr); }
			set { Internal_SetadaptiveRadiusThreshold(mCachedPtr, value); }
		}

		/// <summary>Camera aperture size in mm. Only relevant when using Bokeh depth of field.</summary>
		[ShowInInspector]
		[Range(1f, 200f, false)]
		[NativeWrapper]
		public float ApertureSize
		{
			get { return Internal_GetapertureSize(mCachedPtr); }
			set { Internal_SetapertureSize(mCachedPtr, value); }
		}

		/// <summary>Focal length of the camera&apos;s lens (e.g. 75mm). Only relevant when using Bokeh depth of field.</summary>
		[ShowInInspector]
		[Range(1f, 500f, false)]
		[NativeWrapper]
		public float FocalLength
		{
			get { return Internal_GetfocalLength(mCachedPtr); }
			set { Internal_SetfocalLength(mCachedPtr, value); }
		}

		/// <summary>
		/// Camera sensor width and height, in mm. Used for controlling the size of the circle of confusion. Only relevant  when 
		/// using Bokeh depth of field.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public Vector2 SensorSize
		{
			get
			{
				Vector2 temp;
				Internal_GetsensorSize(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetsensorSize(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Set to true if Bokeh flare should use soft depth testing to ensure they don&apos;t render on top of foreground 
		/// geometry. This can help reduce background bleeding into the foreground, which can be especially noticeable if the 
		/// background is much brighter than the foreground. Use <see cref="occlusionDepthRange"/> to tweak the effect.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool BokehOcclusion
		{
			get { return Internal_GetbokehOcclusion(mCachedPtr); }
			set { Internal_SetbokehOcclusion(mCachedPtr, value); }
		}

		/// <summary>
		/// Range in world units over which range to fade out Bokeh flare influence. Influence of the flare will be faded out as 
		/// the depth difference between the flare&apos;s origin pixel and the destination pixel grows larger. Only relevant if 
		/// <see cref="bokehOcclusion"/> is turned on.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float OcclusionDepthRange
		{
			get { return Internal_GetocclusionDepthRange(mCachedPtr); }
			set { Internal_SetocclusionDepthRange(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_DepthOfFieldSettings(DepthOfFieldSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<Texture> Internal_GetbokehShape(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetbokehShape(IntPtr thisPtr, RRef<Texture> value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getenabled(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setenabled(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern DepthOfFieldType Internal_Gettype(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Settype(IntPtr thisPtr, DepthOfFieldType value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfocalDistance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocalDistance(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfocalRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocalRange(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetnearTransitionRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetnearTransitionRange(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfarTransitionRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfarTransitionRange(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetnearBlurAmount(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetnearBlurAmount(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfarBlurAmount(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfarBlurAmount(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetmaxBokehSize(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaxBokehSize(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetadaptiveColorThreshold(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetadaptiveColorThreshold(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetadaptiveRadiusThreshold(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetadaptiveRadiusThreshold(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetapertureSize(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetapertureSize(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfocalLength(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocalLength(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetsensorSize(IntPtr thisPtr, out Vector2 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetsensorSize(IntPtr thisPtr, ref Vector2 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetbokehOcclusion(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetbokehOcclusion(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetocclusionDepthRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetocclusionDepthRange(IntPtr thisPtr, float value);
	}
}
