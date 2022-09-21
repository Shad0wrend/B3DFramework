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

	/// <summary>Generic settings used for controlling a ParticleSystem.</summary>
	[ShowInInspector]
	public partial class ParticleSystemSettings : ScriptObject
	{
		private ParticleSystemSettings(bool __dummy0) { }
		protected ParticleSystemSettings() { }

		/// <summary>Material to render the particles with.</summary>
		[ShowInInspector]
		[LoadOnAssign]
		[NativeWrapper]
		public RRef<Material> Material
		{
			get { return Internal_Getmaterial(mCachedPtr); }
			set { Internal_Setmaterial(mCachedPtr, value); }
		}

		/// <summary>Mesh used for representing individual particles when using the Mesh rendering mode.</summary>
		[ShowInInspector]
		[Order(2)]
		[LoadOnAssign]
		[NativeWrapper]
		public RRef<Mesh> Mesh
		{
			get { return Internal_Getmesh(mCachedPtr); }
			set { Internal_Setmesh(mCachedPtr, value); }
		}

		/// <summary>Determines in which space are particles in.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public ParticleSimulationSpace SimulationSpace
		{
			get { return Internal_GetsimulationSpace(mCachedPtr); }
			set { Internal_SetsimulationSpace(mCachedPtr, value); }
		}

		/// <summary>Determines how are particles oriented when rendering.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public ParticleOrientation Orientation
		{
			get { return Internal_Getorientation(mCachedPtr); }
			set { Internal_Setorientation(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the time period during which the system runs, in seconds. This effects evaluation of distributions with 
		/// curves using particle system time for evaluation.
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public float Duration
		{
			get { return Internal_Getduration(mCachedPtr); }
			set { Internal_Setduration(mCachedPtr, value); }
		}

		/// <summary>Determines should the particle system time wrap around once it reaches its duration.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool IsLooping
		{
			get { return Internal_GetisLooping(mCachedPtr); }
			set { Internal_SetisLooping(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the maximum number of particles that can ever be active in this system. This number is ignored if GPU 
		/// simulation is enabled, and instead particle count is instead only limited by the size of the internal buffers (shared 
		/// between all particle systems).
		/// </summary>
		[ShowInInspector]
		[NativeWrapper]
		public int MaxParticles
		{
			get { return Internal_GetmaxParticles(mCachedPtr); }
			set { Internal_SetmaxParticles(mCachedPtr, value); }
		}

		/// <summary>
		/// If true the particle system will be simulated on the GPU. This allows much higher particle counts at lower 
		/// performance cost. GPU simulation ignores any provided evolvers and instead uses ParticleGpuSimulationSettings to 
		/// customize the GPU simulation.
		/// </summary>
		[ShowInInspector]
		[Order(1)]
		[Category("Advanced")]
		[NativeWrapper]
		public bool GpuSimulation
		{
			get { return Internal_GetgpuSimulation(mCachedPtr); }
			set { Internal_SetgpuSimulation(mCachedPtr, value); }
		}

		/// <summary>Determines how is each particle represented on the screen.</summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public ParticleRenderMode RenderMode
		{
			get { return Internal_GetrenderMode(mCachedPtr); }
			set { Internal_SetrenderMode(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the particles only be allowed to orient themselves around the Y axis, or freely. Ignored if using 
		/// the Plane orientation mode.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public bool OrientationLockY
		{
			get { return Internal_GetorientationLockY(mCachedPtr); }
			set { Internal_SetorientationLockY(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines a normal of the plane to orient particles towards. Only used if particle orientation mode is set to 
		/// ParticleOrientation::Plane.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public Vector3 OrientationPlaneNormal
		{
			get
			{
				Vector3 temp;
				Internal_GetorientationPlaneNormal(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetorientationPlaneNormal(mCachedPtr, ref value); }
		}

		/// <summary>
		/// Determines how (and if) are particles sorted. Sorting controls in what order are particles rendered. If GPU 
		/// simulation is enabled only distance based sorting is supported.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public ParticleSortMode SortMode
		{
			get { return Internal_GetsortMode(mCachedPtr); }
			set { Internal_SetsortMode(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should an automatic seed be used for the internal random number generator. This ensures the particle 
		/// system yields different results each time it is ran.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public bool UseAutomaticSeed
		{
			get { return Internal_GetuseAutomaticSeed(mCachedPtr); }
			set { Internal_SetuseAutomaticSeed(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines the seed to use for the internal random number generator. Allows you to guarantee identical behaviour 
		/// between different runs. Only relevant if automatic seed is disabled.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public int ManualSeed
		{
			get { return Internal_GetmanualSeed(mCachedPtr); }
			set { Internal_SetmanualSeed(mCachedPtr, value); }
		}

		/// <summary>
		/// Determines should the particle system bounds be automatically calculated, or should the fixed value provided be used. 
		/// Bounds are used primarily for culling purposes. Note that automatic bounds are not supported when GPU simulation is 
		/// enabled.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public bool UseAutomaticBounds
		{
			get { return Internal_GetuseAutomaticBounds(mCachedPtr); }
			set { Internal_SetuseAutomaticBounds(mCachedPtr, value); }
		}

		/// <summary>
		/// Custom bounds to use them <see cref="useAutomaticBounds"/> is disabled. The bounds are in the simulation space of the 
		/// particle system.
		/// </summary>
		[ShowInInspector]
		[Order(2)]
		[NativeWrapper]
		public AABox CustomBounds
		{
			get
			{
				AABox temp;
				Internal_GetcustomBounds(mCachedPtr, out temp);
				return temp;
			}
			set { Internal_SetcustomBounds(mCachedPtr, ref value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<Material> Internal_Getmaterial(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setmaterial(IntPtr thisPtr, RRef<Material> value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<Mesh> Internal_Getmesh(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setmesh(IntPtr thisPtr, RRef<Mesh> value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleSimulationSpace Internal_GetsimulationSpace(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetsimulationSpace(IntPtr thisPtr, ParticleSimulationSpace value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleOrientation Internal_Getorientation(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setorientation(IntPtr thisPtr, ParticleOrientation value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern float Internal_Getduration(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Setduration(IntPtr thisPtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetisLooping(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetisLooping(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetmaxParticles(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmaxParticles(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetgpuSimulation(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetgpuSimulation(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleRenderMode Internal_GetrenderMode(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetrenderMode(IntPtr thisPtr, ParticleRenderMode value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetorientationLockY(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetorientationLockY(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetorientationPlaneNormal(IntPtr thisPtr, out Vector3 __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetorientationPlaneNormal(IntPtr thisPtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ParticleSortMode Internal_GetsortMode(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetsortMode(IntPtr thisPtr, ParticleSortMode value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetuseAutomaticSeed(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetuseAutomaticSeed(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern int Internal_GetmanualSeed(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetmanualSeed(IntPtr thisPtr, int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GetuseAutomaticBounds(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetuseAutomaticBounds(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_GetcustomBounds(IntPtr thisPtr, out AABox __output);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetcustomBounds(IntPtr thisPtr, ref AABox value);
	}

	/** @} */
}
