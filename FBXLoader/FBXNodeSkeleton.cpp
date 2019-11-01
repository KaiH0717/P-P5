#include "FBXNodeSkeleton.h"
#include "FBXUtility.h"

void FBXNodeSkeleton::Load(FbxNode* pNode)
{
	FbxSkeleton* pSkeleton = pNode->GetSkeleton();
	FBXJoint fbxJoint;
	fbxJoint.node = pSkeleton->GetNode();
	if (pSkeleton->IsSkeletonRoot())
	{
		fbxJoint.parentIndex = -1;
	}
	else
	{
		for (int i = 0; i < fbxData.fbxMeshes[fbxData.currentMeshID].joints.size(); ++i)
		{
			if (fbxJoint.node->GetParent() == fbxData.fbxMeshes[fbxData.currentMeshID].joints[i].node)
			{
				fbxJoint.parentIndex = i;
				break;
			}
		}
	}
	FBXUtility::DisplayString("Skeleton Name: ", pSkeleton->GetNode()->GetName());
	FBXUtility::DisplayInt("\tSkeleton ID: ", (int)fbxData.fbxMeshes[fbxData.currentMeshID].joints.size());
	FBXUtility::DisplayInt("\tSkeleton Parent Index: ", fbxJoint.parentIndex);
	fbxData.fbxMeshes[fbxData.currentMeshID].joints.push_back(fbxJoint);
}
