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
	/// A spherical joint removes all translational degrees of freedom but allows all rotational degrees of freedom. 
	/// Essentially this ensures that the anchor points of the two bodies are always coincident. Bodies are allowed to rotate 
	/// around the anchor points, and their rotatation can be limited by an elliptical cone.
	/// </summary>
	[ShowInInspector]
	public partial class SphericalJoint : Joint
	{
		private SphericalJoint(bool __dummy0) { }
		protected SphericalJoint() { }

		[ShowInInspector]
		[NativeWrapper]
		public LimitConeRange Limit
		{
			get
			{
				LimitConeRange temp;
				Internal_GetLimit(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetLimit(mCachedPtr, ref value); }
		}

		public void SetFlag(SphericalJointFlag flag, bool enabled)
		{
			Internal_SetFlag(mCachedPtr, flag, enabled);
		}

		public bool HasFlag(SphericalJointFlag flag)
		{
			return Internal_HasFlag(mCachedPtr, flag);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetLimit(IntPtr thisPtr, out LimitConeRange __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetLimit(IntPtr thisPtr, ref LimitConeRange limit);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetFlag(IntPtr thisPtr, SphericalJointFlag flag, bool enabled);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_HasFlag(IntPtr thisPtr, SphericalJointFlag flag);
	}

	/** @} */
}
