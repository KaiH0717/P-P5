#include "FBXSceneHierarchy.h"

void FBXSceneHierarchy::Load(FbxScene* pScene)
{
	FbxNode* rootNode = pScene->GetRootNode();
	if (rootNode)
	{
		LoadHierarchy(rootNode);
	}
}

bool FBXSceneHierarchy::LoadHierarchy(FbxNode* pRootNode)
{
	if (pRootNode->GetChildCount() <= 0)
		return false;
	for (int childIndex = 0; childIndex < pRootNode->GetChildCount(); ++childIndex)
	{
		ProcessHierarchy(pRootNode->GetChild(childIndex), 0);
	}
	return true;
}

void FBXSceneHierarchy::ProcessHierarchy(FbxNode* pNode, int depth)
{
	FbxString string;
	for (int i = 0; i < depth; ++i)
	{
		string += "|     ";
	}
	string += "+-";
	string += pNode->GetName();
	string += "\n";

	FBXSDK_printf(string.Buffer());
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
	{
		ProcessHierarchy(pNode->GetChild(childIndex), depth + 1);
	}
}
