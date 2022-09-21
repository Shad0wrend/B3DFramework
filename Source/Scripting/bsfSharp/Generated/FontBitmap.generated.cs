//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup GUI_Engine
	 *  @{
	 */

	/// <summary>Contains textures and data about every character for a bitmap font of a specific size.</summary>
	[ShowInInspector]
	public partial class FontBitmap : ScriptObject
	{
		private FontBitmap(bool __dummy0) { }
		protected FontBitmap() { }

		/// <summary>Font size for which the data is contained.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Size
		{
			get { return Internal_Getsize(mCachedPtr); }
			set { Internal_Setsize(mCachedPtr, value); }
		}

		/// <summary>Y offset to the baseline on which the characters are placed, in pixels.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int BaselineOffset
		{
			get { return Internal_GetbaselineOffset(mCachedPtr); }
			set { Internal_SetbaselineOffset(mCachedPtr, value); }
		}

		/// <summary>Height of a single line of the font, in pixels.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int LineHeight
		{
			get { return Internal_GetlineHeight(mCachedPtr); }
			set { Internal_SetlineHeight(mCachedPtr, value); }
		}

		/// <summary>Character to use when data for a character is missing.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public CharDesc MissingGlyph
		{
			get
			{
				CharDesc temp;
				Internal_GetmissingGlyph(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetmissingGlyph(mCachedPtr, ref value); }
		}

		/// <summary>Width of a space in pixels.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int SpaceWidth
		{
			get { return Internal_GetspaceWidth(mCachedPtr); }
			set { Internal_SetspaceWidth(mCachedPtr, value); }
		}

		/// <summary>Textures in which the character&apos;s pixels are stored.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public RRef<Texture>[] TexturePages
		{
			get { return Internal_GettexturePages(mCachedPtr); }
			set { Internal_SettexturePages(mCachedPtr, value); }
		}

		/// <summary>Returns a character description for the character with the specified Unicode key.</summary>
		public CharDesc GetCharDesc(int charId)
		{
			CharDesc temp;
			Internal_GetCharDesc(mCachedPtr, charId, out temp);
			return temp;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetCharDesc(IntPtr thisPtr, int charId, out CharDesc __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_Getsize(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setsize(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetbaselineOffset(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetbaselineOffset(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetlineHeight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetlineHeight(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetmissingGlyph(IntPtr thisPtr, out CharDesc __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmissingGlyph(IntPtr thisPtr, ref CharDesc value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetspaceWidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetspaceWidth(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<Texture>[] Internal_GettexturePages(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettexturePages(IntPtr thisPtr, RRef<Texture>[] value);
	}

	/** @} */
}
