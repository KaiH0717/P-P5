#include "FBXSceneAnimation.h"
#include "FBXUtility.h"

void FBXSceneAnimation::Load(FbxScene* pScene)
{
	int animationCount = pScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); ++i)
	{
		FbxAnimStack* animStack = pScene->GetSrcObject<FbxAnimStack>(i);
		FbxString outputString = "Animation Stack Name: ";
		outputString += animStack->GetName();
		outputString += "\n";
		FBXSDK_printf(outputString);
		LoadAnimation(animStack, pScene->GetRootNode());
	}
}

void FBXSceneAnimation::LoadAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int animLayerCount = pAnimStack->GetMemberCount<FbxAnimLayer>();
	int audioLayerCount = pAnimStack->GetMemberCount<FbxAudioLayer>();
	FbxString outputString;

	// loading animation data
	FbxTime start = pAnimStack->GetLocalTimeSpan().GetStart();
	FbxTime stop = pAnimStack->GetLocalTimeSpan().GetStop();
	FbxTime duration = stop - start;
	fbxData.fbxAnimationClip.duration = duration.GetSecondDouble();
	fbxData.fbxAnimationClip.frameCount = duration.GetFrameCount(FbxTime::eFrames24);
	for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i < stop.GetFrameCount(FbxTime::eFrames24); ++i)
	{
		FileKeyFrame keyFrame;
		FbxTime currentTime;
		currentTime.SetFrame(i, FbxTime::eFrames24);
		keyFrame.time = currentTime.GetSecondDouble();
		for (int i = 0; i < fbxData.fbxMeshes[fbxData.currentMeshID].joints.size(); ++i)
		{
			FbxMatrix matrix = fbxData.fbxMeshes[fbxData.currentMeshID].joints[i].node->EvaluateGlobalTransform(currentTime);
			XMMATRIX worldMatrix;
			worldMatrix.r[0] = XMVectorSet((float)matrix.GetRow(0).mData[0], (float)matrix.GetRow(0).mData[1], (float)matrix.GetRow(0).mData[2], (float)matrix.GetRow(0).mData[3]);
			worldMatrix.r[1] = XMVectorSet((float)matrix.GetRow(1).mData[0], (float)matrix.GetRow(1).mData[1], (float)matrix.GetRow(1).mData[2], (float)matrix.GetRow(1).mData[3]);
			worldMatrix.r[2] = XMVectorSet((float)matrix.GetRow(2).mData[0], (float)matrix.GetRow(2).mData[1], (float)matrix.GetRow(2).mData[2], (float)matrix.GetRow(2).mData[3]);
			worldMatrix.r[3] = XMVectorSet((float)matrix.GetRow(3).mData[0], (float)matrix.GetRow(3).mData[1], (float)matrix.GetRow(3).mData[2], (float)matrix.GetRow(3).mData[3]);
			XMFLOAT4X4 m; XMStoreFloat4x4(&m, worldMatrix);
			keyFrame.jointTransforms.push_back(m);
		}
		fbxData.fbxAnimationClip.frames.push_back(keyFrame);
	}

	//outputString = "   contains ";
	//if (animLayerCount == 0 && audioLayerCount == 0)
	//	outputString += "No Layers";

	//if (animLayerCount)
	//{
	//	outputString += animLayerCount;
	//	outputString += " Animation Layer";
	//	if (animLayerCount > 1)
	//		outputString += "s";
	//}

	//if (audioLayerCount)
	//{
	//	if (animLayerCount)
	//		outputString += " and ";
	//	outputString += audioLayerCount;
	//	outputString += " Audio Layer";
	//	if (audioLayerCount > 1)
	//		outputString += "s";
	//}
	//outputString += "\n\n";
	//FBXSDK_printf(outputString);

	//for (int i = 0; i < animLayerCount; ++i)
	//{
	//	FbxAnimLayer* currentAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(i);
	//	outputString = "AnimLayer ";
	//	outputString += i;
	//	outputString += "\n";
	//	FBXSDK_printf(outputString);
	//	LoadAnimation(currentAnimLayer, pNode, isSwitcher);
	//}

	//for (int i = 0; i < audioLayerCount; ++i)
	//{
	//	FbxAudioLayer* currentAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(i);
	//	outputString = "AudioLayer ";
	//	outputString += i;
	//	outputString += "\n";
	//	FBXSDK_printf(outputString);
	//	LoadAnimation(currentAudioLayer, isSwitcher);
	//	FBXSDK_printf("\n");
	//}
}

void FBXSceneAnimation::LoadAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
	int modelCount;
	FbxString outputString;

	outputString = "     Node Name: ";
	outputString += pNode->GetName();
	outputString += "\n\n";
	FBXSDK_printf(outputString);

	LoadChannels(pNode, pAnimLayer, isSwitcher);
	FBXSDK_printf("\n");

	for (modelCount = 0; modelCount < pNode->GetChildCount(); modelCount++)
	{
		LoadAnimation(pAnimLayer, pNode->GetChild(modelCount), isSwitcher);
	}
}

