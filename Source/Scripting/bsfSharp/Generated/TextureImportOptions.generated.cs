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

	/// <summary>Contains import options you may use to control how is a texture imported.</summary>
	[ShowInInspector]
	public partial class TextureImportOptions : ImportOptions
	{
		private TextureImportOptions(bool __dummy0) { }

		/// <summary>Creates a new import options object that allows you to customize how are textures imported.</summary>
		public TextureImportOptions()
		{
			Internal_Create(this);
		}

		/// <summary>Pixel format to import as.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public PixelFormat Format
		{
			get { return Internal_Getformat(mCachedPtr); }
			set { Internal_Setformat(mCachedPtr, value); }
		}

		/// <summary>Enables or disables mipmap generation for the texture.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool GenerateMips
		{
			get { return Internal_GetgenerateMips(mCachedPtr); }
			set { Internal_SetgenerateMips(mCachedPtr, value); }
		}

		/// <summary>
		/// Maximum mip level to generate when generating mipmaps. If 0 then maximum amount of mip levels will be generated.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MaxMip
		{
			get { return Internal_GetmaxMip(mCachedPtr); }
			set { Internal_SetmaxMip(mCachedPtr, value); }
		}

		/// <summary>Determines whether the texture data is also stored in main memory, available for fast CPU access.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool CpuCached
		{
			get { return Internal_GetcpuCached(mCachedPtr); }
			set { Internal_SetcpuCached(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines whether the texture data should be treated as if its in sRGB (gamma) space. Such texture will be converted 
		/// by hardware to linear space before use on the GPU.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool SRGB
		{
			get { return Internal_GetsRGB(mCachedPtr); }
			set { Internal_SetsRGB(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the texture be imported as a cubemap. See setCubemapSource to choose how will the source texture be 
		/// converted to a cubemap.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Cubemap
		{
			get { return Internal_Getcubemap(mCachedPtr); }
			set { Internal_Setcubemap(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines how should the source texture be interpreted when generating a cubemap. Only relevant when <see 
		/// cref="cubemap"/> is set to true.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public CubemapSourceType CubemapSourceType
		{
			get { return Internal_GetcubemapSourceType(mCachedPtr); }
			set { Internal_SetcubemapSourceType(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern PixelFormat Internal_Getformat(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setformat(IntPtr thisPtr, PixelFormat value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetgenerateMips(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetgenerateMips(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetmaxMip(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaxMip(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetcpuCached(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcpuCached(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetsRGB(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetsRGB(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getcubemap(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setcubemap(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CubemapSourceType Internal_GetcubemapSourceType(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcubemapSourceType(IntPtr thisPtr, CubemapSourceType value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create(TextureImportOptions managedInstance);
	}

	/** @} */
#endif
}
