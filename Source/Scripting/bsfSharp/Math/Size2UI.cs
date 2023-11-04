//************************************ bs::framework - Copyright 2023 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.InteropServices;

namespace bs
{
    /** @addtogroup Math
     *  @{
     */

    /// <summary>
    /// A two dimensional size.
    /// </summary>
    [StructLayout(LayoutKind.Sequential), SerializeObject]
    public struct Size2UI // Note: Must match C++ structSize2UI 
    {
        public uint Width;
        public uint Height;

        public static readonly Size2UI Zero = new Size2UI(0, 0);
        public static readonly Size2UI One = new Size2UI(1, 1);

        /// <summary>
        /// Creates a new default initialized size value.
        /// </summary>
        public static Size2UI Default()
        {
            return new Size2UI();
        }

        /// <summary>
        /// Creates a new two dimensional size.
        /// </summary>
        /// <param name="width">Width</param>
        /// <param name="height">Height</param>
        public Size2UI(uint width, uint height)
        {
            this.Width = width;
            this.Height = height;
        }

        public static bool operator ==(Size2UI lhs, Size2UI rhs)
        {
            return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
        }

        public static bool operator !=(Size2UI lhs, Size2UI rhs)
        {
            return !(lhs == rhs);
        }

        /// <inheritdoc/>
        public override int GetHashCode()
        {
            return Width.GetHashCode() ^ Height.GetHashCode() << 2;
        }

        /// <inheritdoc/>
        public override bool Equals(object other)
        {
            if (!(other is Size2UI))
                return false;

            Size2UI size = (Size2UI)other;
            if (Width.Equals(size.Width) && Height.Equals(size.Height))
                return true;

            return false;
        }

        /// <inheritdoc/>
        public override string ToString()
        {
            return "(" + Width + ", " + Height + ")";
        }
    }

    /** @} */
}