void FBXSceneAnimation::LoadAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher)
{
	int clipCount;
	FbxString outputString;

	clipCount = pAudioLayer->GetMemberCount<FbxAudio>();
	outputString = "     Name: ";
	outputString += pAudioLayer->GetName();
	outputString += "\n\n";
	outputString += "     Nb Audio Clips: ";
	outputString += clipCount;
	outputString += "\n";
	FBXSDK_printf(outputString);

	for (int i = 0; i < clipCount; i++)
	{
		FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
		outputString = "        Clip[";
		outputString += i;
		outputString += "]:\t";
		outputString += lClip->GetName();
		outputString += "\n";
		FBXSDK_printf(outputString);
	}
}

void FBXSceneAnimation::LoadChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, bool isSwitcher)
{
	FbxAnimCurve* animCurve = nullptr;

	// Display general curves.
	if (!isSwitcher)
	{
		animCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (animCurve)
		{
			FBXSDK_printf("        TX\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (animCurve)
		{
			FBXSDK_printf("        TY\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (animCurve)
		{
			FBXSDK_printf("        TZ\n");
			LoadCurveKeys(animCurve);
		}

		animCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (animCurve)
		{
			FBXSDK_printf("        RX\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (animCurve)
		{
			FBXSDK_printf("        RY\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (animCurve)
		{
			FBXSDK_printf("        RZ\n");
			LoadCurveKeys(animCurve);
		}

		animCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (animCurve)
		{
			FBXSDK_printf("        SX\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (animCurve)
		{
			FBXSDK_printf("        SY\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (animCurve)
		{
			FBXSDK_printf("        SZ\n");
			LoadCurveKeys(animCurve);
		}
	}

	// Display curves specific to a light or marker.
	FbxNodeAttribute* nodeAttribute = pNode->GetNodeAttribute();

	if (nodeAttribute)
	{
		animCurve = nodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
		if (animCurve)
		{
			FBXSDK_printf("        Red\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = nodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		if (animCurve)
		{
			FBXSDK_printf("        Green\n");
			LoadCurveKeys(animCurve);
		}
		animCurve = nodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
		if (animCurve)
		{
			FBXSDK_printf("        Blue\n");
			LoadCurveKeys(animCurve);
		}

		// Display curves specific to a light.
		FbxLight* light = pNode->GetLight();
		if (light)
		{
			animCurve = light->Intensity.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Intensity\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = light->OuterAngle.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Outer Angle\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = light->Fog.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Fog\n");
				LoadCurveKeys(animCurve);
			}
		}

		// Display curves specific to a camera.
		FbxCamera* camera = pNode->GetCamera();
		if (camera)
		{
			animCurve = camera->FieldOfView.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Field of View\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Field of View X\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Field of View Y\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Optical Center X\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Optical Center Y\n");
				LoadCurveKeys(animCurve);
			}

			animCurve = camera->Roll.GetCurve(pAnimLayer);
			if (animCurve)
			{
				FBXSDK_printf("        Roll\n");
				LoadCurveKeys(animCurve);
			}
		}

		// Display curves specific to a geometry.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			FbxGeometry* geometry = (FbxGeometry*)nodeAttribute;
			int blendShapeDeformerCount = geometry->GetDeformerCount(FbxDeformer::eBlendShape);
			for (int blendShapeIndex = 0; blendShapeIndex < blendShapeDeformerCount; ++blendShapeIndex)
			{
				FbxBlendShape* lBlendShape = (FbxBlendShape*)geometry->GetDeformer(blendShapeIndex, FbxDeformer::eBlendShape);
				int blendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
				for (int channelIndex = 0; channelIndex < blendShapeChannelCount; ++channelIndex)
				{
					FbxBlendShapeChannel* channel = lBlendShape->GetBlendShapeChannel(channelIndex);
					const char* channelName = channel->GetName();
					animCurve = geometry->GetShapeChannel(blendShapeIndex, channelIndex, pAnimLayer, true);
					if (animCurve)
					{
						FBXSDK_printf("        Shape %s\n", channelName);
						LoadCurveKeys(animCurve);
					}
				}
			}
		}
	} // nodeAttribute

	// Display curves specific to properties
	FbxProperty property = pNode->GetFirstProperty();
	while (property.IsValid())
	{
		if (property.GetFlag(FbxPropertyFlags::eUserDefined))
		{
			FbxString fbxFCurveNodeName = property.GetName();
			FbxAnimCurveNode* curveNode = property.GetCurveNode(pAnimLayer);

			if (!curveNode) {
				property = pNode->GetNextProperty(property);
				continue;
			}

			FbxDataType dataType = property.GetPropertyDataType();
			if (dataType.GetType() == eFbxBool || dataType.GetType() == eFbxDouble || dataType.GetType() == eFbxFloat || dataType.GetType() == eFbxInt)
			{
				FbxString message;
				message = "        Property ";
				message += property.GetName();
				if (property.GetLabel().GetLen() > 0)
				{
					message += " (Label: ";
					message += property.GetLabel();
					message += ")";
				}
				FBXUtility::DisplayString(message.Buffer());
				for (int c = 0; c < curveNode->GetCurveCount(0U); ++c)
				{
					animCurve = curveNode->GetCurve(0U, c);
					if (animCurve)
						LoadCurveKeys(animCurve);
				}
			}
			else if (dataType.GetType() == eFbxDouble3 || dataType.GetType() == eFbxDouble4 || dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT))
			{
				char* componentName1 = (dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
				char* componentName2 = (dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
				char* componentName3 = (dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE : (char*)"Z";
				FbxString message;
				message = "        Property ";
				message += property.GetName();
				if (property.GetLabel().GetLen() > 0)
				{
					message += " (Label: ";
					message += property.GetLabel();
					message += ")";
				}
				FBXUtility::DisplayString(message.Buffer());
				for (int c = 0; c < curveNode->GetCurveCount(0U); ++c)
				{
					animCurve = curveNode->GetCurve(0U, c);
					if (animCurve)
					{
						FBXUtility::DisplayString("        Component ", componentName1);
						LoadCurveKeys(animCurve);
					}
				}
				for (int c = 0; c < curveNode->GetCurveCount(1U); ++c)
				{
					animCurve = curveNode->GetCurve(1U, c);
					if (animCurve)
					{
						FBXUtility::DisplayString("        Component ", componentName2);
						LoadCurveKeys(animCurve);
					}
				}
				for (int c = 0; c < curveNode->GetCurveCount(2U); ++c)
				{
					animCurve = curveNode->GetCurve(2U, c);
					if (animCurve)
					{
						FBXUtility::DisplayString("        Component ", componentName3);
						LoadCurveKeys(animCurve);
					}
				}
			}
			else if (dataType.GetType() == eFbxEnum)
			{
				FbxString message;
				message = "        Property ";
				message += property.GetName();
				if (property.GetLabel().GetLen() > 0)
				{
					message += " (Label: ";
					message += property.GetLabel();
					message += ")";
				};
				FBXUtility::DisplayString(message.Buffer());
				for (int c = 0; c < curveNode->GetCurveCount(0U); ++c)
				{
					animCurve = curveNode->GetCurve(0U, c);
					if (animCurve)
						LoadListCurveKeys(animCurve, &property);
				}
			}
		}
		property = pNode->GetNextProperty(property);
	} // while
}

void FBXSceneAnimation::LoadCurveKeys(FbxAnimCurve* pCurve)
{
	static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
	static const char* constantMode[] = { "?", "Standard", "Next" };
	static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

	FbxTime keyTime;
	float keyValue;
	char timeString[256];
	FbxString outputString;

	int keyCount = pCurve->KeyGetCount();
	for (int i = 0; i < keyCount; i++)
	{
		keyValue = static_cast<float>(pCurve->KeyGetValue(i));
		keyTime = pCurve->KeyGetTime(i);
		outputString = "            Key Time: ";
		outputString += keyTime.GetTimeString(timeString, FbxUShort(256));
		outputString += ".... Key Value: ";
		outputString += keyValue;
		outputString += " [ ";
		outputString += interpolation[FBXUtility::InterpolationFlagToIndex(pCurve->KeyGetInterpolation(i))];
		if ((pCurve->KeyGetInterpolation(i) & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			outputString += " | ";
			outputString += constantMode[FBXUtility::ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(i))];
		}
		else if ((pCurve->KeyGetInterpolation(i) & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			outputString += " | ";
			outputString += cubicMode[FBXUtility::TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(i))];
			outputString += " | ";
			outputString += tangentWVMode[FBXUtility::TangentweightFlagToIndex(pCurve->KeyGet(i).GetTangentWeightMode())];
			outputString += " | ";
			outputString += tangentWVMode[FBXUtility::TangentVelocityFlagToIndex(pCurve->KeyGet(i).GetTangentVelocityMode())];
		}
		outputString += " ]";
		outputString += "\n";
		FBXSDK_printf(outputString);
	}
}

void FBXSceneAnimation::LoadListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
	FbxTime keyTime;
	int keyValue;
	char timeString[256];
	FbxString outputString;

	int keyCount = pCurve->KeyGetCount();
	for (int i = 0; i < keyCount; ++i)
	{
		keyValue = (int)pCurve->KeyGetValue(i);
		keyTime = pCurve->KeyGetTime(i);
		outputString = "            Key Time: ";
		outputString += keyTime.GetTimeString(timeString, FbxUShort(256));
		outputString += ".... Key Value: ";
		outputString += keyValue;
		outputString += " (";
		outputString += pProperty->GetEnumValue(keyValue);
		outputString += ")";
		outputString += "\n";
		FBXSDK_printf(outputString);
	}
}
