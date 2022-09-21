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
	/// Base class for all Joint types. Joints constrain how two rigidbodies move relative to one another (for example a door 
	/// hinge). One of the bodies in the joint must always be movable (non-kinematic).
	/// </summary>
	[ShowInInspector]
	public partial class Joint : Component
	{
		private Joint(bool __dummy0) { }
		protected Joint() { }

		[ShowInInspector]
		[NativeWrapper]
		public float BreakForce
		{
			get { return Internal_GetBreakForce(mCachedPtr); }
			set { Internal_SetBreakForce(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public float BreakTorque
		{
			get { return Internal_GetBreakTorque(mCachedPtr); }
			set { Internal_SetBreakTorque(mCachedPtr, value); }
		}

		[ShowInInspector]
		[NativeWrapper]
		public bool EnableCollision
		{
			get { return Internal_GetEnableCollision(mCachedPtr); }
			set { Internal_SetEnableCollision(mCachedPtr, value); }
		}

		/// <summary>Triggered when the joint&apos;s break force or torque is exceeded.</summary>
		public event Action OnJointBreak;

		public Rigidbody GetBody(JointBody body)
		{
			return Internal_GetBody(mCachedPtr, body);
		}

		public void SetBody(JointBody body, Rigidbody value)
		{
			Internal_SetBody(mCachedPtr, body, value);
		}

		public Vector3 GetPosition(JointBody body)
		{
			Vector3 temp;
			Internal_GetPosition(mCachedPtr, body, out temp);
			return temp;
		}

		public Quaternion GetRotation(JointBody body)
		{
			Quaternion temp;
			Internal_GetRotation(mCachedPtr, body, out temp);
			return temp;
		}

		public void SetTransform(JointBody body, Vector3 position, Quaternion rotation)
		{
			Internal_SetTransform(mCachedPtr, body, ref position, ref rotation);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Rigidbody Internal_GetBody(IntPtr thisPtr, JointBody body);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetBody(IntPtr thisPtr, JointBody body, Rigidbody value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetPosition(IntPtr thisPtr, JointBody body, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetRotation(IntPtr thisPtr, JointBody body, out Quaternion __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetTransform(IntPtr thisPtr, JointBody body, ref Vector3 position, ref Quaternion rotation);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetBreakForce(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetBreakForce(IntPtr thisPtr, float force);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_GetBreakTorque(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetBreakTorque(IntPtr thisPtr, float torque);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetEnableCollision(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetEnableCollision(IntPtr thisPtr, bool value);
		private void Internal_onJointBreak()
		{
			OnJointBreak?.Invoke();
		}
	}

	/** @} */
}
