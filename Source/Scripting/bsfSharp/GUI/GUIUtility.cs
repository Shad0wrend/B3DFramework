//********************************* bs::framework - Copyright 2018-2022 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
	/** @addtogroup GUI
	 *  @{
	 */

	public partial class GUIUtility
	{
        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static int PhysicalToLogicalPixels(int value, float DPIScale)
        {
            // TODO - This should be returning float

            if (DPIScale == 0.0f)
                return value;

            return (int)(value * (1.0f / DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static int LogicalToPhysicalPixels(int value, float DPIScale)
        {
            // TODO - This should be returning float

            return (int)(value * DPIScale);
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static Vector2I PhysicalToLogicalPixels(Vector2I value, float DPIScale)
        {
            // TODO - This should be returning float

            if (DPIScale == 0.0f)
                return value;

            return new Vector2I((int)(value.X * (1.0f / DPIScale)), (int)(value.Y * (1.0f / DPIScale)));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static Vector2I LogicalToPhysicalPixels(Vector2I value, float DPIScale)
        {
            // TODO - This should be returning float

            return new Vector2I((int)(value.X * DPIScale), (int)(value.Y * DPIScale));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static Size2UI PhysicalToLogicalPixels(Size2UI value, float DPIScale)
        {
            // TODO - This should be returning float

            if (DPIScale == 0.0f)
                return value;

            return new Size2UI((int)(value.Width * (1.0f / DPIScale)), (int)(value.Height * (1.0f / DPIScale)));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static Size2UI LogicalToPhysicalPixels(Size2UI value, float DPIScale)
        {
            // TODO - This should be returning float

            return new Size2UI((int)(value.Width * DPIScale), (int)(value.Height * DPIScale));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static Rect2I PhysicalToLogicalPixels(Rect2I value, float DPIScale)
        {
            // TODO - This should be returning float

            if (DPIScale == 0.0f)
                return value;

            float inverseDPIScale = 1.0f / DPIScale;
            return new Rect2I((int)(value.x * inverseDPIScale), (int)(value.y * inverseDPIScale),
                (int)(value.width * inverseDPIScale), (int)(value.height * inverseDPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static Rect2I LogicalToPhysicalPixels(Rect2I value, float DPIScale)
        {
            // TODO - This should be returning float

            return new Rect2I((int)(value.x * DPIScale), (int)(value.y * DPIScale),
                (int)(value.width * DPIScale), (int)(value.height * DPIScale));
        }
	}

	/** @} */
}
