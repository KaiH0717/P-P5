#include "FBXNodeMesh.h"
#include "FBXUtility.h"
#include <unordered_map>
#define MAX_HEADER_LENGTH 200

void FBXNodeMesh::Load(FbxNode* pNode)
{
	FbxMesh* pMesh = pNode->GetMesh();
	FBXUtility::DisplayString("Mesh Name: ", pNode->GetName());
	LoadControlsPoints(pMesh);
	linkLoader.Load(pMesh);
	LoadPolygons(pMesh);
	//LoadMaterialMapping(pMesh);
	materialLoader.Load(pMesh);
	// textureLoader.Load(pMesh, fileIO, fileMeshData); // not implemented yet
	LoadMaterialConnections(pMesh);
}

void FBXNodeMesh::LoadControlsPoints(FbxMesh* pMesh)
{
	std::vector<FbxVector4> controlPointsData;
	int controlPointsCount = pMesh->GetControlPointsCount();
	// resize control point influence
	fbxData.fbxControlPointInfluences.resize(controlPointsCount);

	FbxVector4* controlPoints = pMesh->GetControlPoints();
	FBXUtility::DisplayInt("\tControl Points Count: ", controlPointsCount);
	for (int i = 0; i < controlPointsCount; ++i)
	{
		controlPointsData.push_back(controlPoints[i]);
	}
}

void FBXNodeMesh::LoadPolygons(FbxMesh* pMesh)
{
	int polygonCount = pMesh->GetPolygonCount();
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	//------------------------------------
	// std::unordered_map and iterator
	// for checking duplicate vertices
	//------------------------------------
	std::unordered_map<FileVertex, uint32_t, VERTEXHASH, VERTEXEQUAL> map;
	std::unordered_map<FileVertex, uint32_t, VERTEXHASH, VERTEXEQUAL>::iterator iterator;
	// used for storing data
	FileVertex fileVertex;
	int duplicateVertexRemoved = 0;

	//------------------------------------
	// Generate tangent and binormal data
	//------------------------------------
	FbxStringList uvSetName;
	pMesh->GetUVSetNames(uvSetName);
	int uvSetNameCount = uvSetName.GetCount();
	if (uvSetNameCount > 0)
		pMesh->GenerateTangentsData(uvSetName[0]);

	int vertexId = 0;
	for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		int vertexCount = pMesh->GetPolygonSize(polygonIndex); // should be 3 vertex per triangle
		for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
		{
			// Position data
			int controlPointIndex = pMesh->GetPolygonVertex(polygonIndex, vertexIndex);
			const FbxVector4& vertex = controlPoints[controlPointIndex];
			fileVertex.position.x = (float)vertex.mData[0];
			fileVertex.position.y = (float)vertex.mData[1];
			fileVertex.position.z = (float)vertex.mData[2];
			fileVertex.position.w = 1.0f;

#pragma region Texture Data
			for (int i = 0; i < pMesh->GetElementUVCount(); ++i)
			{
				FbxGeometryElementUV* vertexUV = pMesh->GetElementUV(i);
				switch (vertexUV->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					switch (vertexUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[0];
						fileVertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(controlPointIndex).mData[1];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexUV->GetIndexArray().GetAt(controlPointIndex);
						fileVertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(id).mData[0];
						fileVertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(id).mData[1];
					}
					break;
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (vertexUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(vertexId).mData[0];
						fileVertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(vertexId).mData[1];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexUV->GetIndexArray().GetAt(vertexId);
						fileVertex.texture.x = (float)vertexUV->GetDirectArray().GetAt(id).mData[0];
						fileVertex.texture.y = (float)vertexUV->GetDirectArray().GetAt(id).mData[1];
					}
					break;
					}
				}
				break;
				}
			}
#pragma endregion

