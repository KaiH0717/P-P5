#pragma once
#include "FBXInterface.h"
#include "FBXGeometryMaterial.h"
#include "FBXGeometryLink.h"

class FBXNodeMesh : public IFBXNodeData
{
	FBXGeometryMaterial materialLoader;
	FBXGeometryLink linkLoader;
public:
	FBXNodeMesh(FBXData& fbxData)
		: IFBXNodeData(fbxData), materialLoader(fbxData), linkLoader(fbxData) {}

	void Load(FbxNode* pNode);
private:
	//----------------------------------------
	// Helper function used in Load
	//----------------------------------------
	void LoadControlsPoints(FbxMesh* pMesh);
	void LoadPolygons(FbxMesh* pMesh);
	void LoadMaterialMapping(FbxMesh* pMesh);
	void LoadTextureNames(FbxProperty& property, FbxString& string);
	void LoadMaterialConnections(FbxMesh* pMesh);
	void LoadMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* pHeader, int matId, int i);
};
