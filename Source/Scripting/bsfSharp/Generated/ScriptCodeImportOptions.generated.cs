//********************************* bs::framework - Copyright 2018-2019 Marko Pintera ************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace bs
{
#if !IS_B3D
	/** @addtogroup Importer
	 *  @{
	 */

	/// <summary>
	/// Contains import options you may use to control how is a file containing script source code importer.
	/// </summary>
	[ShowInInspector]
	public partial class ScriptCodeImportOptions : ImportOptions
	{
		private ScriptCodeImportOptions(bool __dummy0) { }

		/// <summary>Creates a new import options object that allows you to customize how is script code imported.</summary>
		public ScriptCodeImportOptions()
		{
			Internal_Create(this);
		}

		/// <summary>Determines whether the script is editor-only or a normal game script.</summary>
		[ShowInInspector]
		[NativeWrapper]
		public bool EditorScript
		{
			get { return Internal_GeteditorScript(mCachedPtr); }
			set { Internal_SeteditorScript(mCachedPtr, value); }
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool Internal_GeteditorScript(IntPtr thisPtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_SeteditorScript(IntPtr thisPtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void Internal_Create(ScriptCodeImportOptions managedInstance);
	}

	/** @} */
#endif
}
