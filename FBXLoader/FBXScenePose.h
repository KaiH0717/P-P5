#pragma once
#include "FBXInterface.h"

class FBXScenePose : public IFBXSceneData
{
public:
	FBXScenePose(FBXData& fbxData)
		: IFBXSceneData(fbxData) {}

	void Load(FbxScene* pScene);
};
