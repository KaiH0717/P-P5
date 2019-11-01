#pragma once
#include "FBXSceneHierarchy.h"
#include "FBXSceneContent.h"
#include "FBXScenePose.h"
#include "FBXSceneAnimation.h"
#include "FBXSceneInfo.h"

//----------------------------------------
// Class for loading fbx scene and export
//----------------------------------------
class FBXBinaryFileIO;
class FBXExporter
{
private:
	// FBX SDK objects
	FbxManager* pFbxManager;
	FbxScene* pFbxScene;

	//----------------------------------------
	// FBX data
	//----------------------------------------
	FBXData fbxData;
	//----------------------------------------
	// FBX loader objects
	//----------------------------------------
	FBXSceneInfo infoLoader{ fbxData };
	FBXSceneHierarchy hierarchyLoader{ fbxData };
	FBXSceneContent contentLoader{ fbxData };
	FBXScenePose poseLoader{ fbxData };
	FBXSceneAnimation animationLoader{ fbxData };
public:
	FBXExporter() : pFbxManager(nullptr), pFbxScene(nullptr) {}

	//----------------------------------------
	// Initialize members and destroy members
	// MUST BE CALLED
	//----------------------------------------
	void Initialize();
	void ShutDown();

	//----------------------------------------
	// Load FBX scene
	//----------------------------------------
	bool LoadScene(const char* pFilename);

	//----------------------------------------
	// Loads all scene information
	//----------------------------------------
	void LoadSceneInfo();
	void LoadGlobalLightSettings();
	void LoadGlobalCameraSettings();
	void LoadGlobalTimeSettings();
	void LoadSceneHierarchy();
	void LoadSceneContent();
	void LoadScenePose();
	void LoadSceneAnimation();

	void SaveData(FBXBinaryFileIO& fileIO);
};
