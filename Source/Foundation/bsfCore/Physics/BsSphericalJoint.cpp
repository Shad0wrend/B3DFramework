//************************************ B3D Framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsSphericalJoint.h"
#include "Physics/BsPhysics.h"

using namespace b3d;

SPtr<SphericalJoint> SphericalJoint::Create(PhysicsScene& scene, const SphericalJointCreateInformation& desc)
{
	return scene.CreateSphericalJoint(desc);
}
