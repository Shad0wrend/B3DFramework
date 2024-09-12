//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;

namespace bs
{
    /** @addtogroup GUI_Engine
     *  @{
     */

    /// <summary>
    /// Vertical layout that will position its child elements top to bottom.
    /// </summary>
    public sealed class GUILayoutY : GUILayout
    {
        private GUILayoutY(bool __dummy0) { }

        /// <summary>
        /// Constructs a new empty vertical layout.
        /// </summary>
        public GUILayoutY()
        {
            Internal_Create(this, new GUIOption[0]);
        }

        /// <summary>
        /// Constructs a new empty vertical layout.
        /// </summary>
        public GUILayoutY(params GUIOption[] options)
        {
            Internal_Create(this, options);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_Create(GUILayoutY instance, GUIOption[] options);
    }

    /** @} */
}
