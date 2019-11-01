#pragma once
#include "FBXInterface.h"
#include "FBXNodeSkeleton.h"
#include "FBXNodeMesh.h"

class FBXSceneContent : public IFBXSceneData
{
private:
	FBXNodeSkeleton skeletonLoader;
	FBXNodeMesh meshLoader;
public:
	FBXSceneContent(FBXData& fbxData)
		: IFBXSceneData(fbxData), skeletonLoader(fbxData), meshLoader(fbxData) {}

	void Load(FbxScene* pScene);
private:
	//----------------------------------------
	// Helper function used in Load
	//----------------------------------------
	// Initiating functions
	bool LoadMarker(FbxNode* pRootNode);
	bool LoadSkeleton(FbxNode* pRootNode);
	bool LoadMesh(FbxNode* pRootNode);
	bool LoadNurbs(FbxNode* pRootNode);
	bool LoadPatch(FbxNode* pRootNode);
	bool LoadCamera(FbxNode* pRootNode);
	bool LoadLight(FbxNode* pRootNode);
	bool LoadLODGroup(FbxNode* pRootNode);

	// Recursive functions
	void ProcessMarker(FbxNode* pNode);
	void ProcessSkeleton(FbxNode* pNode);
	void ProcessMesh(FbxNode* pNode);
	void ProcessNurbs(FbxNode* pNode);
	void ProcessPatch(FbxNode* pNode);
	void ProcessCamera(FbxNode* pNode);
	void ProcessLight(FbxNode* pNode);
	void ProcessLODGroup(FbxNode* pNode);
};
