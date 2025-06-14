//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Physics
	 *  @{
	 */

	/// <summary>Information describing a mesh collider shape.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct MeshColliderShapeInformation
	{
		public MeshColliderShapeInformation(RRef<PhysicsMesh> mesh = null)
		{
			this.Mesh = mesh;
		}

		public RRef<PhysicsMesh> Mesh;
	}

	/** @} */
}
