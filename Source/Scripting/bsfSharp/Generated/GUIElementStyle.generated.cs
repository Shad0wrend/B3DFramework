//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	/// <summary>
	/// GUI element style that determines the look of a GUI element, as well as the element&apos;s default layout options. 
	/// Different looks can be provided for different element states.
	/// </summary>
	[ShowInInspector]
	public partial class GUIElementStyle : ScriptObject
	{
		private GUIElementStyle(bool __dummy0) { }

		public GUIElementStyle()
		{
			Internal_GUIElementStyle(this);
		}

		/// <summary>Font to use for all text within the GUI element.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public Font Font
		{
			get { return Internal_Getfont(mCachedPtr); }
			set { Internal_Setfont(mCachedPtr, value); }
		}

		/// <summary>Font size to use for all text within the GUI element.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int FontSize
		{
			get { return Internal_GetfontSize(mCachedPtr); }
			set { Internal_SetfontSize(mCachedPtr, value); }
		}

		/// <summary>Horizontal alignment of text within the GUI element.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public TextHorzAlign TextHorzAlign
		{
			get { return Internal_GettextHorzAlign(mCachedPtr); }
			set { Internal_SettextHorzAlign(mCachedPtr, value); }
		}

		/// <summary>Vertical alignment of text within the GUI element.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public TextVertAlign TextVertAlign
		{
			get { return Internal_GettextVertAlign(mCachedPtr); }
			set { Internal_SettextVertAlign(mCachedPtr, value); }
		}

		/// <summary>Position of content image relative to text.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIImagePosition ImagePosition
		{
			get { return Internal_GetimagePosition(mCachedPtr); }
			set { Internal_SetimagePosition(mCachedPtr, value); }
		}

		/// <summary>Should the text word wrap if it doesn&apos;t fit.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool WordWrap
		{
			get { return Internal_GetwordWrap(mCachedPtr); }
			set { Internal_SetwordWrap(mCachedPtr, value); }
		}

		/// <summary>
		/// Style used when the element doesn&apos;t have focus nor is the user interacting with the element. Used when the 
		/// element is in the &apos;off&apos; state.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle Normal
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_Getnormal(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setnormal(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Style used when the user is hovering the pointer over the element, while the element doesn&apos;t have focus. Used 
		/// when the element is in the &apos;off&apos; state.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle Hover
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_Gethover(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Sethover(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Style used when the user is actively interacting with the element. Used when the element is in the &apos;off&apos; 
		/// state.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle Active
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_Getactive(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setactive(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Style used when the element has focus but the pointer is not hovering over the element. Used when the element is in 
		/// the &apos;off&apos; state.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle Focused
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_Getfocused(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setfocused(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Style used when the element has focus and the pointer is hovering over the element. Used when the element is in the 
		/// &apos;off&apos; state.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle FocusedHover
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GetfocusedHover(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetfocusedHover(mCachedPtr, ref value); }
		}

		/// <summary>Same as GUIElementStyle::normal, except it&apos;s used when element is in the &apos;on&apos; state.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle NormalOn
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GetnormalOn(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetnormalOn(mCachedPtr, ref value); }
		}

		/// <summary>Same as GUIElementStyle::hover, except it&apos;s used when element is in the &apos;on&apos; state.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle HoverOn
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GethoverOn(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SethoverOn(mCachedPtr, ref value); }
		}

		/// <summary>Same as GUIElementStyle::active, except it&apos;s used when element is in the &apos;on&apos; state.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle ActiveOn
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GetactiveOn(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetactiveOn(mCachedPtr, ref value); }
		}

		/// <summary>Same as GUIElementStyle::focused, except it&apos;s used when element is in the &apos;on&apos; state.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle FocusedOn
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GetfocusedOn(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetfocusedOn(mCachedPtr, ref value); }
		}

		/// <summary>Same as GUIElementStyle::focusedHover, except it&apos;s used when element is in the &apos;on&apos; state.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public GUIElementStateStyle FocusedHoverOn
		{
			get
			{
				GUIElementStateStyle temp;
				Internal_GetfocusedHoverOn(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetfocusedHoverOn(mCachedPtr, ref value); }
		}

		/// <summary>Determines how the element is scaled (using the typical Scale9Grid approach).</summary>
		[ShowInInspector]
		[NativeWrapper]
		public RectOffset Border
		{
			get
			{
				RectOffset temp;
				Internal_Getborder(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setborder(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Determines offset from the background graphics to the content. Input uses bounds offset by this value.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public RectOffset Margins
		{
			get
			{
				RectOffset temp;
				Internal_Getmargins(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setmargins(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Additional offset to the content, that doesn&apos;t effect the bounds. Applied on top of the margins offsets.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public RectOffset ContentOffset
		{
			get
			{
				RectOffset temp;
				Internal_GetcontentOffset(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetcontentOffset(mCachedPtr, ref value); }
		}

		/// <summary>Determines extra distance between this and other elements in a layout.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public RectOffset Padding
		{
			get
			{
				RectOffset temp;
				Internal_Getpadding(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setpadding(mCachedPtr, ref value); }
		}

		/// <summary>Wanted width of the GUI element in pixels. Only used if fixedWidth is enabled.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Width
		{
			get { return Internal_Getwidth(mCachedPtr); }
			set { Internal_Setwidth(mCachedPtr, value); }
		}

		/// <summary>Wanted height of the GUI element in pixels. Only used if fixedHeight is enabled.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public int Height
		{
			get { return Internal_Getheight(mCachedPtr); }
			set { Internal_Setheight(mCachedPtr, value); }
		}

		/// <summary>
		/// Minimum width allowed for the GUI element. Used by the layout only when exact width is not specified.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MinWidth
		{
			get { return Internal_GetminWidth(mCachedPtr); }
			set { Internal_SetminWidth(mCachedPtr, value); }
		}

		/// <summary>
		/// Maximum width allowed for the GUI element. Used by the layout only when exact width is not specified.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MaxWidth
		{
			get { return Internal_GetmaxWidth(mCachedPtr); }
			set { Internal_SetmaxWidth(mCachedPtr, value); }
		}

		/// <summary>
		/// Minimum height allowed for the GUI element. Used by the layout only when exact height is not specified.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MinHeight
		{
			get { return Internal_GetminHeight(mCachedPtr); }
			set { Internal_SetminHeight(mCachedPtr, value); }
		}

		/// <summary>
		/// Maximum height allowed for the GUI element. Used by the layout only when exact height is not specified.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MaxHeight
		{
			get { return Internal_GetmaxHeight(mCachedPtr); }
			set { Internal_SetmaxHeight(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the layout resize the element depending on available size. If true no resizing will be done.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool FixedWidth
		{
			get { return Internal_GetfixedWidth(mCachedPtr); }
			set { Internal_SetfixedWidth(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the layout resize the element depending on available size. If true no resizing will be done.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool FixedHeight
		{
			get { return Internal_GetfixedHeight(mCachedPtr); }
			set { Internal_SetfixedHeight(mCachedPtr, value); }
		}

		/// <summary>
		/// Registers a new sub-style that is used by complex GUI elements that contain one or multiple sub-elements.
		/// </summary>
		/// <param name="guiType">
		/// Name of the sub-element this style is to be used for. This depends on GUI element the style is applied to.
		/// </param>
		/// <param name="styleName">Name of the style in GUI skin to use for the sub-element.</param>
		public void AddSubStyle(string guiType, string styleName)
		{
			Internal_AddSubStyle(mCachedPtr, guiType, styleName);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GUIElementStyle(GUIElementStyle managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_AddSubStyle(IntPtr thisPtr, string guiType, string styleName);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Font Internal_Getfont(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setfont(IntPtr thisPtr, Font value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetfontSize(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfontSize(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern TextHorzAlign Internal_GettextHorzAlign(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettextHorzAlign(IntPtr thisPtr, TextHorzAlign value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern TextVertAlign Internal_GettextVertAlign(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettextVertAlign(IntPtr thisPtr, TextVertAlign value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern GUIImagePosition Internal_GetimagePosition(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetimagePosition(IntPtr thisPtr, GUIImagePosition value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetwordWrap(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetwordWrap(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getnormal(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setnormal(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Gethover(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Sethover(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getactive(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setactive(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getfocused(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setfocused(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetfocusedHover(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocusedHover(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetnormalOn(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetnormalOn(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GethoverOn(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SethoverOn(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetactiveOn(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetactiveOn(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetfocusedOn(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocusedOn(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetfocusedHoverOn(IntPtr thisPtr, out GUIElementStateStyle __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfocusedHoverOn(IntPtr thisPtr, ref GUIElementStateStyle value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getborder(IntPtr thisPtr, out RectOffset __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setborder(IntPtr thisPtr, ref RectOffset value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getmargins(IntPtr thisPtr, out RectOffset __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setmargins(IntPtr thisPtr, ref RectOffset value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetcontentOffset(IntPtr thisPtr, out RectOffset __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcontentOffset(IntPtr thisPtr, ref RectOffset value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getpadding(IntPtr thisPtr, out RectOffset __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setpadding(IntPtr thisPtr, ref RectOffset value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_Getwidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setwidth(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_Getheight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setheight(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetminWidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetminWidth(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetmaxWidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaxWidth(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetminHeight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetminHeight(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetmaxHeight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaxHeight(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetfixedWidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfixedWidth(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetfixedHeight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfixedHeight(IntPtr thisPtr, bool value);
	}

	/** @} */
}
