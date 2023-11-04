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

	/// <summary>Implementation of SpriteImage that uses a user-provided texture as the atlas texture.</summary>
	[ShowInInspector]
	public partial class SpriteTexture : SpriteImage
	{
		private SpriteTexture(bool __dummy0) { }
		protected SpriteTexture() { }

		/// <summary>Creates a new sprite texture that references the entire area of the provided texture.</summary>
		public SpriteTexture(RRef<Texture> texture)
		{
			Internal_Create(this, texture);
		}

		/// <summary>Creates a new sprite texture that references a sub-area of the provided texture.</summary>
		public SpriteTexture(SpriteTextureCreateInformation createInformation)
		{
			Internal_Create0(this, ref createInformation);
		}

		/// <summary>Returns a reference wrapper for this resource.</summary>
		public RRef<SpriteTexture> Ref
		{
			get { return Internal_GetRef(mCachedPtr); }
		}

		/// <summary>Returns a reference wrapper for this resource.</summary>
		public static implicit operator RRef<SpriteTexture>(SpriteTexture x)
		{
			if(x != null)
				return Internal_GetRef(x.mCachedPtr);
			else
				return null;
		}

		/// <summary>Sets the atlas texture to utilize.</summary>
		public void SetAtlasTexture(RRef<Texture> texture)
		{
			Internal_SetAtlasTexture(mCachedPtr, texture);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern RRef<SpriteTexture> Internal_GetRef(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SetAtlasTexture(IntPtr thisPtr, RRef<Texture> texture);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create(SpriteTexture managedInstance, RRef<Texture> texture);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create0(SpriteTexture managedInstance, ref SpriteTextureCreateInformation createInformation);
	}

	/** @} */
}
