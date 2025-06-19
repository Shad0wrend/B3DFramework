//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsFixedJoint.h"
#include "Physics/BsPhysics.h"

using namespace b3d;

SPtr<FixedJoint> FixedJoint::Create(PhysicsScene& scene, const FixedJointCreateInformation& desc)
{
	return scene.CreateFixedJoint(desc);
}
