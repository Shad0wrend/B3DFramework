//************************************ B3D Framework - Copyright 2025 Marko Pintera **************************************//
//*********** Licensed under the MIT license. See LICENSE.md for full terms. This notice is not to be removed. ***********//
#include "B3DFrameGraphDependency.h"
#include "B3DFrameGraphPass.h"

using namespace b3d;
using namespace b3d::render;

FrameGraphPassNode::FrameGraphPassNode(FrameGraphPass* pass)
	: mPass(pass)
{
}

void FrameGraphPassNode::AddIncomingDependency(const FrameGraphPassDependency& dependency)
{
	mIncomingDependencies.push_back(dependency);
}

void FrameGraphPassNode::AddOutgoingDependency(const FrameGraphPassDependency& dependency)
{
	mOutgoingDependencies.push_back(dependency);
}

void FrameGraphPassNode::Reset()
{
	mIncomingDependencies.clear();
	mOutgoingDependencies.clear();
	mReferenceCount = 0;
	mCulled = false;
}
