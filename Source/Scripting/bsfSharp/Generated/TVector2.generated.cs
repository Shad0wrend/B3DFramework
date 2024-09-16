//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup Math
	 *  @{
	 */

	/// <summary>A two dimensional vector.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct Vector2
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static Vector2 Default()
		{
			Vector2 value = new Vector2();
			value.X = 0;
			value.Y = 0;

			return value;
		}

		public Vector2(float x, float y)
		{
			this.X = x;
			this.Y = y;
		}

		public float X;
		public float Y;
	}

	/** @} */

	/** @addtogroup Math
	 *  @{
	 */

	/// <summary>A two dimensional vector.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct Vector2d
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static Vector2d Default()
		{
			Vector2d value = new Vector2d();
			value.X = 0;
			value.Y = 0;

			return value;
		}

		public Vector2d(double x, double y)
		{
			this.X = x;
			this.Y = y;
		}

		public double X;
		public double Y;
	}

	/** @} */
}
