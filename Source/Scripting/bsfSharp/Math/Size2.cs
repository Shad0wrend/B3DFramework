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
    public partial struct Size2
    {
        public static readonly Size2 Zero = new Size2(0.0f, 0.0f);
        public static readonly Size2 One = new Size2(1.0f, 1.0f);

        public static bool operator ==(Size2 lhs, Size2 rhs)
        {
            return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
        }

        public static bool operator !=(Size2 lhs, Size2 rhs)
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
            if (!(other is Size2))
                return false;

            Size2 size = (Size2)other;
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
