#pragma once
#include "FBXInterface.h"

class FBXSceneHierarchy : public IFBXSceneData
{
public:
	FBXSceneHierarchy(FBXData& fbxData)
		: IFBXSceneData(fbxData) {}

	void Load(FbxScene* pScene);
private:
	//----------------------------------------
	// Helper function used in Load
	//----------------------------------------
	// Initiating functions
	bool LoadHierarchy(FbxNode* pRootNode);

	// Recursive functions
	void ProcessHierarchy(FbxNode* pNode, int depth);
};
