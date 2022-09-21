//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
#if !IS_B3D
	/** @addtogroup Text
	 *  @{
	 */

	/// <summary>Import options that allow you to control how is a font imported.</summary>
	[ShowInInspector]
	public partial class FontImportOptions : ImportOptions
	{
		private FontImportOptions(bool __dummy0) { }

		/// <summary>Creates a new import options object that allows you to customize how are fonts imported.</summary>
		public FontImportOptions()
		{
			Internal_Create(this);
		}

		/// <summary>Determines font sizes that are to be imported. Sizes are in points.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int[] FontSizes
		{
			get { return Internal_GetfontSizes(mCachedPtr); }
			set { Internal_SetfontSizes(mCachedPtr, value); }
		}

		/// <summary>Determines character index ranges to import. Ranges are defined as unicode numbers.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public CharRange[] CharIndexRanges
		{
			get { return Internal_GetcharIndexRanges(mCachedPtr); }
			set { Internal_SetcharIndexRanges(mCachedPtr, value); }
		}

		/// <summary>Determines dots per inch scale that will be used when rendering the characters.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Dpi
		{
			get { return Internal_Getdpi(mCachedPtr); }
			set { Internal_Setdpi(mCachedPtr, value); }
		}

		/// <summary>Determines the render mode used for rendering the characters into a bitmap.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public FontRenderMode RenderMode
		{
			get { return Internal_GetrenderMode(mCachedPtr); }
			set { Internal_SetrenderMode(mCachedPtr, value); }
		}

		/// <summary>Determines whether the bold font style should be used when rendering.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Bold
		{
			get { return Internal_Getbold(mCachedPtr); }
			set { Internal_Setbold(mCachedPtr, value); }
		}

		/// <summary>Determines whether the italic font style should be used when rendering.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool Italic
		{
			get { return Internal_Getitalic(mCachedPtr); }
			set { Internal_Setitalic(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int[] Internal_GetfontSizes(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfontSizes(IntPtr thisPtr, int[] value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CharRange[] Internal_GetcharIndexRanges(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcharIndexRanges(IntPtr thisPtr, CharRange[] value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_Getdpi(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setdpi(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern FontRenderMode Internal_GetrenderMode(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetrenderMode(IntPtr thisPtr, FontRenderMode value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getbold(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setbold(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_Getitalic(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setitalic(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create(FontImportOptions managedInstance);
	}

	/** @} */
#endif
}
