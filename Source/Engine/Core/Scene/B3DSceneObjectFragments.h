//************************************ B3D Framework - Copyright 2026 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#pragma once

#include "B3DPrerequisites.h"
#include "Scene/B3DTransform.h"

namespace b3d::ecs
{
	/** @addtogroup Scene-Internal
	 *  @{
	 */

	/** Local-space transform — position/rotation/scale relative to parent. */
	struct LocalTransform : Transform
	{
		LocalTransform() = default;
		explicit LocalTransform(const Transform& transform)
			: Transform(transform)
		{ }
	};

	/** World-space transform — position/rotation/scale in world coordinates. */
	struct WorldTransform : Transform
	{
		WorldTransform() = default;
		explicit WorldTransform(const Transform& transform)
			: Transform(transform)
		{ }
	};

	/** @} */
} // namespace b3d::ecs