#pragma region Normal Data
			for (int i = 0; i < pMesh->GetElementNormalCount(); ++i)
			{
				FbxGeometryElementNormal* vertexNormal = pMesh->GetElementNormal(i);
				switch (vertexNormal->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					switch (vertexNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.normal.x = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0];
						fileVertex.normal.y = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1];
						fileVertex.normal.z = (float)vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
						fileVertex.normal.x = (float)vertexNormal->GetDirectArray().GetAt(id).mData[0];
						fileVertex.normal.y = (float)vertexNormal->GetDirectArray().GetAt(id).mData[1];
						fileVertex.normal.z = (float)vertexNormal->GetDirectArray().GetAt(id).mData[2];
					}
					break;
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (vertexNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.normal.x = (float)vertexNormal->GetDirectArray().GetAt(vertexId).mData[0];
						fileVertex.normal.y = (float)vertexNormal->GetDirectArray().GetAt(vertexId).mData[1];
						fileVertex.normal.z = (float)vertexNormal->GetDirectArray().GetAt(vertexId).mData[2];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexNormal->GetIndexArray().GetAt(vertexId);
						fileVertex.normal.x = (float)vertexNormal->GetDirectArray().GetAt(id).mData[0];
						fileVertex.normal.y = (float)vertexNormal->GetDirectArray().GetAt(id).mData[1];
						fileVertex.normal.z = (float)vertexNormal->GetDirectArray().GetAt(id).mData[2];
					}
					break;
					}
				}
				break;
				}
			}
#pragma endregion

#pragma region Tangent Data
			for (int i = 0; i < pMesh->GetElementTangentCount(); ++i)
			{
				FbxGeometryElementTangent* vertexTangent = pMesh->GetElementTangent(i);
				switch (vertexTangent->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					switch (vertexTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.tangent.x = (float)vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0];
						fileVertex.tangent.y = (float)vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1];
						fileVertex.tangent.z = (float)vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2];
						fileVertex.tangent.w = (float)vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[3];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
						fileVertex.tangent.x = (float)vertexTangent->GetDirectArray().GetAt(id).mData[0];
						fileVertex.tangent.y = (float)vertexTangent->GetDirectArray().GetAt(id).mData[1];
						fileVertex.tangent.z = (float)vertexTangent->GetDirectArray().GetAt(id).mData[2];
						fileVertex.tangent.w = (float)vertexTangent->GetDirectArray().GetAt(id).mData[3];
					}
					break;
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (vertexTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.tangent.x = (float)vertexTangent->GetDirectArray().GetAt(vertexId).mData[0];
						fileVertex.tangent.y = (float)vertexTangent->GetDirectArray().GetAt(vertexId).mData[1];
						fileVertex.tangent.z = (float)vertexTangent->GetDirectArray().GetAt(vertexId).mData[2];
						fileVertex.tangent.w = (float)vertexTangent->GetDirectArray().GetAt(vertexId).mData[3];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexTangent->GetIndexArray().GetAt(vertexId);
						fileVertex.tangent.x = (float)vertexTangent->GetDirectArray().GetAt(id).mData[0];
						fileVertex.tangent.y = (float)vertexTangent->GetDirectArray().GetAt(id).mData[1];
						fileVertex.tangent.z = (float)vertexTangent->GetDirectArray().GetAt(id).mData[2];
						fileVertex.tangent.w = (float)vertexTangent->GetDirectArray().GetAt(id).mData[3];
					}
					break;
					}
				}
				break;
				}
			}
#pragma endregion

#pragma region Binormal Data
			for (int i = 0; i < pMesh->GetElementBinormalCount(); ++i)
			{
				FbxGeometryElementBinormal* vertexBinormal = pMesh->GetElementBinormal(i);
				switch (vertexBinormal->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
				{
					switch (vertexBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0];
						fileVertex.binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1];
						fileVertex.binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2];
						fileVertex.binormal.w = (float)vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[3];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
						fileVertex.binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[0];
						fileVertex.binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[1];
						fileVertex.binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[2];
						fileVertex.binormal.w = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[3];
					}
					break;
					}
				}
				break;
				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (vertexBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						fileVertex.binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(vertexId).mData[0];
						fileVertex.binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(vertexId).mData[1];
						fileVertex.binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(vertexId).mData[2];
						fileVertex.binormal.w = (float)vertexBinormal->GetDirectArray().GetAt(vertexId).mData[3];
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = vertexBinormal->GetIndexArray().GetAt(vertexId);
						fileVertex.binormal.x = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[0];
						fileVertex.binormal.y = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[1];
						fileVertex.binormal.z = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[2];
						fileVertex.binormal.w = (float)vertexBinormal->GetDirectArray().GetAt(id).mData[3];
					}
					break;
					}
				}
				break;
				}
			}
