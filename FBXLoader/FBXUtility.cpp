#include "FBXUtility.h"

void FBXUtility::DisplayMetaDataConnections(FbxObject* pObject)
{
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
		DisplayString("    MetaData connections ");

	for (int i = 0; i < nbMetaData; ++i)
	{
		FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		DisplayString("        Name: ", (char*)metaData->GetName());
	}
}

void FBXUtility::DisplayString(const char* pHeader, const char* pBuffer, const char* pSuffix)
{
	FbxString string = pHeader;
	string += pBuffer;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::DisplayBool(const char* pHeader, bool value, const char* pSuffix)
{
	FbxString string = pHeader;
	string += value ? "True" : "False";
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::DisplayInt(const char* pHeader, int value, const char* pSuffix)
{
	FbxString string = pHeader;
	string += value;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::DisplayDouble(const char* pHeader, double value, const char* pSuffix)
{
	FbxString string = pHeader;
	FbxString floatString = (float)value;
	floatString = value <= -HUGE_VAL ? "-INFINITY" : floatString.Buffer();
	floatString = value >= HUGE_VAL ? "INFINITY" : floatString.Buffer();
	string += floatString;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::Display2DVector(const char* pHeader, FbxVector2 value, const char* pSuffix)
{
	FbxString string = pHeader;
	FbxString xFloatString = (float)value[0];
	FbxString yFloatString = (float)value[1];
	xFloatString = value[0] <= -HUGE_VAL ? "-INFINITY" : xFloatString.Buffer();
	xFloatString = value[0] >= HUGE_VAL ? "INFINITY" : xFloatString.Buffer();
	yFloatString = value[1] <= -HUGE_VAL ? "-INFINITY" : yFloatString.Buffer();
	yFloatString = value[1] >= HUGE_VAL ? "INFINITY" : yFloatString.Buffer();
	string += xFloatString;
	string += ", ";
	string += yFloatString;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::Display3DVector(const char* pHeader, FbxVector4 value, const char* pSuffix)
{
	FbxString string = pHeader;
	FbxString xFloatString = (float)value[0];
	FbxString yFloatString = (float)value[1];
	FbxString zFloatString = (float)value[2];
	xFloatString = value[0] <= -HUGE_VAL ? "-INFINITY" : xFloatString.Buffer();
	xFloatString = value[0] >= HUGE_VAL ? "INFINITY" : xFloatString.Buffer();
	yFloatString = value[1] <= -HUGE_VAL ? "-INFINITY" : yFloatString.Buffer();
	yFloatString = value[1] >= HUGE_VAL ? "INFINITY" : yFloatString.Buffer();
	zFloatString = value[2] <= -HUGE_VAL ? "-INFINITY" : zFloatString.Buffer();
	zFloatString = value[2] >= HUGE_VAL ? "INFINITY" : zFloatString.Buffer();
	string += xFloatString;
	string += ", ";
	string += yFloatString;
	string += ", ";
	string += zFloatString;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::Display4DVector(const char* pHeader, FbxVector4 value, const char* pSuffix)
{
	FbxString string = pHeader;
	FbxString xFloatString = (float)value[0];
	FbxString yFloatString = (float)value[1];
	FbxString zFloatString = (float)value[2];
	FbxString wFloatString = (float)value[3];
	xFloatString = value[0] <= -HUGE_VAL ? "-INFINITY" : xFloatString.Buffer();
	xFloatString = value[0] >= HUGE_VAL ? "INFINITY" : xFloatString.Buffer();
	yFloatString = value[1] <= -HUGE_VAL ? "-INFINITY" : yFloatString.Buffer();
	yFloatString = value[1] >= HUGE_VAL ? "INFINITY" : yFloatString.Buffer();
	zFloatString = value[2] <= -HUGE_VAL ? "-INFINITY" : zFloatString.Buffer();
	zFloatString = value[2] >= HUGE_VAL ? "INFINITY" : zFloatString.Buffer();
	wFloatString = value[3] <= -HUGE_VAL ? "-INFINITY" : wFloatString.Buffer();
	wFloatString = value[3] >= HUGE_VAL ? "INFINITY" : wFloatString.Buffer();
	string += xFloatString;
	string += ", ";
	string += yFloatString;
	string += ", ";
	string += zFloatString;
	string += ", ";
	string += wFloatString;
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::DisplayColor(const char* pHeader, FbxColor value, const char* pSuffix)
{
	FbxString string = pHeader;
	string += (float)value.mRed;
	string += " (red), ";
	string += (float)value.mGreen;
	string += " (green), ";
	string += (float)value.mBlue;
	string += " (blue), ";
	string += (float)value.mAlpha;
	string += " (alpha)";
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

void FBXUtility::DisplayMaterialComponent(const char* pHeader, FbxColor value, const char* pSuffix)
{
	FbxString string;
	string = pHeader;
	string += (float)value.mRed;
	string += " (red), ";
	string += (float)value.mGreen;
	string += " (green), ";
	string += (float)value.mBlue;
	string += " (blue), ";
	string += (float)value.mAlpha;
	string += " (factor)";
	string += pSuffix;
	string += "\n";
	FBXSDK_printf(string);
}

int FBXUtility::InterpolationFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
	if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
	if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
	return 0;
}

int FBXUtility::ConstantmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
	if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
	return 0;
}

int FBXUtility::TangentmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
	if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
	if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
	if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
	if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
	if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
	return 0;
}

int FBXUtility::TangentweightFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
	if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
	if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
	return 0;
}

int FBXUtility::TangentVelocityFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
	if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
	if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
	return 0;
}

FileVertex FBXUtility::ConvertVertexToCoordSystem(const FileVertex& vertex)
{
	FileVertex output = vertex;
	output.position.z = -output.position.z;
	output.tangent.z = -output.tangent.z;
	output.binormal.z = -output.binormal.z;
	output.normal.z = -output.position.z;
	output.texture.y = 1.0f - output.texture.y;
	return output;
}

FbxAMatrix FBXUtility::ConvertMatrixToCoordSystem(const FbxAMatrix& matrix)
{
	FbxVector4 translation = matrix.GetT();
	FbxVector4 rotation = matrix.GetR();
	translation.Set(translation.mData[0], translation.mData[1], -translation.mData[2]);
	rotation.Set(-rotation.mData[0], -rotation.mData[1], rotation.mData[2]);
	FbxAMatrix out = matrix;
	out.SetT(translation);
	out.SetR(rotation);
	return FBXMath::FBXAffinedMatrixMultiply(out, FBXMath::FBXMatrixRotationY(180.0));

	/*return FbxAMatrix(
	     matrix.mData[0][0],  matrix.mData[0][1],  matrix.mData[0][2], -matrix.mData[0][3],
	     matrix.mData[1][0],  matrix.mData[1][1],  matrix.mData[1][2], -matrix.mData[1][3],
	    -matrix.mData[2][0], -matrix.mData[2][1],  matrix.mData[2][2],  matrix.mData[2][3],
	     matrix.mData[3][0],  matrix.mData[3][1], -matrix.mData[3][2],  matrix.mData[3][3]
	);*/
}

FbxAMatrix FBXUtility::GetGeometryTransformation(const FbxNode* pNode)
{
	if (!pNode)
		throw std::exception("Call to FBXUtility::GetGeometryTransformation Failed.\nNull for mesh geometry");
	const FbxVector4& translation = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4& rotation = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4& scaling = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}

FbxAMatrix FBXUtility::FBXMath::FBXMatrixRotationX(double degrees)
{
	return FbxAMatrix();
}

FbxAMatrix FBXUtility::FBXMath::FBXMatrixRotationY(double degrees)
{
	return
	{

	};
}

FbxAMatrix FBXUtility::FBXMath::FBXMatrixRotationZ(double degrees)
{
	return FbxAMatrix();
}

FbxAMatrix FBXUtility::FBXMath::FBXAffinedMatrixMultiply(const FbxAMatrix& a, const FbxAMatrix& b)
{
	FbxAMatrix out;
	return out;
}
