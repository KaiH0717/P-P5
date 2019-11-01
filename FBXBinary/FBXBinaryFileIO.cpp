#include "FBXBinaryFileIO.h"
#include <Windows.h>

std::vector<FileMesh>& FBXBinaryFileIO::GetMeshes() { return meshes; }
std::vector<FileMaterial>& FBXBinaryFileIO::GetMaterials() { return materials; }
FileAnimationClip& FBXBinaryFileIO::GetAnimationClip() { return animationClip; }
std::vector<XMFLOAT4X4>& FBXBinaryFileIO::GetInverseBindMatrices() { return inverseBindPoseMatrices; }

bool FBXBinaryFileIO::Write(const char* outputVertexFile, const char* outputMaterialFile, const char* outputAnimationFile)
{
	std::fstream bfout;
	bfout.open(outputVertexFile, std::ios::out | std::ios::binary | std::ios::trunc);
	if (bfout.is_open())
	{
		// per mesh data
		uint32_t meshCount = (uint32_t)meshes.size();
		bfout.write((char*)&meshCount, sizeof(uint32_t));
		for (uint32_t i = 0; i < meshCount; ++i)
		{
			// write vertex data for per mesh
			uint32_t vertexCount = (uint32_t)meshes[i].vertices.size();
			bfout.write((char*)&vertexCount, sizeof(uint32_t));
			bfout.write((char*)meshes[i].vertices.data(), vertexCount * sizeof(FileVertex));
			// write index data for per mesh
			uint32_t indexCount = (uint32_t)meshes[i].indices.size();
			bfout.write((char*)&indexCount, sizeof(uint32_t));
			bfout.write((char*)meshes[i].indices.data(), indexCount * sizeof(uint32_t));
			// write joint data for per mesh
			uint32_t jointCount = (uint32_t)meshes[i].joints.size();
			bfout.write((char*)&jointCount, sizeof(uint32_t));
			bfout.write((char*)meshes[i].joints.data(), jointCount * sizeof(FileJoint));
		}
		bfout.close();
		std::cout << "\nVertex File Write Success!";
	}
	else
	{
		std::cout << "\nFailed to open file: " << outputVertexFile;
		return false;
	}

	bfout.open(outputMaterialFile, std::ios::out | std::ios::binary | std::ios::trunc);
	if (bfout.is_open())
	{
		// material data
		uint32_t materialCount = (uint32_t)(materials.size());
		bfout.write((char*)&materialCount, sizeof(uint32_t));
		bfout.write((char*)materials.data(), materialCount * sizeof(FileMaterial));
		bfout.close();
		std::cout << "\nMaterial File Write Success!";
	}
	else
	{
		std::cout << "\nFailed to open file: " << outputMaterialFile;
		return false;
	}

	bfout.open(outputAnimationFile, std::ios::out | std::ios::binary | std::ios::trunc);
	if (bfout.is_open())
	{
		double duration = animationClip.duration;
		double frameCount = animationClip.frameCount;
		bfout.write((char*)&duration, sizeof(double));
		bfout.write((char*)&frameCount, sizeof(double));
		for (uint32_t i = 0; i < frameCount; ++i)
		{
			double currentFrameTime = animationClip.frames[i].time;
			uint32_t transformCount = (uint32_t)animationClip.frames[i].jointTransforms.size();
			bfout.write((char*)&currentFrameTime, sizeof(double));
			bfout.write((char*)&transformCount, sizeof(uint32_t));
			bfout.write((char*)animationClip.frames[i].jointTransforms.data(), transformCount * sizeof(XMFLOAT4X4));
		}
		uint32_t inverseBindMatrixCount = (uint32_t)(inverseBindPoseMatrices.size());
		bfout.write((char*)&inverseBindMatrixCount, sizeof(uint32_t));
		bfout.write((char*)inverseBindPoseMatrices.data(), inverseBindMatrixCount * sizeof(XMFLOAT4X4));
		bfout.close();
		std::cout << "\nAnimation File Write Success!" << std::endl;
	}
	else
	{
		std::cout << "\nFailed to open file: " << outputAnimationFile << std::endl;
		return false;
	}
	return true;
}

