//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;

namespace bs
{
    /** @addtogroup GUI_Engine
     *  @{
     */

    /// <summary>
    /// Type of scaling modes for GUI images.
    /// </summary>
    public enum GUITextureScaleMode // Note: Must match C++ enum TextureScaleMode
    {
        /// <summary>
        /// Image will stretch non-uniformly in all dimensions in order to cover the assigned area fully.
        /// </summary>
        StretchToFit,
        /// <summary>
        /// Image will scale uniformly until one dimension is aligned with the assigned area. Remaining dimension might have empty space.
        /// </summary>
        ScaleToFit,
        /// <summary>
        /// Image will scale uniformly until both dimensions are larger or aligned with the assigned area. Remaining dimension might be cropped.
        /// </summary>
        CropToFit,
        /// <summary>
        /// Image will keep its original size, but will repeat in order to fill the assigned area.
        /// </summary>
        RepeatToFit
    };

    /// <summary>
    /// A GUI element that displays a texture.
    /// </summary>
    public sealed class GUITexture : GUIElement
    {
        /// <summary>
        /// Creates a new texture element.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="scale">Scale mode to use when sizing the texture.</param>
        /// <param name="transparent">Determines should the texture be rendered with transparency active.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, GUITextureScaleMode scale, bool transparent, string style, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, scale, transparent, style, options);
        }

        /// <summary>
        /// Creates a new texture element.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="scale">Scale mode to use when sizing the texture.</param>
        /// <param name="transparent">Determines should the texture be rendered with transparency active.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, GUITextureScaleMode scale, bool transparent, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, scale, transparent, "", options);
        }

        /// <summary>
        /// Creates a new texture element. Texture will use the default StretchToFit scaling.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="transparent">Determines should the texture be rendered with transparency active.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, bool transparent, string style, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, GUITextureScaleMode.StretchToFit, transparent, style, options);
        }

        /// <summary>
        /// Creates a new texture element. Texture will use the default StretchToFit scaling.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="transparent">Determines should the texture be rendered with transparency active.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, bool transparent, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, GUITextureScaleMode.StretchToFit, transparent, "", options);
        }

        /// <summary>
        /// Creates a new texture element with transparency active.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="scale">Scale mode to use when sizing the texture.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, GUITextureScaleMode scale, string style, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, scale, true, style, options);
        }

        /// <summary>
        /// Creates a new texture element with transparency active.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="scale">Scale mode to use when sizing the texture.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, GUITextureScaleMode scale, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, scale, true, "", options);
        }

        /// <summary>
        /// Creates a new texture element with transparency active. Texture will use the default StretchToFit scaling.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="style">Optional style to use for the element. Style controls the look of the element, as well as
        ///                     default layout options. Style will be retrieved from the active GUISkin. If not specified
        ///                     default element style is used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, string style, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, GUITextureScaleMode.StretchToFit, true, style, options);
        }

        /// <summary>
        /// Creates a new texture element with transparency active. Texture will use the default StretchToFit scaling.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        /// <param name="options">Options that allow you to control how is the element  positioned and sized. This will
        ///                       override any similar options set by style.</param>
        public GUITexture(SpriteImage image, params GUIOption[] options)
        {
            Internal_CreateInstance(this, image, GUITextureScaleMode.StretchToFit, true, "", options);
        }

        /// <summary>
        /// Sets the texture to display.
        /// </summary>
        /// <param name="image">Image to display. If this is null then the image specified by the style will be used.</param>
        public void SetImage(SpriteImage image)
        {
            Internal_SetImage(mCachedPtr, image);
        }

        /// <summary>
        /// Colors the element with a specific tint.
        /// </summary>
        /// <param name="color">Tint to apply to the element.</param>
        public void SetTint(Color color)
        {
            Internal_SetTint(mCachedPtr, ref color);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_CreateInstance(GUITexture instance, SpriteImage image,
            GUITextureScaleMode scale, bool transparent, string style, GUIOption[] options);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetImage(IntPtr nativeInstance, SpriteImage image);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetTint(IntPtr nativeInstance, ref Color color);
    }

    /** @} */
}
