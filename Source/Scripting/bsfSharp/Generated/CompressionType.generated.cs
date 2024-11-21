//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/// <summary>Supported compression modes.</summary>
	public enum CompressionType
	{
		Uncompressed = 0,
		Snappy = 1,
		Default = 1
	}
}
