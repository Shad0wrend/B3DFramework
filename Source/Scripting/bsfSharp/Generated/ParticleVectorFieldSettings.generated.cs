//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Particles
	 *  @{
	 */

	/// <summary>Settings used for controlling a vector field in a GPU simulated particle system.</summary>
	[ShowInInspector]
	public partial class ParticleVectorFieldSettings : ScriptObject
	{
		private ParticleVectorFieldSettings(bool __dummy0) { }
		protected ParticleVectorFieldSettings() { }

		/// <summary>Vector field resource used for influencing the particles.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public RRef<VectorField> VectorField
		{
			get { return Internal_GetvectorField(mCachedPtr); }
			set { Internal_SetvectorField(mCachedPtr, value); }
		}

		/// <summary>Intensity of the forces and velocities applied by the vector field.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Intensity
		{
			get { return Internal_Getintensity(mCachedPtr); }
			set { Internal_Setintensity(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines how closely does the particle velocity follow the vectors in the field. If set to 1 particles will be 
		/// snapped to the exact velocity of the value in the field, and if set to 0 the field will not influence particle 
		/// velocities directly.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Tightness
		{
			get { return Internal_Gettightness(mCachedPtr); }
			set { Internal_Settightness(mCachedPtr, value); }
		}

		/// <summary>
		/// Scale to apply to the vector field bounds. This is multiplied with the bounds of the vector field resource.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public Vector3 Scale
		{
			get
			{
				Vector3 temp;
				Internal_Getscale(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setscale(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Amount of to move the vector field by relative to the parent particle system. This is added to the bounds provided in 
		/// the vector field resource.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public Vector3 Offset
		{
			get
			{
				Vector3 temp;
				Internal_Getoffset(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setoffset(mCachedPtr, ref value); }
		}

		/// <summary>Initial rotation of the vector field.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public Quaternion Rotation
		{
			get
			{
				Quaternion temp;
				Internal_Getrotation(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setrotation(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Determines the amount to rotate the vector field every second, in degrees, around XYZ axis respectively. Evaluated 
		/// over the particle system lifetime.
		/// </summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public Vector3Distribution RotationRate
		{
			get { return Internal_GetrotationRate(mCachedPtr); }
			set { Internal_SetrotationRate(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the field influence particles outside of the field bounds. If true the field will be tiled 
		/// infinitely in the X direction.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool TilingX
		{
			get { return Internal_GettilingX(mCachedPtr); }
			set { Internal_SettilingX(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the field influence particles outside of the field bounds. If true the field will be tiled 
		/// infinitely in the Y direction.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool TilingY
		{
			get { return Internal_GettilingY(mCachedPtr); }
			set { Internal_SettilingY(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the field influence particles outside of the field bounds. If true the field will be tiled 
		/// infinitely in the Z direction.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool TilingZ
		{
			get { return Internal_GettilingZ(mCachedPtr); }
			set { Internal_SettilingZ(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<VectorField> Internal_GetvectorField(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetvectorField(IntPtr thisPtr, RRef<VectorField> value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getintensity(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setintensity(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Gettightness(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Settightness(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getscale(IntPtr thisPtr, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setscale(IntPtr thisPtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getoffset(IntPtr thisPtr, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setoffset(IntPtr thisPtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getrotation(IntPtr thisPtr, out Quaternion __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setrotation(IntPtr thisPtr, ref Quaternion value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Vector3Distribution Internal_GetrotationRate(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetrotationRate(IntPtr thisPtr, Vector3Distribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GettilingX(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettilingX(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GettilingY(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettilingY(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GettilingZ(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SettilingZ(IntPtr thisPtr, bool value);
	}

	/** @} */
}
