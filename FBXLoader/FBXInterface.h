#pragma once
//--------------------------------------------------
// This header defines fbx loader interfaces
//--------------------------------------------------
#include "fbxsdk.h"
#include <Windows.h>
#include <functional>
#include "../FBXBinary/FileStructures.h"

struct VERTEXHASH
{
	std::size_t operator()(const FileVertex& v) const
	{
		return std::size_t(
			std::hash<float>()(v.position.x) +
			std::hash<float>()(v.position.y) +
			std::hash<float>()(v.position.z) +
			std::hash<float>()(v.position.w) +
			std::hash<float>()(v.tangent.x) +
			std::hash<float>()(v.tangent.y) +
			std::hash<float>()(v.tangent.z) +
			std::hash<float>()(v.tangent.w) +
			std::hash<float>()(v.binormal.x) +
			std::hash<float>()(v.binormal.y) +
			std::hash<float>()(v.binormal.z) +
			std::hash<float>()(v.binormal.w) +
			std::hash<float>()(v.normal.x) +
			std::hash<float>()(v.normal.y) +
			std::hash<float>()(v.normal.z) +
			std::hash<float>()(v.texture.x) +
			std::hash<float>()(v.texture.y) +
			std::hash<float>()(v.joints.x) +
			std::hash<float>()(v.joints.y) +
			std::hash<float>()(v.joints.z) +
			std::hash<float>()(v.joints.w) +
			std::hash<float>()(v.weights.x) +
			std::hash<float>()(v.weights.y) +
			std::hash<float>()(v.weights.z) +
			std::hash<float>()(v.weights.w)
		);
	}
};

struct VERTEXEQUAL
{
	bool operator()(const FileVertex& a, const FileVertex& b) const
	{
		return (
			a.position.x == b.position.x &&
			a.position.y == b.position.y &&
			a.position.z == b.position.z &&
			a.position.w == b.position.w &&
			a.tangent.x == b.tangent.x &&
			a.tangent.y == b.tangent.y &&
			a.tangent.z == b.tangent.z &&
			a.tangent.w == b.tangent.w &&
			a.binormal.x == b.binormal.x &&
			a.binormal.y == b.binormal.y &&
			a.binormal.z == b.binormal.z &&
			a.binormal.w == b.binormal.w &&
			a.normal.x == b.normal.x &&
			a.normal.y == b.normal.y &&
			a.normal.z == b.normal.z &&
			a.texture.x == b.texture.x &&
			a.texture.y == b.texture.y &&
			a.joints.x == b.joints.x &&
			a.joints.y == b.joints.y &&
			a.joints.z == b.joints.z &&
			a.joints.w == b.joints.w &&
			a.weights.x == b.weights.x &&
			a.weights.y == b.weights.y &&
			a.weights.z == b.weights.z &&
			a.weights.w == b.weights.w
			);
	}
};

struct FBXJoint
{
	FbxNode* node;
	int32_t parentIndex;
};

struct FBXMesh
{
	std::vector<FileVertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<FBXJoint> joints;
};

struct FBXData
{
	std::vector<FBXMesh> fbxMeshes;
	std::vector<FileMaterial> fbxMaterials;
	std::vector<std::vector<FBXJoint>> fbxJoints;
	FileAnimationClip fbxAnimationClip;
	std::vector<XMFLOAT4X4> inverseBindPoseMatrix;

	std::vector<std::vector<Influence>> fbxControlPointInfluences;

	// used to store data
	int currentMeshID = 0;
};

class IFBXSceneData
{
protected:
	FBXData& fbxData;
public:
	IFBXSceneData(FBXData& fbxData) : fbxData(fbxData) {}
	//----------------------------------------
	// Interface function/Wrapper function
	//----------------------------------------
	virtual void Load(FbxScene*) = 0;
};

class IFBXNodeData
{
protected:
	FBXData& fbxData;
public:
	IFBXNodeData(FBXData& fbxData) : fbxData(fbxData) {}
	//----------------------------------------
	// Interface function/Wrapper function
	//----------------------------------------
	virtual void Load(FbxNode*) = 0;
};

class IFBXGeometryData
{
protected:
	FBXData& fbxData;
public:
	IFBXGeometryData(FBXData& fbxData) : fbxData(fbxData) {}
	//----------------------------------------
	// Interface function/Wrapper function
	//----------------------------------------
	virtual void Load(FbxGeometry*) = 0;
};

