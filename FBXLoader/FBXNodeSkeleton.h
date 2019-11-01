#pragma once
#include "FBXInterface.h"

class FBXNodeSkeleton : public IFBXNodeData
{
public:
	FBXNodeSkeleton(FBXData& fbxData)
		: IFBXNodeData(fbxData) {}

	void Load(FbxNode* pNode);
};
