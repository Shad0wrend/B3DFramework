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

	/// <summary>Encapsulates width/height in a single structure.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct Size2
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static Size2 Default()
		{
			Size2 value = new Size2();
			value.Width = 0;
			value.Height = 0;

			return value;
		}

		public Size2(float width, float height)
		{
			this.Width = width;
			this.Height = height;
		}

		public float Width;
		public float Height;
	}

	/** @} */

	/** @addtogroup Math
	 *  @{
	 */

	/// <summary>Encapsulates width/height in a single structure.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct Size2UI
	{
		/// <summary>Initializes the struct with default values.</summary>
		public static Size2UI Default()
		{
			Size2UI value = new Size2UI();
			value.Width = 0;
			value.Height = 0;

			return value;
		}

		public Size2UI(int width, int height)
		{
			this.Width = width;
			this.Height = height;
		}

		public int Width;
		public int Height;
	}

	/** @} */
}