bool FBXBinaryFileIO::Read(const char* inputVertexFile, const char* inputMaterialFile, const char* inputAnimationFile)
{
	ZeroMemory(&meshes, sizeof(meshes));
	ZeroMemory(&materials, sizeof(materials));
	ZeroMemory(&animationClip, sizeof(animationClip));
	ZeroMemory(&inverseBindPoseMatrices, sizeof(inverseBindPoseMatrices));
	std::fstream bfin;
	bfin.open(inputVertexFile, std::ios::in | std::ios::binary);
	if (bfin.is_open())
	{
		// get mesh count
		uint32_t meshCount = 0;
		bfin.read((char*)&meshCount, sizeof(uint32_t));
		meshes.resize(size_t(meshCount));
		// loop per mesh
		for (uint32_t i = 0; i < meshCount; ++i)
		{
			// get vertex count
			uint32_t vertexCount = 0;
			bfin.read((char*)&vertexCount, sizeof(uint32_t));
			// resize first then read whole block, reading vertex data
			meshes[i].vertices.resize(size_t(vertexCount));
			bfin.read((char*)meshes[i].vertices.data(), vertexCount * sizeof(FileVertex));
			// get index count
			uint32_t indexCount = 0;
			bfin.read((char*)&indexCount, sizeof(uint32_t));
			// resize first then read whole block, reading index data
			meshes[i].indices.resize(size_t(indexCount));
			bfin.read((char*)meshes[i].indices.data(), indexCount * sizeof(uint32_t));
			// get joint count
			uint32_t jointCount = 0;
			bfin.read((char*)&jointCount, sizeof(uint32_t));
			// resize first then read whole block, reading joint data
			meshes[i].joints.resize(size_t(jointCount));
			bfin.read((char*)meshes[i].joints.data(), jointCount * sizeof(FileJoint));
		}
		bfin.close();
		std::cout << "\nVertex File Read Success!";
	}
	else
	{
		std::cout << "\nFailed to open file: " << inputVertexFile;
		return false;
	}

	bfin.open(inputMaterialFile, std::ios::in | std::ios::binary);
	if (bfin.is_open())
	{
		// material data
		uint32_t materialCount = 0;
		bfin.read((char*)&materialCount, sizeof(uint32_t));
		materials.resize(materialCount);
		bfin.read((char*)materials.data(), materialCount * sizeof(FileMaterial));
		bfin.close();
		std::cout << "\nMaterial File Read Success!";
	}
	else
	{
		std::cout << "\nFailed to open file: " << inputMaterialFile;
		return false;
	}

	bfin.open(inputAnimationFile, std::ios::in | std::ios::binary);
	if (bfin.is_open())
	{
		bfin.read((char*)&animationClip.duration, sizeof(double));
		bfin.read((char*)&animationClip.frameCount, sizeof(double));
		animationClip.frames.resize(animationClip.frameCount);
		for (uint32_t i = 0; i < animationClip.frameCount; ++i)
		{
			uint32_t transformCount = 0;
			bfin.read((char*)&animationClip.frames[i].time, sizeof(double));
			bfin.read((char*)&transformCount, sizeof(uint32_t));
			animationClip.frames[i].jointTransforms.resize(transformCount);
			bfin.read((char*)animationClip.frames[i].jointTransforms.data(), transformCount * sizeof(XMFLOAT4X4));
		}
		uint32_t inverseBindMatrixCount = 0;
		bfin.read((char*)&inverseBindMatrixCount, sizeof(uint32_t));
		inverseBindPoseMatrices.resize(inverseBindMatrixCount);
		bfin.read((char*)inverseBindPoseMatrices.data(), inverseBindMatrixCount * sizeof(XMFLOAT4X4));
		bfin.close();
		std::cout << "\nMaterial File Read Success!" << std::endl;
	}
	else
	{
		std::cout << "\nFailed to open file: " << inputMaterialFile << std::endl;
		return false;
	}

	return true;
}
