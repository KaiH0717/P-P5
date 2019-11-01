#pragma once
#include "FBXInterface.h"

class FBXGeometryLink : public IFBXGeometryData
{
public:
	FBXGeometryLink(FBXData& fbxData)
		: IFBXGeometryData(fbxData) {}

	void Load(FbxGeometry* pGeometry);
};
