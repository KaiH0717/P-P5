#pragma once
#include "FBXInterface.h"

class FBXGeometryMaterial : public IFBXGeometryData
{
public:
	FBXGeometryMaterial(FBXData& fbxData)
		: IFBXGeometryData(fbxData) {}

	void Load(FbxGeometry* pGeometry);
};
