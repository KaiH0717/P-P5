#pragma once
#include "FBXInterface.h"

class FBXSceneInfo : public IFBXSceneData
{
private:
	int markerCount;
public:
	FBXSceneInfo(FBXData& fbxData)
		: IFBXSceneData(fbxData) {}

	void Load(FbxScene* pScene);
private:
	//----------------------------------------
	// Helper function used in Load
	//----------------------------------------
	void LoadInfo(FbxNode* pNode, int depth);
	void LoadProperties(FbxObject* pObject);
};

