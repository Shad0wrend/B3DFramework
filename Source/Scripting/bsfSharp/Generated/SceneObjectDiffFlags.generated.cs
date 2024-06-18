//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/// <summary>Flags that mark which portion of a scene-object is modified.</summary>
	public enum SceneObjectDiffFlags
	{
		Name = 1,
		Position = 2,
		Rotation = 4,
		Scale = 8,
		Active = 16
	}
}