#pragma endregion

			//------------------------------------
			vertexId++;
			//------------------------------------

			//------------------------------------------------------------------------
			// end of reading vertex data, time to remove dupes
			//------------------------------------------------------------------------
			// fix model hand system coordinate
			// 1. find online
			/*XMVECTOR rotationFixPosition = XMLoadFloat4(&fileVertex.position);
			rotationFixPosition = XMVector3Transform(rotationFixPosition, XMMatrixRotationY(XMConvertToRadians(180.0f)));
			XMStoreFloat4(&fileVertex.position, rotationFixPosition);
			XMVECTOR rotationFixNormal = XMLoadFloat3(&fileVertex.normal);
			rotationFixNormal = XMVector3Transform(rotationFixNormal, XMMatrixRotationY(XMConvertToRadians(180.0f)));
			XMStoreFloat3(&fileVertex.normal, rotationFixNormal);*/
			fileVertex.joints.x = fbxData.fbxControlPointInfluences[controlPointIndex][0].joint;
			fileVertex.joints.y = fbxData.fbxControlPointInfluences[controlPointIndex][1].joint;
			fileVertex.joints.z = fbxData.fbxControlPointInfluences[controlPointIndex][2].joint;
			fileVertex.joints.w = fbxData.fbxControlPointInfluences[controlPointIndex][3].joint;
			fileVertex.weights.x = fbxData.fbxControlPointInfluences[controlPointIndex][0].weight;
			fileVertex.weights.y = fbxData.fbxControlPointInfluences[controlPointIndex][1].weight;
			fileVertex.weights.z = fbxData.fbxControlPointInfluences[controlPointIndex][2].weight;
			fileVertex.weights.w = fbxData.fbxControlPointInfluences[controlPointIndex][3].weight;
			float sum = fileVertex.weights.x + fileVertex.weights.y + fileVertex.weights.z + fileVertex.weights.w;
			XMVECTOR temp = XMLoadFloat4(&fileVertex.weights) / sum;
			XMStoreFloat4(&fileVertex.weights, temp);
			iterator = map.find(fileVertex);
			if (iterator == map.end())
			{
				map[fileVertex] = (uint32_t)fbxData.fbxMeshes[fbxData.currentMeshID].vertices.size();
				fbxData.fbxMeshes[fbxData.currentMeshID].indices.push_back((uint32_t)fbxData.fbxMeshes[fbxData.currentMeshID].vertices.size());
				fbxData.fbxMeshes[fbxData.currentMeshID].vertices.push_back(fileVertex);
			}
			else
			{
				duplicateVertexRemoved++;
				fbxData.fbxMeshes[fbxData.currentMeshID].indices.push_back(iterator->second);
			}
		} // for vertexCount
	} // for polygonCount/triangleCount
	FBXUtility::DisplayInt("\tPolygon/Triangle Count: ", polygonCount);
	FBXUtility::DisplayInt("\tTotal Vertex Count: ", polygonCount * 3);
	FBXUtility::DisplayInt("\tUnique Vertex Count: ", (int)fbxData.fbxMeshes[fbxData.currentMeshID].vertices.size());
	FBXUtility::DisplayInt("\tDuplicate Vertex Removed: ", duplicateVertexRemoved);
}

void FBXNodeMesh::LoadMaterialMapping(FbxMesh* pMesh)
{
	const char* mappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* referenceMode[] = { "Direct", "Index", "Index to Direct" };

	int materialCount = 0;
	FbxNode* node = pMesh->GetNode();
	if (node)
	{
		materialCount = node->GetMaterialCount();
	}

	for (int i = 0; i < pMesh->GetElementMaterialCount(); ++i)
	{
		FbxGeometryElementMaterial* material = pMesh->GetElementMaterial(i);
		if (material)
		{
			char header[100];
			FBXSDK_sprintf(header, 100, "    Material Element %d: ", i);
			FBXUtility::DisplayString(header);
			FBXUtility::DisplayString("           Mapping: ", mappingTypes[material->GetMappingMode()]);
			FBXUtility::DisplayString("           ReferenceMode: ", referenceMode[material->GetReferenceMode()]);

			if (material->GetReferenceMode() == FbxGeometryElement::eIndex ||
				material->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				FbxString string = "           Indices: ";
				int indexArrayCount = material->GetIndexArray().GetCount();
				for (int j = 0; j < indexArrayCount; ++j)
				{
					string += material->GetIndexArray().GetAt(j);
					if (j < indexArrayCount - 1)
					{
						string += ", ";
					}
				}
				string += "\n";
				FBXSDK_printf(string);
			}
		}
	}
	FBXUtility::DisplayString("");
}

