#include "FBXGeometryLink.h"
#include "FBXUtility.h"

void FBXGeometryLink::Load(FbxGeometry* pGeometry)
{
	int deformerCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);
	FBXUtility::DisplayInt("Deformer Count: ", deformerCount);
	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		FBXUtility::DisplayInt("Deformer[", deformerIndex, "]");
		int clusterCount = ((FbxSkin*)pGeometry->GetDeformer(deformerIndex, FbxDeformer::eSkin))->GetClusterCount();
		FBXUtility::DisplayInt("\tCluster Count: ", clusterCount);
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			FBXUtility::DisplayInt("\tCluster[", clusterIndex, "]");
			FbxCluster* currentCluster = ((FbxSkin*)pGeometry->GetDeformer(deformerIndex, FbxDeformer::eSkin))->GetCluster(clusterIndex);
			if (currentCluster->GetLink())
				FBXUtility::DisplayString("\t\tName: ", currentCluster->GetLink()->GetName());

			FbxNode* node = nullptr;
			int jointIndex = 0;
			for (int i = 0; i < fbxData.fbxMeshes[fbxData.currentMeshID].joints.size(); i++)
			{
				if (fbxData.fbxMeshes[fbxData.currentMeshID].joints[i].node == currentCluster->GetLink())
				{
					jointIndex = i;
					break;
				}
			}
			FbxString linkString = "\t\tLink Indices: ";
			FbxString weightString = "\t\tWeight Values: ";
			int controlPointIndexCount = currentCluster->GetControlPointIndicesCount();
			int* controlPointIndices = currentCluster->GetControlPointIndices();
			double* controlPointWeights = currentCluster->GetControlPointWeights();
			for (int i = 0; i < controlPointIndexCount; ++i)
			{
				Influence influence;
				influence.joint = jointIndex;
				influence.weight = (float)controlPointWeights[i];
				int controlPointIndex = controlPointIndices[i];
				fbxData.fbxControlPointInfluences[controlPointIndex].push_back(influence);
				std::sort(fbxData.fbxControlPointInfluences[controlPointIndex].begin(), fbxData.fbxControlPointInfluences[controlPointIndex].end(), 
					[&](const Influence& x, const Influence& y) {return x.weight > y.weight; });
				fbxData.fbxControlPointInfluences[controlPointIndex].resize(4);
				linkString += controlPointIndices[i];
				weightString += (float)controlPointWeights[i];
				if (i < controlPointIndexCount - 1)
				{
					linkString += ", ";
					weightString += ", ";
				}
			}
			FBXUtility::DisplayString(linkString);
			FBXUtility::DisplayString(weightString);

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			transformMatrix = currentCluster->GetTransformMatrix(transformMatrix);
			FBXUtility::Display3DVector("\t\tTransform Translation: ", transformMatrix.GetT());
			FBXUtility::Display3DVector("\t\tTransform Rotation: ", transformMatrix.GetR());
			FBXUtility::Display3DVector("\t\tTransform Scaling: ", transformMatrix.GetS());
			transformLinkMatrix = currentCluster->GetTransformLinkMatrix(transformLinkMatrix);
			FBXUtility::Display3DVector("\t\tTransform Link Translation: ", transformLinkMatrix.GetT());
			FBXUtility::Display3DVector("\t\tTransform Link Rotation: ", transformLinkMatrix.GetR());
			FBXUtility::Display3DVector("\t\tTransform Link Scaling: ", transformLinkMatrix.GetS());

			FbxAMatrix bindPoseInverse = transformLinkMatrix.Inverse() * transformMatrix;
			XMMATRIX worldMatrix;
			worldMatrix.r[0] = XMVectorSet((float)bindPoseInverse.GetRow(0).mData[0], (float)bindPoseInverse.GetRow(0).mData[1], (float)bindPoseInverse.GetRow(0).mData[2], (float)bindPoseInverse.GetRow(0).mData[3]);
			worldMatrix.r[1] = XMVectorSet((float)bindPoseInverse.GetRow(1).mData[0], (float)bindPoseInverse.GetRow(1).mData[1], (float)bindPoseInverse.GetRow(1).mData[2], (float)bindPoseInverse.GetRow(1).mData[3]);
			worldMatrix.r[2] = XMVectorSet((float)bindPoseInverse.GetRow(2).mData[0], (float)bindPoseInverse.GetRow(2).mData[1], (float)bindPoseInverse.GetRow(2).mData[2], (float)bindPoseInverse.GetRow(2).mData[3]);
			worldMatrix.r[3] = XMVectorSet((float)bindPoseInverse.GetRow(3).mData[0], (float)bindPoseInverse.GetRow(3).mData[1], (float)bindPoseInverse.GetRow(3).mData[2], (float)bindPoseInverse.GetRow(3).mData[3]);
			XMFLOAT4X4 inverseBind;
			XMStoreFloat4x4(&inverseBind, worldMatrix);
			fbxData.inverseBindPoseMatrix.push_back(inverseBind);
			if (currentCluster->GetAssociateModel())
			{
				transformMatrix = currentCluster->GetTransformAssociateModelMatrix(transformMatrix);
				FBXUtility::DisplayString("\t\tAssociate Model: ", currentCluster->GetAssociateModel()->GetName());
				FBXUtility::Display3DVector("\t\tAssociate Model Translation: ", transformMatrix.GetT());
				FBXUtility::Display3DVector("\t\tAssociate Model Rotation: ", transformMatrix.GetR());
				FBXUtility::Display3DVector("\t\tAssociate Model Scaling: ", transformMatrix.GetS());
			}
		}
	}
}
