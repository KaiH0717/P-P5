#include "FBXSceneInfo.h"
#include "FBXUtility.h"

void FBXSceneInfo::Load(FbxScene* pScene)
{
	FbxNode* rootNode = pScene->GetRootNode();
	if (rootNode)
	{
		FbxNode* rootNode = pScene->GetRootNode();

		for (int childIndex = 0; childIndex < rootNode->GetChildCount(); ++childIndex)
		{
			LoadInfo(rootNode->GetChild(childIndex), 0);
		}

		//Other objects directly connected onto the scene
		for (int i = 0; i < pScene->GetSrcObjectCount(); ++i)
		{
			LoadProperties(pScene->GetSrcObject(i));
		}
	}
}

void FBXSceneInfo::LoadInfo(FbxNode* pNode, int depth)
{
	FbxString string;
	for (int i = 0; i < depth; i++)
	{
		string += "     ";
	}

	string += pNode->GetName();
	string += "\n";

	FBXUtility::DisplayString(string.Buffer());

	//Display generic info about that Node
	//LoadProperties(pNode);
	FBXUtility::DisplayString("");
	for (int childIndex = 0; childIndex < pNode->GetChildCount(); ++childIndex)
	{
		LoadInfo(pNode->GetChild(childIndex), depth + 1);
	}
}

void FBXSceneInfo::LoadProperties(FbxObject* pObject)
{
	FBXUtility::DisplayString("Name: ", pObject->GetName());
	// Display all the properties
	int propertyCount = 0;
	FbxProperty property = pObject->GetFirstProperty();
	while (property.IsValid())
	{
		propertyCount++;
		property = pObject->GetNextProperty(property);
	}

	FbxString string = "    Property Count: ";

	if (propertyCount == 0)
		return; // there are no properties to display

	FBXUtility::DisplayInt(string.Buffer(), propertyCount);

	int i = 0;
	property = pObject->GetFirstProperty();
	while (property.IsValid())
	{
		// exclude user properties
		FbxString string;
		FBXUtility::DisplayInt("        Property ", i);
		string = property.GetLabel();
		FBXUtility::DisplayString("            Display Name: ", string.Buffer());
		string = property.GetName();
		FBXUtility::DisplayString("            Internal Name: ", string.Buffer());
		string = property.GetPropertyDataType().GetName();
		FBXUtility::DisplayString("            Type: ", string.Buffer());
		if (property.HasMinLimit()) FBXUtility::DisplayDouble("            Min Limit: ", property.GetMinLimit());
		if (property.HasMaxLimit()) FBXUtility::DisplayDouble("            Max Limit: ", property.GetMaxLimit());
		FBXUtility::DisplayBool("            Is Animatable: ", property.GetFlag(FbxPropertyFlags::eAnimatable));

		switch (property.GetPropertyDataType().GetType())
		{
		case eFbxBool:
		{
			FBXUtility::DisplayBool("            Default Value: ", property.Get<FbxBool>());
		}
		break;
		case eFbxDouble:
		{
			FBXUtility::DisplayDouble("            Default Value: ", property.Get<FbxDouble>());
		}
		break;
		case eFbxDouble4:
		{
			FbxColor lDefault;
			char lBuf[64];

			lDefault = property.Get<FbxColor>();
			FBXSDK_sprintf(lBuf, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
			FBXUtility::DisplayString("            Default Value: ", lBuf);
		}
		break;
		case eFbxInt:
		{
			FBXUtility::DisplayInt("            Default Value: ", property.Get<FbxInt>());
		}
		break;
		case eFbxDouble3:
		{
			FbxDouble3 lDefault;
			char   lBuf[64];

			lDefault = property.Get<FbxDouble3>();
			FBXSDK_sprintf(lBuf, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
			FBXUtility::DisplayString("            Default Value: ", lBuf);
		}
		break;
		case eFbxFloat:
		{
			FBXUtility::DisplayDouble("            Default Value: ", property.Get<FbxFloat>());
		}
		break;
		case eFbxString:
		{
			string = property.Get<FbxString>();
			FBXUtility::DisplayString("            Default Value: ", string.Buffer());
		}
		break;
		default:
			FBXUtility::DisplayString("            Default Value: UNIDENTIFIED");
			break;
		}
		++i;
		property = pObject->GetNextProperty(property);
	}
}