void FBXNodeMesh::LoadTextureNames(FbxProperty& property, FbxString& string)
{
	int layeredTextureCount = property.GetSrcObjectCount<FbxLayeredTexture>();
	if (layeredTextureCount > 0)
	{
		for (int i = 0; i < layeredTextureCount; ++i)
		{
			FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
			int lNbTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
			string += " Texture ";
			for (int j = 0; j < lNbTextures; ++j)
			{
				string += "\"";
				string += layeredTexture->GetName();
				string += "\"";
				string += " ";
			}
			string += "of ";
			string += property.GetName();
			string += " on layer ";
			string += i;
		}
		string += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int textureCount = property.GetSrcObjectCount<FbxTexture>();
		if (textureCount > 0)
		{
			string += " Texture ";
			string += " ";

			for (int j = 0; j < textureCount; ++j)
			{
				FbxTexture* texture = property.GetSrcObject<FbxTexture>(j);
				if (texture)
				{
					string += "\"";
					string += texture->GetName();
					string += "\"";
					string += " ";
				}
			}
			string += "of ";
			string += property.GetName();
			string += " |";
		}
	}
}

void FBXNodeMesh::LoadMaterialConnections(FbxMesh* pMesh)
{
	int i, l, polygonCount = pMesh->GetPolygonCount();
	char header[MAX_HEADER_LENGTH];
	FBXUtility::DisplayString("Polygons Material Connections");
	//check whether the material maps with only one mesh
	bool isSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			isSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (isSame)
	{
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial* materialElement = pMesh->GetElementMaterial(l);
			if (materialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* material = pMesh->GetNode()->GetMaterial(materialElement->GetIndexArray().GetAt(0));
				int lMatId = materialElement->GetIndexArray().GetAt(0);
				if (lMatId >= 0)
				{
					FBXUtility::DisplayInt("\tAll polygons share the same material in mesh ", l);
					LoadMaterialTextureConnections(material, header, lMatId, l);
				}
			}
		}
		//no material
		if (l == 0)
			FBXUtility::DisplayString("\tNo material applied");
	}
	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for (i = 0; i < polygonCount; i++)
		{
			FBXUtility::DisplayInt("\tPolygon ", i);
			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{
				FbxGeometryElementMaterial* materialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int materialID = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(materialElement->GetIndexArray().GetAt(i));
				materialID = materialElement->GetIndexArray().GetAt(i);

				if (materialID >= 0)
				{
					LoadMaterialTextureConnections(lMaterial, header, materialID, l);
				}
			}
		}
	}
}

void FBXNodeMesh::LoadMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* pHeader, int matId, int i)
{
	if (!pMaterial)
		return;

	FbxString string = "\t\tMaterial[%d]  -- ";
	//Show all the textures

	FbxProperty property;
	//Diffuse Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	LoadTextureNames(property, string);

	//DiffuseFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	LoadTextureNames(property, string);

	//Emissive Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	LoadTextureNames(property, string);

	//EmissiveFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
	LoadTextureNames(property, string);

	//Ambient Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	LoadTextureNames(property, string);

	//AmbientFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	LoadTextureNames(property, string);

	//Specular Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	LoadTextureNames(property, string);

	//SpecularFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	LoadTextureNames(property, string);

	//Shininess Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	LoadTextureNames(property, string);

	//Bump Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
	LoadTextureNames(property, string);

	//Normal Map Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	LoadTextureNames(property, string);

	//Transparent Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
	LoadTextureNames(property, string);

	//TransparencyFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	LoadTextureNames(property, string);

	//Reflection Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	LoadTextureNames(property, string);

	//ReflectionFactor Textures
	property = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	LoadTextureNames(property, string);

	//Update header with material info
	bool stringOverflow = (string.GetLen() + 10 >= MAX_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (stringOverflow)
	{
		// Truncate string!
		string = string.Left(MAX_HEADER_LENGTH - 10);
		string = string + "...";
	}
	FBXSDK_sprintf(pHeader, MAX_HEADER_LENGTH, string.Buffer(), matId, i);
	FBXUtility::DisplayString(pHeader);
}
