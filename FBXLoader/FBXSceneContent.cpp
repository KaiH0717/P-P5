#include "FBXSceneContent.h"
#include "FBXUtility.h"

void FBXSceneContent::Load(FbxScene* pScene)
{
	FbxNode* rootNode = pScene->GetRootNode();
	if (rootNode)
	{
		//if (LoadMarker(rootNode))
		//	FBXUtility::DisplayString("Marker Loaded");

		FBXUtility::DisplayString("---------------------------------");
		FBXUtility::DisplayString("|\tLoading Skeleton...\t|");
		FBXUtility::DisplayString("---------------------------------");
		if (LoadSkeleton(rootNode))
		{
			FBXUtility::DisplayString("---------------------------------");
			FBXUtility::DisplayString("|\tSkeleton Loaded\t\t|");
			FBXUtility::DisplayString("---------------------------------");
			FBXUtility::DisplayString("");
		}

		FBXUtility::DisplayString("---------------------------------");
		FBXUtility::DisplayString("|\tLoading Mesh...\t\t|");
		FBXUtility::DisplayString("---------------------------------");
		if (LoadMesh(rootNode))
		{
			FBXUtility::DisplayString("---------------------------------");
			FBXUtility::DisplayString("|\tMesh Loaded\t\t|");
			FBXUtility::DisplayString("---------------------------------");
			FBXUtility::DisplayString("");
		}

		//if (LoadNurbs(rootNode))
		//	FBXUtility::DisplayString("Nurbs Loaded");

		//if (LoadPatch(rootNode))
		//	FBXUtility::DisplayString("Patch Loaded");

		//if (LoadCamera(rootNode))
		//	FBXUtility::DisplayString("Camera Loaded");

		//if (LoadLight(rootNode))
		//	FBXUtility::DisplayString("Light Loaded");

		//if (LoadLODGroup(rootNode))
		//	FBXUtility::DisplayString("LODGroup Loaded");
	}
}

bool FBXSceneContent::LoadMarker(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

bool FBXSceneContent::LoadSkeleton(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;
	for (int childIndex = 0; childIndex < pRootNode->GetChildCount(); ++childIndex)
	{
		ProcessSkeleton(pRootNode->GetChild(childIndex));
	}
	return true;
}

bool FBXSceneContent::LoadMesh(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;
	for (int childIndex = 0; childIndex < pRootNode->GetChildCount(); ++childIndex)
	{
		ProcessMesh(pRootNode->GetChild(childIndex));
	}
	return true;
}

bool FBXSceneContent::LoadNurbs(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

bool FBXSceneContent::LoadPatch(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

bool FBXSceneContent::LoadCamera(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

bool FBXSceneContent::LoadLight(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

bool FBXSceneContent::LoadLODGroup(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;

	return true;
}

void FBXSceneContent::ProcessMarker(FbxNode* pNode)
{
}

void FBXSceneContent::ProcessSkeleton(FbxNode* pNode)
{
	if (pNode->GetNodeAttribute())
	{
		if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eSkeleton)
		{
			skeletonLoader.Load(pNode);
		}
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessSkeleton(pNode->GetChild(i));
	}
}

void FBXSceneContent::ProcessMesh(FbxNode* pNode)
{
	if (pNode->GetNodeAttribute())
	{
		if (pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			meshLoader.Load(pNode);
		}
	}
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessMesh(pNode->GetChild(i));
	}
}

void FBXSceneContent::ProcessNurbs(FbxNode* pNode)
{
}

void FBXSceneContent::ProcessPatch(FbxNode* pNode)
{
}

void FBXSceneContent::ProcessCamera(FbxNode* pNode)
{
}

void FBXSceneContent::ProcessLight(FbxNode* pNode)
{
}

void FBXSceneContent::ProcessLODGroup(FbxNode* pNode)
{
}
