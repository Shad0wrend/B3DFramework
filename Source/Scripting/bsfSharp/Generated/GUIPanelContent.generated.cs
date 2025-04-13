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

	/// <summary>Structure describing contents of a GUIPanel element.</summary>
	[StructLayout(LayoutKind.Sequential), SerializeObject]
	public partial struct GUIPanelContent
	{
		public GUIPanelContent(short depth = 0, ushort depthRangeMinimum = 65535, ushort depthRangeMaximum = 65535)
		{
			this.Depth = depth;
			this.DepthRangeMinimum = depthRangeMinimum;
			this.DepthRangeMaximum = depthRangeMaximum;
		}

		/// <summary>
		/// Determines rendering order of the GUI panel. Panels with lower depth will be rendered in front of panels with higher 
		/// depth. Provided depth is relative to depth of the parent GUI panel (if any).
		/// </summary>
		public short Depth;
		/// <summary>
		/// Minimum range of depths that children of this GUI panel can have. If any panel has depth outside of the range [depth 
		/// - depthRangeMin, depth + depthRangeMax] it will be clamped to nearest extreme. Value of 65535 means infinite range.
		/// </summary>
		public ushort DepthRangeMinimum;
		/// <summary>
		/// Maximum range of depths that children of this GUI panel can have. If any panel has depth outside of the range [depth 
		/// - depthRangeMin, depth + depthRangeMax] it will be clamped to nearest extreme. Value of 65535 means infinite range.
		/// </summary>
		public ushort DepthRangeMaximum;
	}

	/** @} */
}
