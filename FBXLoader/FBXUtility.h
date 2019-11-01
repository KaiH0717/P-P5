#pragma once
#include "fbxsdk.h"
#include "../FBXBinary/FileStructures.h"

class FBXUtility
{
public:
	static void DisplayMetaDataConnections(FbxObject* pObject);
	static void DisplayString(const char* pHeader, const char* pBuffer = "", const char* pSuffix = "");
	static void DisplayBool(const char* pHeader, bool value, const char* pSuffix = "");
	static void DisplayInt(const char* pHeader, int value, const char* pSuffix = "");
	static void DisplayDouble(const char* pHeader, double value, const char* pSuffix = "");
	static void Display2DVector(const char* pHeader, FbxVector2 value, const char* pSuffix = "");
	static void Display3DVector(const char* pHeader, FbxVector4 value, const char* pSuffix = "");
	static void Display4DVector(const char* pHeader, FbxVector4 value, const char* pSuffix = "");
	static void DisplayColor(const char* pHeader, FbxColor value, const char* pSuffix = "");
	static void DisplayMaterialComponent(const char* pHeader, FbxColor value, const char* pSuffix = "");

	static int InterpolationFlagToIndex(int flags);
	static int ConstantmodeFlagToIndex(int flags);
	static int TangentmodeFlagToIndex(int flags);
	static int TangentweightFlagToIndex(int flags);
	static int TangentVelocityFlagToIndex(int flags);

	static FileVertex ConvertVertexToCoordSystem(const FileVertex& vertex);
	static FbxAMatrix ConvertMatrixToCoordSystem(const FbxAMatrix& matrix);
	static FbxAMatrix GetGeometryTransformation(const FbxNode* pNode);

	class FBXMath
	{
	public:
		static FbxAMatrix FBXMatrixRotationX(double degrees);
		static FbxAMatrix FBXMatrixRotationY(double degrees);
		static FbxAMatrix FBXMatrixRotationZ(double degrees);
		static FbxAMatrix FBXAffinedMatrixMultiply(const FbxAMatrix& a, const FbxAMatrix& b);
	};
};

