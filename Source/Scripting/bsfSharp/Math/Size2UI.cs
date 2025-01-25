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
    public partial struct Size2UI 
    {
        public static readonly Size2UI Zero = new Size2UI(0, 0);
        public static readonly Size2UI One = new Size2UI(1, 1);

        public static Size2UI operator +(Size2UI a, Size2UI b)
        {
            return new Size2UI(a.Width + b.Width, a.Height + b.Height);
        }

        public static Size2UI operator -(Size2UI a, Size2UI b)
        {
            int newWidth = (int)a.Width - (int)b.Width;
            int newHeight = (int)a.Height - (int)b.Height;

            return new Size2UI(Math.Max(0, newWidth), Math.Max(0, newHeight));
        }

        public static Size2UI operator *(Size2UI v, int d)
        {
            return new Size2UI(v.Width * d, v.Height * d);
        }

        public static Size2UI operator *(int d, Size2UI v)
        {
            return new Size2UI(v.Width * d, v.Height * d);
        }

        public static Size2UI operator /(Size2UI v, int d)
        {
            return new Size2UI(v.Width / d, v.Height / d);
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
