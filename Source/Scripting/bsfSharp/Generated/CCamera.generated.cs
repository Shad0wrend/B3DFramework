//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Rendering
	 *  @{
	 */

	[ShowInInspector]
	public partial class Camera : Component
	{
		private Camera(bool __dummy0) { }
		protected Camera() { }

		[ShowInInspector]
		[NativeWrapper]
		public CameraFlag Flags
		{
			get { return Internal_GetFlags(mCachedPtr); }
			set { Internal_SetFlags(mCachedPtr, value); }
		}

		[NativeWrapper]
		public Viewport Viewport
		{
			get { return Internal_GetViewport(mCachedPtr); }
		}

		[ShowInInspector]
		[Range(1f, 360f, true)]
		[Order(-1)]
		[NativeWrapper]
		public Radian FieldOfView
		{
			get
			{
				Radian temp;
				Internal_GetHorzFov(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetHorzFov(mCachedPtr, ref value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float NearClipPlane
		{
			get { return Internal_GetNearClipDistance(mCachedPtr); }
			set { Internal_SetNearClipDistance(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float FarClipPlane
		{
			get { return Internal_GetFarClipDistance(mCachedPtr); }
			set { Internal_SetFarClipDistance(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float AspectRatio
		{
			get { return Internal_GetAspectRatio(mCachedPtr); }
			set { Internal_SetAspectRatio(mCachedPtr, value); }
		}

		[NativeWrapper]
		public Matrix4 ProjMatrix
		{
			get
			{
				Matrix4 temp;
				Internal_GetProjectionMatrixRs(mCachedPtr, out temp);
				return temp;
			}
		}

		[NativeWrapper]
		public Matrix4 ViewMatrix
		{
			get
			{
				Matrix4 temp;
				Internal_GetViewMatrix(mCachedPtr, out temp);
				return temp;
			}
		}

		[ShowInInspector]
		[Order(-2)]
		[NativeWrapper]
		public ProjectionType ProjectionType
		{
			get { return Internal_GetProjectionType(mCachedPtr); }
			set { Internal_SetProjectionType(mCachedPtr, value); }
		}

		[ShowInInspector]
		[Order(-1)]
		[NativeWrapper]
		public float OrthoHeight
		{
			get { return Internal_GetOrthoWindowHeight(mCachedPtr); }
			set { Internal_SetOrthoWindowHeight(mCachedPtr, value); }
		}

		[NativeWrapper]
		public float OrthoWidth
		{
			get { return Internal_GetOrthoWindowWidth(mCachedPtr); }
			set { Internal_SetOrthoWindowWidth(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public int Priority
		{
			get { return Internal_GetPriority(mCachedPtr); }
			set { Internal_SetPriority(mCachedPtr, value); }
		}

		[ShowInInspector]
		[LayerMask]
		[NativeWrapper]
		public ulong Layers
		{
			get { return Internal_GetLayers(mCachedPtr); }
			set { Internal_SetLayers(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public int SampleCount
		{
			get { return Internal_GetMsaaCount(mCachedPtr); }
			set { Internal_SetMsaaCount(mCachedPtr, value); }
		}

		[ShowInInspector]
		[ApplyOnDirty]
		[NativeWrapper]
		public RenderSettings RenderSettings
		{
			get { return Internal_GetRenderSettings(mCachedPtr); }
			set { Internal_SetRenderSettings(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public bool Main
		{
			get { return Internal_IsMain(mCachedPtr); }
			set { Internal_SetMain(mCachedPtr, value); }
		}

		public void NotifyNeedsRedraw()
		{
			Internal_NotifyNeedsRedraw(mCachedPtr);
		}

		public TVector2<int> WorldToScreenPoint(Vector3 worldPoint)
		{
			TVector2<int> temp;
			Internal_WorldToScreenPoint(mCachedPtr, ref worldPoint, out temp);
			return temp;
		}

		public TVector2<float> WorldToNdcPoint(Vector3 worldPoint)
		{
			TVector2<float> temp;
			Internal_WorldToNdcPoint(mCachedPtr, ref worldPoint, out temp);
			return temp;
		}

		public Vector3 WorldToViewPoint(Vector3 worldPoint)
		{
			Vector3 temp;
			Internal_WorldToViewPoint(mCachedPtr, ref worldPoint, out temp);
			return temp;
		}

		public Vector3 ScreenToWorldPoint(TVector2<int> screenPoint, float depth = 0.5f)
		{
			Vector3 temp;
			Internal_ScreenToWorldPoint(mCachedPtr, ref screenPoint, depth, out temp);
			return temp;
		}

		public Vector3 ScreenToViewPoint(TVector2<int> screenPoint, float depth = 0.5f)
		{
			Vector3 temp;
			Internal_ScreenToViewPoint(mCachedPtr, ref screenPoint, depth, out temp);
			return temp;
		}

		public TVector2<float> ScreenToNdcPoint(TVector2<int> screenPoint)
		{
			TVector2<float> temp;
			Internal_ScreenToNdcPoint(mCachedPtr, ref screenPoint, out temp);
			return temp;
		}

		public Vector3 ViewToWorldPoint(Vector3 viewPoint)
		{
			Vector3 temp;
			Internal_ViewToWorldPoint(mCachedPtr, ref viewPoint, out temp);
			return temp;
		}

		public TVector2<int> ViewToScreenPoint(Vector3 viewPoint)
		{
			TVector2<int> temp;
			Internal_ViewToScreenPoint(mCachedPtr, ref viewPoint, out temp);
			return temp;
		}

		public TVector2<float> ViewToNdcPoint(Vector3 viewPoint)
		{
			TVector2<float> temp;
			Internal_ViewToNdcPoint(mCachedPtr, ref viewPoint, out temp);
			return temp;
		}

		public Vector3 NdcToWorldPoint(TVector2<float> ndcPoint, float depth = 0.5f)
		{
			Vector3 temp;
			Internal_NdcToWorldPoint(mCachedPtr, ref ndcPoint, depth, out temp);
			return temp;
		}

		public Vector3 NdcToViewPoint(TVector2<float> ndcPoint, float depth = 0.5f)
		{
			Vector3 temp;
			Internal_NdcToViewPoint(mCachedPtr, ref ndcPoint, depth, out temp);
			return temp;
		}

		public TVector2<int> NdcToScreenPoint(TVector2<float> ndcPoint)
		{
			TVector2<int> temp;
			Internal_NdcToScreenPoint(mCachedPtr, ref ndcPoint, out temp);
			return temp;
		}

		public Ray ScreenPointToRay(TVector2<int> screenPoint)
		{
			Ray temp;
			Internal_ScreenPointToRay(mCachedPtr, ref screenPoint, out temp);
			return temp;
		}

		public Vector3 ProjectPoint(Vector3 point)
		{
			Vector3 temp;
			Internal_ProjectPoint(mCachedPtr, ref point, out temp);
			return temp;
		}

		public Vector3 UnprojectPoint(Vector3 point)
		{
			Vector3 temp;
			Internal_UnprojectPoint(mCachedPtr, ref point, out temp);
			return temp;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetFlags(IntPtr thisPtr, CameraFlag flags);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CameraFlag Internal_GetFlags(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Viewport Internal_GetViewport(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetHorzFov(IntPtr thisPtr, ref Radian fovy);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetHorzFov(IntPtr thisPtr, out Radian __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetNearClipDistance(IntPtr thisPtr, float nearDist);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetNearClipDistance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetFarClipDistance(IntPtr thisPtr, float farDist);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetFarClipDistance(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetAspectRatio(IntPtr thisPtr, float ratio);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetAspectRatio(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetProjectionMatrixRs(IntPtr thisPtr, out Matrix4 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetViewMatrix(IntPtr thisPtr, out Matrix4 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetProjectionType(IntPtr thisPtr, ProjectionType pt);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ProjectionType Internal_GetProjectionType(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetOrthoWindowHeight(IntPtr thisPtr, float h);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetOrthoWindowHeight(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetOrthoWindowWidth(IntPtr thisPtr, float w);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetOrthoWindowWidth(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetPriority(IntPtr thisPtr, int priority);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetPriority(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetLayers(IntPtr thisPtr, ulong layers);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ulong Internal_GetLayers(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetMsaaCount(IntPtr thisPtr, int count);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetMsaaCount(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetRenderSettings(IntPtr thisPtr, RenderSettings settings);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RenderSettings Internal_GetRenderSettings(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_NotifyNeedsRedraw(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_WorldToScreenPoint(IntPtr thisPtr, ref Vector3 worldPoint, out TVector2<int> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_WorldToNdcPoint(IntPtr thisPtr, ref Vector3 worldPoint, out TVector2<float> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_WorldToViewPoint(IntPtr thisPtr, ref Vector3 worldPoint, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ScreenToWorldPoint(IntPtr thisPtr, ref TVector2<int> screenPoint, float depth, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ScreenToViewPoint(IntPtr thisPtr, ref TVector2<int> screenPoint, float depth, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ScreenToNdcPoint(IntPtr thisPtr, ref TVector2<int> screenPoint, out TVector2<float> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ViewToWorldPoint(IntPtr thisPtr, ref Vector3 viewPoint, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ViewToScreenPoint(IntPtr thisPtr, ref Vector3 viewPoint, out TVector2<int> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ViewToNdcPoint(IntPtr thisPtr, ref Vector3 viewPoint, out TVector2<float> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_NdcToWorldPoint(IntPtr thisPtr, ref TVector2<float> ndcPoint, float depth, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_NdcToViewPoint(IntPtr thisPtr, ref TVector2<float> ndcPoint, float depth, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_NdcToScreenPoint(IntPtr thisPtr, ref TVector2<float> ndcPoint, out TVector2<int> __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ScreenPointToRay(IntPtr thisPtr, ref TVector2<int> screenPoint, out Ray __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_ProjectPoint(IntPtr thisPtr, ref Vector3 point, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_UnprojectPoint(IntPtr thisPtr, ref Vector3 point, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetMain(IntPtr thisPtr, bool main);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_IsMain(IntPtr thisPtr);
	}

	/** @} */
}
