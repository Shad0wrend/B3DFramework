//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Physics
	 *  @{
	 */

	/// <summary>
	/// Collider represents physics geometry that can be in multiple states: - Default: Static geometry that physics objects 
	/// can collide with. - Trigger: Static geometry that can&apos;t be collided with but will report touch events. - Dynamic: 
	/// Dynamic geometry that is a part of a Rigidbody. A set of colliders defines the shape of the parent rigidbody.
	/// </summary>
	[ShowInInspector]
	public partial class Collider : Component
	{
		private Collider(bool __dummy0) { }
		protected Collider() { }

		[ShowInInspector]
		[NativeWrapper]
		public bool Trigger
		{
			get { return Internal_GetIsTrigger(mCachedPtr); }
			set { Internal_SetIsTrigger(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float Mass
		{
			get { return Internal_GetMass(mCachedPtr); }
			set { Internal_SetMass(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public RRef<PhysicsMaterial> Material
		{
			get { return Internal_GetMaterial(mCachedPtr); }
			set { Internal_SetMaterial(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float ContactOffset
		{
			get { return Internal_GetContactOffset(mCachedPtr); }
			set { Internal_SetContactOffset(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float RestOffset
		{
			get { return Internal_GetRestOffset(mCachedPtr); }
			set { Internal_SetRestOffset(mCachedPtr, value); }
		}

		[ShowInInspector]
		[LayerMask]
		[NativeWrapper]
		public ulong Layer
		{
			get { return Internal_GetLayer(mCachedPtr); }
			set { Internal_SetLayer(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public CollisionReportMode CollisionReportMode
		{
			get { return Internal_GetCollisionReportMode(mCachedPtr); }
			set { Internal_SetCollisionReportMode(mCachedPtr, value); }
		}

		/// <summary>
		/// Triggered when some object starts interacting with the collider. Only triggered if proper collision report mode is 
		/// turned on.
		/// </summary>
		public event Action<CollisionData> OnCollisionBegin;

		/// <summary>
		/// Triggered for every frame that an object remains interacting with a collider. Only triggered if proper collision 
		/// report mode is turned on.
		/// </summary>
		public event Action<CollisionData> OnCollisionStay;

		/// <summary>
		/// Triggered when some object stops interacting with the collider. Only triggered if proper collision report mode is 
		/// turned on.
		/// </summary>
		public event Action<CollisionData> OnCollisionEnd;

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetIsTrigger(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetIsTrigger(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetMass(IntPtr thisPtr, float mass);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetMass(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetMaterial(IntPtr thisPtr, RRef<PhysicsMaterial> material);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<PhysicsMaterial> Internal_GetMaterial(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetContactOffset(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetContactOffset(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetRestOffset(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetRestOffset(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetLayer(IntPtr thisPtr, ulong layer);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ulong Internal_GetLayer(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetCollisionReportMode(IntPtr thisPtr, CollisionReportMode mode);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern CollisionReportMode Internal_GetCollisionReportMode(IntPtr thisPtr);
		private void Internal_onCollisionBegin(ref CollisionData p0)
		{
			OnCollisionBegin?.Invoke(p0);
		}
		private void Internal_onCollisionStay(ref CollisionData p0)
		{
			OnCollisionStay?.Invoke(p0);
		}
		private void Internal_onCollisionEnd(ref CollisionData p0)
		{
			OnCollisionEnd?.Invoke(p0);
		}
	}

	/** @} */
}
