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

	/// <summary>Settings that control rendering for a specific camera (view).</summary>
	[ShowInInspector]
	public partial class RenderSettings : ScriptObject
	{
		private RenderSettings(bool __dummy0) { }

		public RenderSettings()
		{
			Internal_RenderSettings(this);
		}

		/// <summary>Parameters used for customizing the gaussian depth of field effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public DepthOfFieldSettings DepthOfField
		{
			get { return Internal_GetdepthOfField(mCachedPtr); }
			set { Internal_SetdepthOfField(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the chromatic aberration effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ChromaticAberrationSettings ChromaticAberration
		{
			get { return Internal_GetchromaticAberration(mCachedPtr); }
			set { Internal_SetchromaticAberration(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should automatic exposure be applied to the HDR image. When turned on the average scene brightness will be 
		/// calculated and used to automatically expose the image to the optimal range. Use the parameters provided by 
		/// autoExposure to customize the automatic exposure effect. You may also use exposureScale to manually adjust the 
		/// automatic exposure. When automatic exposure is turned off you can use exposureScale to manually set the exposure.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableAutoExposure
		{
			get { return Internal_GetenableAutoExposure(mCachedPtr); }
			set { Internal_SetenableAutoExposure(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing automatic scene exposure.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AutoExposureSettings AutoExposure
		{
			get { return Internal_GetautoExposure(mCachedPtr); }
			set { Internal_SetautoExposure(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the image be tonemapped. Tonemapping converts an HDR image into LDR image by applying a filmic 
		/// curve to the image, simulating the effect of film cameras. Filmic curve improves image quality by tapering off lows 
		/// and highs, preventing under- and over-exposure. This is useful if an image contains both very dark and very bright 
		/// areas, in which case the global exposure parameter would leave some areas either over- or under-exposed. Use 
		/// #tonemapping to customize how tonemapping performed.
		///
		/// If this is disabled, then color grading and white balancing will not be enabled either. Only relevant for HDR images.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableTonemapping
		{
			get { return Internal_GetenableTonemapping(mCachedPtr); }
			set { Internal_SetenableTonemapping(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing tonemapping.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public TonemappingSettings Tonemapping
		{
			get { return Internal_Gettonemapping(mCachedPtr); }
			set { Internal_Settonemapping(mCachedPtr, value); }
		}

		/// <summary>
		/// Parameters used for customizing white balancing. White balancing converts a scene illuminated by a light of the 
		/// specified temperature into a scene illuminated by a standard D65 illuminant (average midday light) in order to 
		/// simulate the effects of chromatic adaptation of the human visual system.
		/// </summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public WhiteBalanceSettings WhiteBalance
		{
			get { return Internal_GetwhiteBalance(mCachedPtr); }
			set { Internal_SetwhiteBalance(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing color grading.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ColorGradingSettings ColorGrading
		{
			get { return Internal_GetcolorGrading(mCachedPtr); }
			set { Internal_SetcolorGrading(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing screen space ambient occlusion.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public AmbientOcclusionSettings AmbientOcclusion
		{
			get { return Internal_GetambientOcclusion(mCachedPtr); }
			set { Internal_SetambientOcclusion(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing screen space reflections.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ScreenSpaceReflectionsSettings ScreenSpaceReflections
		{
			get { return Internal_GetscreenSpaceReflections(mCachedPtr); }
			set { Internal_SetscreenSpaceReflections(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the bloom effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public BloomSettings Bloom
		{
			get { return Internal_Getbloom(mCachedPtr); }
			set { Internal_Setbloom(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the screen space lens flare effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ScreenSpaceLensFlareSettings ScreenSpaceLensFlare
		{
			get { return Internal_GetscreenSpaceLensFlare(mCachedPtr); }
			set { Internal_SetscreenSpaceLensFlare(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the film grain effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public FilmGrainSettings FilmGrain
		{
			get { return Internal_GetfilmGrain(mCachedPtr); }
			set { Internal_SetfilmGrain(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the motion blur effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public MotionBlurSettings MotionBlur
		{
			get { return Internal_GetmotionBlur(mCachedPtr); }
			set { Internal_SetmotionBlur(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing the temporal anti-aliasing effect.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public TemporalAASettings TemporalAA
		{
			get { return Internal_GettemporalAA(mCachedPtr); }
			set { Internal_SettemporalAA(mCachedPtr, value); }
		}

		/// <summary>Enables the fast approximate anti-aliasing effect.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableFXAA
		{
			get { return Internal_GetenableFXAA(mCachedPtr); }
			set { Internal_SetenableFXAA(mCachedPtr, value); }
		}

		/// <summary>
		/// Log2 value to scale the eye adaptation by (for example 2^0 = 1). Smaller values yield darker image, while larger 
		/// yield brighter image. Allows you to customize exposure manually, applied on top of eye adaptation exposure (if 
		/// enabled). In range [-8, 8].
		/// </summary>
		[ShowInInspector]
		[Range(-8f, 8f, true)]
		[NativeWrapper]
		public float ExposureScale
		{
			get { return Internal_GetexposureScale(mCachedPtr); }
			set { Internal_SetexposureScale(mCachedPtr, value); }
		}

		/// <summary>
		/// Gamma value to adjust the image for. Larger values result in a brighter image. When tonemapping is turned on the best 
		/// gamma curve for the output device is chosen automatically and this value can by used to merely tweak that curve. If 
		/// tonemapping is turned off this is the exact value of the gamma curve that will be applied.
		/// </summary>
		[ShowInInspector]
		[Range(1f, 3f, true)]
		[NativeWrapper]
		public float Gamma
		{
			get { return Internal_Getgamma(mCachedPtr); }
			set { Internal_Setgamma(mCachedPtr, value); }
		}

		/// <summary>
		/// High dynamic range allows light intensity to be more correctly recorded when rendering by allowing for a larger range 
		/// of values. The stored light is then converted into visible color range using exposure and a tone mapping operator.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableHDR
		{
			get { return Internal_GetenableHDR(mCachedPtr); }
			set { Internal_SetenableHDR(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines if scene objects will be lit by lights. If disabled everything will be rendered using their albedo texture 
		/// with no lighting applied.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableLighting
		{
			get { return Internal_GetenableLighting(mCachedPtr); }
			set { Internal_SetenableLighting(mCachedPtr, value); }
		}

		/// <summary>Determines if shadows cast by lights should be rendered. Only relevant if lighting is turned on.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableShadows
		{
			get { return Internal_GetenableShadows(mCachedPtr); }
			set { Internal_SetenableShadows(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines if the G-Buffer should contain per-pixel velocity information. This can be useful if you are rendering an 
		/// effect that requires this information. Note that effects such as motion blur or temporal anti-aliasing might force 
		/// the velocity buffer to be enabled regardless of this setting.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableVelocityBuffer
		{
			get { return Internal_GetenableVelocityBuffer(mCachedPtr); }
			set { Internal_SetenableVelocityBuffer(mCachedPtr, value); }
		}

		/// <summary>Parameters used for customizing shadow rendering.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ShadowSettings ShadowSettings
		{
			get { return Internal_GetshadowSettings(mCachedPtr); }
			set { Internal_SetshadowSettings(mCachedPtr, value); }
		}

		/// <summary>Determines if indirect lighting (e.g. from light probes or the sky) is rendered.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableIndirectLighting
		{
			get { return Internal_GetenableIndirectLighting(mCachedPtr); }
			set { Internal_SetenableIndirectLighting(mCachedPtr, value); }
		}

		/// <summary>
		/// Signals the renderer to only render overlays (like GUI), and not scene objects. Such rendering doesn&apos;t require 
		/// depth buffer or multi-sampled render targets and will not render any scene objects. This can improve performance and 
		/// memory usage for overlay-only views.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool OverlayOnly
		{
			get { return Internal_GetoverlayOnly(mCachedPtr); }
			set { Internal_SetoverlayOnly(mCachedPtr, value); }
		}

		/// <summary>
		/// If enabled the camera will use the skybox for rendering the background. A skybox has to be present in the scene. When 
		/// disabled the camera will use the clear color for rendering the background.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EnableSkybox
		{
			get { return Internal_GetenableSkybox(mCachedPtr); }
			set { Internal_SetenableSkybox(mCachedPtr, value); }
		}

		/// <summary>
		/// The absolute base cull-distance for objects rendered through this camera in world units. Objects will use this 
		/// distance and apply their own factor to it to determine whether they should be visible.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float CullDistance
		{
			get { return Internal_GetcullDistance(mCachedPtr); }
			set { Internal_SetcullDistance(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_RenderSettings(RenderSettings managedInstance);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern DepthOfFieldSettings Internal_GetdepthOfField(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetdepthOfField(IntPtr thisPtr, DepthOfFieldSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ChromaticAberrationSettings Internal_GetchromaticAberration(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetchromaticAberration(IntPtr thisPtr, ChromaticAberrationSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableAutoExposure(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableAutoExposure(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AutoExposureSettings Internal_GetautoExposure(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetautoExposure(IntPtr thisPtr, AutoExposureSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableTonemapping(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableTonemapping(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern TonemappingSettings Internal_Gettonemapping(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Settonemapping(IntPtr thisPtr, TonemappingSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern WhiteBalanceSettings Internal_GetwhiteBalance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetwhiteBalance(IntPtr thisPtr, WhiteBalanceSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ColorGradingSettings Internal_GetcolorGrading(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcolorGrading(IntPtr thisPtr, ColorGradingSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern AmbientOcclusionSettings Internal_GetambientOcclusion(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetambientOcclusion(IntPtr thisPtr, AmbientOcclusionSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ScreenSpaceReflectionsSettings Internal_GetscreenSpaceReflections(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetscreenSpaceReflections(IntPtr thisPtr, ScreenSpaceReflectionsSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern BloomSettings Internal_Getbloom(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setbloom(IntPtr thisPtr, BloomSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ScreenSpaceLensFlareSettings Internal_GetscreenSpaceLensFlare(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetscreenSpaceLensFlare(IntPtr thisPtr, ScreenSpaceLensFlareSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern FilmGrainSettings Internal_GetfilmGrain(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetfilmGrain(IntPtr thisPtr, FilmGrainSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern MotionBlurSettings Internal_GetmotionBlur(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmotionBlur(IntPtr thisPtr, MotionBlurSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern TemporalAASettings Internal_GettemporalAA(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettemporalAA(IntPtr thisPtr, TemporalAASettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableFXAA(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableFXAA(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetexposureScale(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetexposureScale(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getgamma(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setgamma(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableHDR(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableHDR(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableLighting(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableLighting(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableShadows(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableShadows(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableVelocityBuffer(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableVelocityBuffer(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ShadowSettings Internal_GetshadowSettings(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetshadowSettings(IntPtr thisPtr, ShadowSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableIndirectLighting(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableIndirectLighting(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetoverlayOnly(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetoverlayOnly(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetenableSkybox(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetenableSkybox(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetcullDistance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcullDistance(IntPtr thisPtr, float value);
	}

	/** @} */
}
