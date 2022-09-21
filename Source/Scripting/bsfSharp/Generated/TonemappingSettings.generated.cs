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

	/// <summary>Settings that control tonemap post-process.</summary>
	[ShowInInspector]
	public partial class TonemappingSettings : ScriptObject
	{
		private TonemappingSettings(bool __dummy0) { }

		public TonemappingSettings()
		{
			Internal_TonemappingSettings(this);
		}

		/// <summary>
		/// Controls the shoulder (upper non-linear) section of the filmic curve used for tonemapping. Mostly affects bright 
		/// areas of the image and allows you to reduce over-exposure.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveShoulderStrength
		{
			get { return Internal_GetfilmicCurveShoulderStrength(mCachedPtr); }
			set { Internal_SetfilmicCurveShoulderStrength(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls the linear (middle) section of the filmic curve used for tonemapping. Mostly affects mid-range areas of the 
		/// image.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveLinearStrength
		{
			get { return Internal_GetfilmicCurveLinearStrength(mCachedPtr); }
			set { Internal_SetfilmicCurveLinearStrength(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls the linear (middle) section of the filmic curve used for tonemapping. Mostly affects mid-range areas of the 
		/// image and allows you to control how quickly does the curve climb.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveLinearAngle
		{
			get { return Internal_GetfilmicCurveLinearAngle(mCachedPtr); }
			set { Internal_SetfilmicCurveLinearAngle(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls the toe (lower non-linear) section of the filmic curve used for tonemapping. Mostly affects dark areas of 
		/// the image and allows you to reduce under-exposure.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveToeStrength
		{
			get { return Internal_GetfilmicCurveToeStrength(mCachedPtr); }
			set { Internal_SetfilmicCurveToeStrength(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls the toe (lower non-linear) section of the filmic curve. used for tonemapping. Affects low-range.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveToeNumerator
		{
			get { return Internal_GetfilmicCurveToeNumerator(mCachedPtr); }
			set { Internal_SetfilmicCurveToeNumerator(mCachedPtr, value); }
		}

		/// <summary>
		/// Controls the toe (lower non-linear) section of the filmic curve used for tonemapping. Affects low-range.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveToeDenominator
		{
			get { return Internal_GetfilmicCurveToeDenominator(mCachedPtr); }
			set { Internal_SetfilmicCurveToeDenominator(mCachedPtr, value); }
		}

		/// <summary>Controls the white point of the filmic curve used for tonemapping. Affects the entire curve.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float FilmicCurveLinearWhitePoint
		{
			get { return Internal_GetfilmicCurveLinearWhitePoint(mCachedPtr); }
			set { Internal_SetfilmicCurveLinearWhitePoint(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_TonemappingSettings(TonemappingSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveShoulderStrength(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveShoulderStrength(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveLinearStrength(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveLinearStrength(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveLinearAngle(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveLinearAngle(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveToeStrength(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveToeStrength(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveToeNumerator(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveToeNumerator(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveToeDenominator(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveToeDenominator(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetfilmicCurveLinearWhitePoint(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmicCurveLinearWhitePoint(IntPtr thisPtr, float value);
	}

	/** @} */
}
