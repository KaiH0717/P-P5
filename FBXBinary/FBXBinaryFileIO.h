#pragma once
#include "FileStructures.h"
#include <fstream>
#include <iostream>

class FBXBinaryFileIO
{
	friend class FBXExporter;
private:
	// file data
	std::vector<FileMesh> meshes;
	std::vector<FileMaterial> materials;
	FileAnimationClip animationClip;
	std::vector<XMFLOAT4X4> inverseBindPoseMatrices;
public:
	std::vector<FileMesh>& GetMeshes();
	std::vector<FileMaterial>& GetMaterials();
	FileAnimationClip& GetAnimationClip();
	std::vector<XMFLOAT4X4>& GetInverseBindMatrices();
	//----------------------------------------
	// File IO functions
	//----------------------------------------
	bool Write(const char* outputVertexFile, const char* outputMaterialFile, const char* outputAnimationFile);
	bool Read(const char* inputVertexFile, const char* inputMaterialFile, const char* inputAnimationFile);
};
