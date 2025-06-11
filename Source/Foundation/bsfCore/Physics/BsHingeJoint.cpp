//************************************ bs::framework - Copyright 2018 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "Physics/BsHingeJoint.h"
#include "Physics/BsPhysics.h"

using namespace bs;

SPtr<HingeJoint> HingeJoint::Create(PhysicsScene& scene, const HingeJointCreateInformation& desc)
{
	return scene.CreateHingeJoint(desc);
}
