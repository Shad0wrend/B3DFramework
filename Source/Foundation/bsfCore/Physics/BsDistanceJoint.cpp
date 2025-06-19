//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsDistanceJoint.h"
#include "Physics/BsPhysics.h"

using namespace b3d;

SPtr<DistanceJoint> DistanceJoint::Create(PhysicsScene& scene, const DistanceJointCreateInformation& desc)
{
	return scene.CreateDistanceJoint(desc);
}
