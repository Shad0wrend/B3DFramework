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

	/// <summary>Settings used for controlling particle system GPU simulation.</summary>
	[ShowInInspector]
	public partial class ParticleGpuSimulationSettings : ScriptObject
	{
		private ParticleGpuSimulationSettings(bool __dummy0) { }
		protected ParticleGpuSimulationSettings() { }

		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ParticleVectorFieldSettings VectorField
		{
			get { return Internal_GetvectorField(mCachedPtr); }
			set { Internal_SetvectorField(mCachedPtr, value); }
		}

		/// <summary>Determines particle color, evaluated over the particle lifetime.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ColorDistribution ColorOverLifetime
		{
			get { return Internal_GetcolorOverLifetime(mCachedPtr); }
			set { Internal_SetcolorOverLifetime(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines particle size, evaluated over the particle lifetime. Multiplied by the initial particle size.
		/// </summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public Vector2Distribution SizeScaleOverLifetime
		{
			get { return Internal_GetsizeScaleOverLifetime(mCachedPtr); }
			set { Internal_SetsizeScaleOverLifetime(mCachedPtr, value); }
		}

		/// <summary>Constant acceleration to apply for each step of the simulation.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public Vector3 Acceleration
		{
			get
			{
				Vector3 temp;
				Internal_Getacceleration(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_Setacceleration(mCachedPtr, ref value); }
		}

		/// <summary>Amount of resistance to apply in the direction opposite of the particle&apos;s velocity.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Drag
		{
			get { return Internal_Getdrag(mCachedPtr); }
			set { Internal_Setdrag(mCachedPtr, value); }
		}

		/// <summary>Settings controlling particle depth buffer collisions.</summary>
		[ShowInInspector]
		[NotNull]
		[PassByCopy]
		[NativeWrapper]
		public ParticleDepthCollisionSettings DepthCollision
		{
			get { return Internal_GetdepthCollision(mCachedPtr); }
			set { Internal_SetdepthCollision(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleVectorFieldSettings Internal_GetvectorField(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetvectorField(IntPtr thisPtr, ParticleVectorFieldSettings value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ColorDistribution Internal_GetcolorOverLifetime(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcolorOverLifetime(IntPtr thisPtr, ColorDistribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern Vector2Distribution Internal_GetsizeScaleOverLifetime(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetsizeScaleOverLifetime(IntPtr thisPtr, Vector2Distribution value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Getacceleration(IntPtr thisPtr, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setacceleration(IntPtr thisPtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getdrag(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setdrag(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleDepthCollisionSettings Internal_GetdepthCollision(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetdepthCollision(IntPtr thisPtr, ParticleDepthCollisionSettings value);
	}

	/** @} */
}
