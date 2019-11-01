#include "FBXExporter.h"
#include "FBXUtility.h"
#include "../FBXBinary/FBXBinaryFileIO.h"
#include "../FBXBinary/FileStructures.h"
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pFbxManager->GetIOSettings()))
#endif

void FBXExporter::Initialize()
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pFbxManager = FbxManager::Create();
	if (!pFbxManager)
	{
		FBXUtility::DisplayString("Error: Unable to create FBX Manager!");
		exit(1);
	}
	else
		FBXUtility::DisplayString("Autodesk FBX SDK version: ", pFbxManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString path = FbxGetApplicationDirectory();
	pFbxManager->LoadPluginsDirectory(path.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pFbxScene = FbxScene::Create(pFbxManager, "My Scene");
	if (!pFbxScene)
	{
		FBXUtility::DisplayString("Error: Unable to create FBX scene!");
		exit(1);
	}
}

void FBXExporter::ShutDown()
{
	if (pFbxManager) { pFbxManager->Destroy(); }
}

bool FBXExporter::LoadScene(const char* pFilename)
{
	int fileMajor, fileMinor, fileRevision;
	int SDKMajor, SDKMinor, SDKRevision;
	int animStackCount;
	bool status;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(SDKMajor, SDKMinor, SDKRevision);

	// Create an importer.
	FbxImporter* importer = FbxImporter::Create(pFbxManager, "");

	// Initialize the importer by providing a filename.
	const bool importStatus = importer->Initialize(pFilename, -1, pFbxManager->GetIOSettings());
	importer->GetFileVersion(fileMajor, fileMinor, fileRevision);

	if (!importStatus)
	{
		FbxString error = importer->GetStatus().GetErrorString();
		FBXUtility::DisplayString("Call to FbxImporter::Initialize() failed.");
		FBXUtility::DisplayString("Error returned: %s\n\n", error.Buffer());
		if (importer->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", SDKMajor, SDKMinor, SDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, fileMajor, fileMinor, fileRevision);
		}
		return false;
	}
	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", SDKMajor, SDKMinor, SDKRevision);
	if (importer->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file: '%s' is %d.%d.%d\n\n", pFilename, fileMajor, fileMinor, fileRevision);
		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
	// Import the scene.
	status = importer->Import(pFbxScene);
	// Destroy the importer.
	importer->Destroy();
	fbxData.fbxMeshes.resize(1);

	FbxAxisSystem axisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	if (axisSystem == FbxAxisSystem::eOpenGL)
	{
		FBXUtility::DisplayString("OpenGL/Maya");
		FBXUtility::DisplayString("Coordinate System: Left Handed");
		FBXUtility::DisplayString("X-Vector: Right (1, 0, 0)");
		FBXUtility::DisplayString("Y-Vector: Up (0, 1, 0)");
		FBXUtility::DisplayString("Z-Vector: Out (1, 0, 0)");
	}
	else if (axisSystem == FbxAxisSystem::eDirectX)
	{
		FBXUtility::DisplayString("DirectX");
		FBXUtility::DisplayString("Coordinate System: Right Handed");
		FBXUtility::DisplayString("X-Vector: Left (-1, 0, 0)");
		FBXUtility::DisplayString("Y-Vector: Up (0, 1, 0)");
		FBXUtility::DisplayString("Z-Vector: In (-1, 0, 0)");
	}
	FbxGeometryConverter converter(pFbxManager);
	if (converter.Triangulate(pFbxScene, false))
	{
		FBXUtility::DisplayString("---------------------------------");
		FBXUtility::DisplayString("|\tScene Triangulated\t|");
		FBXUtility::DisplayString("---------------------------------");
		FBXUtility::DisplayString("");
	}
	return status;
}

void FBXExporter::LoadSceneInfo()
{
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	FBXUtility::DisplayString("|\t\t\t\t\t\tScene Info\t\t\t\t\t\t\t\t|");
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	infoLoader.Load(pFbxScene);
	FBXUtility::DisplayString("");
}

void FBXExporter::LoadGlobalLightSettings()
{
	const FbxGlobalSettings& settings = pFbxScene->GetGlobalSettings();
	FBXUtility::DisplayColor("Ambient Light Color: ", settings.GetAmbientColor());
}

void FBXExporter::LoadGlobalCameraSettings()
{
	const FbxGlobalSettings& settings = pFbxScene->GetGlobalSettings();
	FBXUtility::DisplayString("Default Camera: ", settings.GetDefaultCamera());
}

void FBXExporter::LoadGlobalTimeSettings()
{
	const FbxGlobalSettings& settings = pFbxScene->GetGlobalSettings();
	char string[256];
	FBXUtility::DisplayString("Time Mode : ", FbxGetTimeModeName(settings.GetTimeMode()));

	FbxTimeSpan timeSpan;
	FbxTime start, end;
	settings.GetTimelineDefaultTimeSpan(timeSpan);
	start = timeSpan.GetStart();
	end = timeSpan.GetStop();
	FBXUtility::DisplayString("Timeline default timespan: ");
	FBXUtility::DisplayString("\tStart: ", start.GetTimeString(string, FbxUShort(256)));
	FBXUtility::DisplayString("\tStop: ", end.GetTimeString(string, FbxUShort(256)));
	FBXUtility::DisplayString("");
}

void FBXExporter::LoadSceneHierarchy()
{
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	FBXUtility::DisplayString("|\t\t\t\t\t\tScene Hierarchy\t\t\t\t\t\t\t\t|");
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	hierarchyLoader.Load(pFbxScene);
	FBXUtility::DisplayString("");
}

void FBXExporter::LoadSceneContent()
{
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	FBXUtility::DisplayString("|\t\t\t\t\t\tScene Content\t\t\t\t\t\t\t\t|");
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	contentLoader.Load(pFbxScene);
	FBXUtility::DisplayString("");
}

void FBXExporter::LoadScenePose()
{
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	FBXUtility::DisplayString("|\t\t\t\t\t\tScene Pose\t\t\t\t\t\t\t\t|");
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	poseLoader.Load(pFbxScene);
	FBXUtility::DisplayString("");
}

void FBXExporter::LoadSceneAnimation()
{
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	FBXUtility::DisplayString("|\t\t\t\t\t\tScene Animation\t\t\t\t\t\t\t\t|");
	FBXUtility::DisplayString("------------------------------------------------------------------------------------------------------------------------");
	animationLoader.Load(pFbxScene);
	FBXUtility::DisplayString("");
}

void FBXExporter::SaveData(FBXBinaryFileIO& fileIO)
{
	const auto& fbxMeshes = fbxData.fbxMeshes;
	auto& fileMeshes = fileIO.meshes;
	// save mesh data
	for (size_t meshID = 0; meshID < fbxMeshes.size(); meshID++)
	{
		const FBXMesh& currentFBXMesh = fbxMeshes[meshID];
		FileMesh mesh;
		mesh.vertices = currentFBXMesh.vertices;
		mesh.indices = currentFBXMesh.indices;
		for (size_t i = 0; i < currentFBXMesh.joints.size(); i++)
		{
			FileJoint fileJoint;
			FbxMatrix matrix = currentFBXMesh.joints[i].node->EvaluateGlobalTransform();
			XMMATRIX worldMatrix;
			worldMatrix.r[0] = XMVectorSet((float)matrix.GetRow(0).mData[0], (float)matrix.GetRow(0).mData[1], (float)matrix.GetRow(0).mData[2], (float)matrix.GetRow(0).mData[3]);
			worldMatrix.r[1] = XMVectorSet((float)matrix.GetRow(1).mData[0], (float)matrix.GetRow(1).mData[1], (float)matrix.GetRow(1).mData[2], (float)matrix.GetRow(1).mData[3]);
			worldMatrix.r[2] = XMVectorSet((float)matrix.GetRow(2).mData[0], (float)matrix.GetRow(2).mData[1], (float)matrix.GetRow(2).mData[2], (float)matrix.GetRow(2).mData[3]);
			worldMatrix.r[3] = XMVectorSet((float)matrix.GetRow(3).mData[0], (float)matrix.GetRow(3).mData[1], (float)matrix.GetRow(3).mData[2], (float)matrix.GetRow(3).mData[3]);
			worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(XMConvertToRadians(180.0f)));
			XMStoreFloat4x4(&fileJoint.world, worldMatrix);
			fileJoint.parentIndex = currentFBXMesh.joints[i].parentIndex;
			mesh.joints.push_back(fileJoint);
		}
		fileIO.meshes.push_back(mesh);
	}

	const auto& fbxMaterials = fbxData.fbxMaterials;
	// save material data
	for (size_t materialID = 0; materialID < fbxMaterials.size(); materialID++)
	{
		fileIO.materials.push_back(fbxMaterials[materialID]);
	}

	// save animation data
	const auto& fbxAnimationClip = fbxData.fbxAnimationClip;
	fileIO.animationClip = fbxAnimationClip;

	const auto& fbxBindPoseMatrices = fbxData.inverseBindPoseMatrix;
	fileIO.inverseBindPoseMatrices = fbxBindPoseMatrices;
}
