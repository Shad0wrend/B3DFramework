//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/// <summary>Filter to use when generating mip maps.</summary>
	public enum MipMapFilter
	{
		Kaiser = 2,
		Box = 0,
		Triangle = 1
	}
}
