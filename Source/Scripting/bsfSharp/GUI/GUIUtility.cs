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
        public static int PhysicalToLogicalPixels(int value, float DPIScale) // DEPRECATED
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
        public static int LogicalToPhysicalPixels(int value, float DPIScale) // DEPRECATED
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
        public static Vector2I PhysicalToLogicalPixels(Vector2I value, float DPIScale) // DEPRECATED
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
        public static Vector2I LogicalToPhysicalPixels(Vector2I value, float DPIScale) // DEPRECATED
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
        public static Size2UI PhysicalToLogicalPixels(Size2UI value, float DPIScale) // DEPRECATED
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
        public static Size2UI LogicalToPhysicalPixels(Size2UI value, float DPIScale) // DEPRECATED
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
        public static Area2I PhysicalToLogicalPixels(Area2I value, float DPIScale) // DEPRECATED
        {
            // TODO - This should be returning float

            if (DPIScale == 0.0f)
                return value;

            float inverseDPIScale = 1.0f / DPIScale;
            return new Area2I((int)(value.X * inverseDPIScale), (int)(value.Y * inverseDPIScale),
                (int)(value.Width * inverseDPIScale), (int)(value.Height * inverseDPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static Area2I LogicalToPhysicalPixels(Area2I value, float DPIScale) // DEPRECATED
        {
            // TODO - This should be returning float

            return new Area2I((int)(value.X * DPIScale), (int)(value.Y * DPIScale),
                (int)(value.Width * DPIScale), (int)(value.Height * DPIScale));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static GUILogicalUnit PhysicalToLogicalPixels(GUIPhysicalUnit value, float DPIScale)
        {
            if (DPIScale == 0.0f)
                return new GUILogicalUnit((int)value);

            return new GUILogicalUnit((int)((float)value * (1.0f / DPIScale)));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static GUILogicalUnitF PhysicalToLogicalPixels(GUIPhysicalUnitF value, float DPIScale)
        {
            if (DPIScale == 0.0f)
                return new GUILogicalUnitF((float)value);

            return new GUILogicalUnitF((float)value * (1.0f / DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static GUIPhysicalUnit LogicalToPhysicalPixels(GUILogicalUnit value, float DPIScale)
        {
            return new GUIPhysicalUnit((int)((float)value * DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static GUIPhysicalUnitF LogicalToPhysicalPixels(GUILogicalUnitF value, float DPIScale)
        {
            return new GUIPhysicalUnitF((float)value * DPIScale);
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static GUILogicalPoint PhysicalToLogicalPixels(GUIPhysicalPoint value, float DPIScale)
        {
            return new GUILogicalPoint(PhysicalToLogicalPixels(value.X, DPIScale), PhysicalToLogicalPixels(value.Y, DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static GUIPhysicalPoint LogicalToPhysicalPixels(GUILogicalPoint value, float DPIScale)
        {
            return new GUIPhysicalPoint(LogicalToPhysicalPixels(value.X, DPIScale), LogicalToPhysicalPixels(value.Y, DPIScale));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static GUILogicalSize PhysicalToLogicalPixels(GUIPhysicalSize value, float DPIScale)
        {
            return new GUILogicalSize(PhysicalToLogicalPixels(value.Width, DPIScale), PhysicalToLogicalPixels(value.Height, DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static GUIPhysicalSize LogicalToPhysicalPixels(GUILogicalSize value, float DPIScale)
        {
            return new GUIPhysicalSize(LogicalToPhysicalPixels(value.Width, DPIScale), LogicalToPhysicalPixels(value.Height, DPIScale));
        }

        /// <summary>
        /// Converts a value from physical pixels to logical pixels.
        /// </summary>
        /// <param name="value">Value in physical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in logical pixels.</returns>
        public static GUILogicalArea PhysicalToLogicalPixels(GUIPhysicalArea value, float DPIScale)
        {
            return new GUILogicalArea(PhysicalToLogicalPixels(value.X, DPIScale), PhysicalToLogicalPixels(value.Y, DPIScale),
                PhysicalToLogicalPixels(value.Width, DPIScale), PhysicalToLogicalPixels(value.Height, DPIScale));
        }

        /// <summary>
        /// Converts a value from logical pixels to physical pixels.
        /// </summary>
        /// <param name="value">Value in logical pixels.</param>
        /// <param name="DPIScale">DPI scale applied to physical pixels. This is calculated as DPIScale = DPI / 96.</param>
        /// <returns>Value in physical pixels.</returns>
        public static GUIPhysicalArea LogicalToPhysicalPixels(GUILogicalArea value, float DPIScale)
        {
            return new GUIPhysicalArea(LogicalToPhysicalPixels(value.X, DPIScale), LogicalToPhysicalPixels(value.Y, DPIScale),
                LogicalToPhysicalPixels(value.Width, DPIScale), LogicalToPhysicalPixels(value.Height, DPIScale));
        }
	}

	/** @} */
}
