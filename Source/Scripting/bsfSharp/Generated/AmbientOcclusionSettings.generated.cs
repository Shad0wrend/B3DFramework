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

	/// <summary>Settings that control screen space ambient occlusion.</summary>
	[ShowInInspector]
	public partial class AmbientOcclusionSettings : ScriptObject
	{
		private AmbientOcclusionSettings(bool __dummy0) { }

		public AmbientOcclusionSettings()
		{
			Internal_AmbientOcclusionSettings(this);
		}

		/// <summary>Enables or disables the screen space ambient occlusion effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Enabled
		{
			get { return Internal_Getenabled(mCachedPtr); }
			set { Internal_Setenabled(mCachedPtr, value); }
		}

		/// <summary>
		/// Radius (in world space, in meters) over which occluders are searched for. Smaller radius ensures better sampling 
		/// precision but can miss occluders. Larger radius ensures far away occluders are considered but can yield lower quality 
		/// or noise because of low sampling precision. Usually best to keep at around a meter, valid range is roughly [0.05, 
		/// 5.0].
		/// </summary>
		[ShowInInspector]
		[Range(0.05f, 5f, true)]
		[NativeWrapper]
		public float Radius
		{
			get { return Internal_Getradius(mCachedPtr); }
			set { Internal_Setradius(mCachedPtr, value); }
		}

		/// <summary>
		/// Bias used to reduce false occlusion artifacts. Higher values reduce the amount of artifacts but will cause details to 
		/// be lost in areas where occlusion isn&apos;t high. Value is in millimeters. Usually best to keep at a few dozen 
		/// millimeters, valid range is roughly [0, 200].
		/// </summary>
		[ShowInInspector]
		[Range(0f, 200f, true)]
		[NativeWrapper]
		public float Bias
		{
			get { return Internal_Getbias(mCachedPtr); }
			set { Internal_Setbias(mCachedPtr, value); }
		}

		/// <summary>
		/// Distance (in view space, in meters) after which AO starts fading out. The fade process will happen over the range as 
		/// specified by <see cref="fadeRange"/>.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FadeDistance
		{
			get { return Internal_GetfadeDistance(mCachedPtr); }
			set { Internal_SetfadeDistance(mCachedPtr, value); }
		}

		/// <summary>
		/// Range (in view space, in meters) in which AO fades out from 100% to 0%. AO starts fading out after the distance 
		/// specified in <see cref="fadeDistance"/>.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FadeRange
		{
			get { return Internal_GetfadeRange(mCachedPtr); }
			set { Internal_SetfadeRange(mCachedPtr, value); }
		}

		/// <summary>
		/// Linearly scales the intensity of the AO effect. Values less than 1 make the AO effect less pronounced, and vice 
		/// versa. Valid range is roughly [0.2, 2].
		/// </summary>
		[ShowInInspector]
		[Range(0.2f, 2f, true)]
		[NativeWrapper]
		public float Intensity
		{
			get { return Internal_Getintensity(mCachedPtr); }
			set { Internal_Setintensity(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls how quickly does the AO darkening effect increase with higher occlusion percent. This is a non-linear 
		/// control and will cause the darkening to ramp up exponentially. Valid range is roughly [1, 4], where 1 means no extra 
		/// darkening will occur.
		/// </summary>
		[ShowInInspector]
		[Range(1f, 4f, true)]
		[NativeWrapper]
		public float Power
		{
			get { return Internal_Getpower(mCachedPtr); }
			set { Internal_Setpower(mCachedPtr, value); }
		}

		/// <summary>
		/// Quality level of generated ambient occlusion. In range [0, 4]. Higher levels yield higher quality AO at the cost of 
		/// performance.
		/// </summary>
		[ShowInInspector]
		[Range(0f, 4f, true)]
		[NativeWrapper]
		public int Quality
		{
			get { return Internal_Getquality(mCachedPtr); }
			set { Internal_Setquality(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_AmbientOcclusionSettings(AmbientOcclusionSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getenabled(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setenabled(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getradius(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setradius(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getbias(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setbias(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfadeDistance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfadeDistance(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfadeRange(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfadeRange(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getintensity(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setintensity(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getpower(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setpower(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_Getquality(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setquality(IntPtr thisPtr, int value);
	}

	/** @} */
}
