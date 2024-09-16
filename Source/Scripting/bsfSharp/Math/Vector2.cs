//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.InteropServices;

namespace bs
{
    /** @addtogroup Math
     *  @{
     */

    /// <summary>
    /// A two dimensional vector.
    /// </summary>
    public partial struct Vector2
    {
        public static readonly Vector2 Zero = new Vector2(0.0f, 0.0f);
        public static readonly Vector2 One = new Vector2(1.0f, 1.0f);
        public static readonly Vector2 XAxis = new Vector2(1.0f, 0.0f);
        public static readonly Vector2 YAxis = new Vector2(0.0f, 1.0f);

        /// <summary>
        /// Accesses a specific component of the vector.
        /// </summary>
        /// <param name="index">Index of the component.</param>
        /// <returns>Value of the specific component.</returns>
        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0:
                        return X;
                    case 1:
                        return Y;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index.");
                }
            }

            set
            {
                switch (index)
                {
                    case 0:
                        X = value;
                        break;
                    case 1:
                        Y = value;
                        break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index.");
                }
            }
        }

        /// <summary>
        /// Returns a normalized copy of the vector.
        /// </summary>
        public Vector2 Normalized
        {
            get
            {
                return Normalize(this);
            }
        }

        /// <summary>
        /// Returns the length of the vector.
        /// </summary>
        public float Length
        {
            get
            {
                return MathEx.Sqrt(X * X + Y * Y);
            }
        }

        /// <summary>
        /// Returns the squared length of the vector.
        /// </summary>
        public float SqrdLength
        {
            get
            {
                return (X * X + Y * Y);
            }
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator +(Vector2 v, float scalar)
        {
            return new Vector2(v.X + scalar, v.Y + scalar);
        }

        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        public static Vector2 operator -(Vector2 v)
        {
            return new Vector2(-v.X, -v.Y);
        }

        public static Vector2 operator -(Vector2 v, float scalar)
        {
            return new Vector2(v.X - scalar, v.Y - scalar);
        }

        public static Vector2 operator *(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X * b.X, a.Y * b.Y);
        }

        public static Vector2 operator *(Vector2 v, float d)
        {
            return new Vector2(v.X * d, v.Y * d);
        }

        public static Vector2 operator *(float d, Vector2 v)
        {
            return new Vector2(v.X * d, v.Y * d);
        }

        public static Vector2 operator /(Vector2 v, float d)
        {
            return new Vector2(v.X / d, v.Y / d);
        }

        public static bool operator ==(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X == rhs.X && lhs.Y == rhs.Y;
        }

        public static bool operator !=(Vector2 lhs, Vector2 rhs)
        {
            return !(lhs == rhs);
        }

        /// <summary>
        /// Scales one vector by another.
        /// </summary>
        /// <param name="a">First two dimensional vector.</param>
        /// <param name="b">Second two dimensional vector.</param>
        /// <returns>One vector scaled by another.</returns>
        public static Vector2 Scale(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X * b.X, a.Y * b.Y);
        }

        /// <summary>
        /// Normalizes the provided vector and returns the normalized copy.
        /// </summary>
        /// <param name="value">Vector to normalize.</param>
        /// <returns>Normalized copy of the vector.</returns>
        public static Vector2 Normalize(Vector2 value)
        {
            float sqrdLen = value.SqrdLength;
            if (sqrdLen > 1e-04f)
                return value * MathEx.InvSqrt(sqrdLen);

            return value;
        }

        /// <summary>
        /// Calculates the inner product of the two vectors.
        /// </summary>
        /// <param name="lhs">First two dimensional vector.</param>
        /// <param name="rhs">Second two dimensional vector.</param>
        /// <returns>Inner product between the two vectors.</returns>
        public static float Dot(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X * rhs.X + lhs.Y * rhs.Y;
        }

        /// <summary>
        /// Calculates the cross product of the two vectors.
        /// </summary>
        /// <param name="lhs">First two dimensional vector.</param>
        /// <param name="rhs">Second two dimensional vector.</param>
        /// <returns>Cross product between the two vectors.</returns>
        public static float Cross(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X * rhs.Y - lhs.Y * rhs.X;
        }

        /// <summary>
        /// Calculates the distance between two points.
        /// </summary>
        /// <param name="a">First two dimensional point.</param>
        /// <param name="b">Second two dimensional point.</param>
        /// <returns>Distance between the two points.</returns>
        public static float Distance(Vector2 a, Vector2 b)
        {
            Vector2 vector2 = new Vector2(a.X - b.X, a.Y - b.Y);
            return MathEx.Sqrt(vector2.X * vector2.X + vector2.Y * vector2.Y);
        }

        /// <summary>
        /// Calculates the magnitude of the provided vector.
        /// </summary>
        /// <param name="v">Vector to calculate the magnitude for.</param>
        /// <returns>Magnitude of the vector.</returns>
        public static float Magnitude(Vector2 v)
        {
            return MathEx.Sqrt(v.X * v.X + v.Y * v.Y);
        }

        /// <summary>
        /// Calculates the squared magnitude of the provided vector.
        /// </summary>
        /// <param name="v">Vector to calculate the magnitude for.</param>
        /// <returns>Squared magnitude of the vector.</returns>
        public static float SqrMagnitude(Vector2 v)
        {
            return (v.X * v.X + v.Y * v.Y);
        }

        /// <summary>
        /// Returns the maximum of all the vector components as a new vector.
        /// </summary>
        /// <param name="a">First vector.</param>
        /// <param name="b">Second vector.</param>
        /// <returns>Vector consisting of maximum components of the first and second vector.</returns>
        public static Vector2 Max(Vector2 a, Vector2 b)
        {
            return new Vector2(MathEx.Max(a.X, b.X), MathEx.Max(a.Y, b.Y));
        }

        /// <summary>
        /// Returns the minimum of all the vector components as a new vector.
        /// </summary>
        /// <param name="a">First vector.</param>
        /// <param name="b">Second vector.</param>
        /// <returns>Vector consisting of minimum components of the first and second vector.</returns>
        public static Vector2 Min(Vector2 a, Vector2 b)
        {
            return new Vector2(MathEx.Min(a.X, b.X), MathEx.Min(a.Y, b.Y));
        }

        /// <summary>
        /// Scales the components of the vector by specified scale factors.
        /// </summary>
        /// <param name="scale">Scale factors to multiply components by.</param>
        public void Scale(Vector2 scale)
        {
            X *= scale.X;
            Y *= scale.Y;
        }

        /// <summary>
        /// Normalizes the vector.
        /// </summary>
        public void Normalize()
        {
            float sqrdLen = SqrdLength;
            if (sqrdLen > 1e-04f)
                this = this * MathEx.InvSqrt(sqrdLen);
        }

        /// <inheritdoc/>
        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode() << 2;
        }

        /// <inheritdoc/>
        public override bool Equals(object other)
        {
            if (!(other is Vector2))
                return false;

            Vector2 vec = (Vector2)other;
            if (X.Equals(vec.X) && Y.Equals(vec.Y))
                return true;

            return false;
        }

        /// <inheritdoc/>
        public override string ToString()
        {
            return "(" + X + ", " + Y + ")";
        }
    }

    /** @} */
}
