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

	/// <summary>
	/// Settings that control the film grain effect. Film grains adds a time-varying noise effect over the entire image.
	/// </summary>
	[ShowInInspector]
	public partial class FilmGrainSettings : ScriptObject
	{
		private FilmGrainSettings(bool __dummy0) { }

		public FilmGrainSettings()
		{
			Internal_FilmGrainSettings(this);
		}

		/// <summary>Enables or disables the effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Enabled
		{
			get { return Internal_Getenabled(mCachedPtr); }
			set { Internal_Setenabled(mCachedPtr, value); }
		}

		/// <summary>Controls how intense are the displayed film grains.</summary>
		[ShowInInspector]
		[Range(0f, 100f, false)]
		[NativeWrapper]
		public float Intensity
		{
			get { return Internal_Getintensity(mCachedPtr); }
			set { Internal_Setintensity(mCachedPtr, value); }
		}

		/// <summary>Controls at what speed do the film grains change.</summary>
		[ShowInInspector]
		[Range(0f, 100f, false)]
		[NativeWrapper]
		public float Speed
		{
			get { return Internal_Getspeed(mCachedPtr); }
			set { Internal_Setspeed(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_FilmGrainSettings(FilmGrainSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getenabled(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setenabled(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getintensity(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setintensity(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getspeed(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setspeed(IntPtr thisPtr, float value);
	}

	/** @} */
}
