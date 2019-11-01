#include "FBXScenePose.h"
#include "FBXUtility.h"

void FBXScenePose::Load(FbxScene* pScene)
{
	FbxString  name;
	for (int i = 0; i < pScene->GetPoseCount(); ++i)
	{
		FbxPose* currentPose = pScene->GetPose(i);
		name = currentPose->GetName();
		FBXUtility::DisplayString("Pose Name: ", name.Buffer());
		FBXUtility::DisplayBool("\tIs a bind pose: ", currentPose->IsBindPose());
		FBXUtility::DisplayInt("\tNumber of items in the pose: ", currentPose->GetCount());
		FBXUtility::DisplayString("", "");
		for (int j = 0; j < currentPose->GetCount(); ++j)
		{
			name = currentPose->GetNodeName(j).GetCurrentName();
			FBXUtility::DisplayString("\tItem name: ", name.Buffer());
			if (!currentPose->IsBindPose())
				FBXUtility::DisplayBool("\tIs local space matrix: ", currentPose->IsLocalMatrix(j));
			FBXUtility::DisplayString("\tMatrix value: ", "");
			FbxString matrixValue;
			FbxMatrix matrix = currentPose->GetMatrix(j);
			for (int k = 0; k < 4; ++k)
			{
				FbxVector4 row = matrix.GetRow(k);
				char rowValueString[1024];

				FBXSDK_sprintf(rowValueString, 1024, "%9.4f %9.4f %9.4f %9.4f\n", row[0], row[1], row[2], row[3]);
				matrixValue += FbxString("        ") + FbxString(rowValueString);
			}
			FBXUtility::DisplayString("", matrixValue.Buffer());
		}
	}

	for (int i = 0; i < pScene->GetCharacterPoseCount(); ++i)
	{
		FbxCharacterPose* characterPose = pScene->GetCharacterPose(i);
		FbxCharacter* character = characterPose->GetCharacter();
		if (!character) break;
		FBXUtility::DisplayString("Character Pose Name: ", character->GetName());
		FbxCharacterLink characterLink;
		FbxCharacter::ENodeId nodeId = FbxCharacter::eHips;
		while (character->GetCharacterLink(nodeId, &characterLink))
		{
			FbxAMatrix& globalPosition = characterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);
			FBXUtility::DisplayString("    Matrix value: ", "");
			FbxString matrixValue;
			for (int j = 0; j < 4; ++j)
			{
				FbxVector4 row = globalPosition.GetRow(j);
				char rowValueString[1024];
				FBXSDK_sprintf(rowValueString, 1024, "%9.4f %9.4f %9.4f %9.4f\n", row[0], row[1], row[2], row[3]);
				matrixValue += FbxString("        ") + FbxString(rowValueString);
			}
			FBXUtility::DisplayString("", matrixValue.Buffer());
			nodeId = FbxCharacter::ENodeId((int)nodeId + 1);
		}
	}
}
