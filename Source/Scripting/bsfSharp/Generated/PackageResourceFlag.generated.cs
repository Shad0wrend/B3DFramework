//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/// <summary>Flags that can be set per-resource package meta-data.</summary>
	public enum PackageResourceFlag
	{
		None = 0,
		/// <summary>Resource entry represents a folder and has no associated resource data.</summary>
		Folder = 1
	}
}
