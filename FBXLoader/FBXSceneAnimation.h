#pragma once
#include "FBXInterface.h"

class FBXSceneAnimation : public IFBXSceneData
{
public:
	FBXSceneAnimation(FBXData& fbxData)
		: IFBXSceneData(fbxData) {}

	void Load(FbxScene* pScene);
private:
	//----------------------------------------
	// Helper function used in Load
	//----------------------------------------
	void LoadAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
	void LoadAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
	void LoadAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);
	void LoadChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, bool isSwitcher);
	void LoadCurveKeys(FbxAnimCurve* pCurve);
	void LoadListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);
};
