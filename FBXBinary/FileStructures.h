#pragma once
#include <vector>
#include <DirectXMath.h>
#include <array>
using namespace DirectX;

struct FileVertex
{
	XMFLOAT4 position;
	XMFLOAT4 tangent;
	XMFLOAT4 binormal;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	XMINT4 joints;
	XMFLOAT4 weights;
};

struct FileJoint
{
	XMFLOAT4X4 world;
	int32_t parentIndex;
};

struct FileMesh
{
	std::vector<FileVertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<FileJoint> joints;
};

struct FileKeyFrame
{
	double time;
	std::vector<XMFLOAT4X4> jointTransforms;
};

struct FileAnimationClip
{
	double duration;
	double frameCount;
	std::vector<FileKeyFrame> frames;
};

struct Influence
{
	int joint;
	float weight;
};

typedef enum class PHONG_COMPONENT_TYPE
{
	Ambient,
	Diffuse,
	Specular,
	Emissive,
	Opacity,
	Shininess,
	Reflectivity,
	Count
} Phong;

using FilePath = std::array<char, 260>;
struct FileMaterial
{
private:
	struct Component
	{
		XMFLOAT3 color = XMFLOAT3(0.0f, 0.0f, 0.0f);
		// this will be used for OPACITY, SHININESS, REFLECTIVITY, anything that is factor related
		float factor = 1.0f;
		// fileSize will stay at -1 if no path were found
		int64_t fileSize = -1;
		FilePath filePath = { "None" };
	};
private:
	Component components[(unsigned int)Phong::Count];
public:
	Component& operator[](Phong type) { return components[(unsigned int)type]; }
	const Component& operator[](Phong type) const { return components[(unsigned int)type]; }
	Component& operator[](unsigned int type) { return components[type]; }
	const Component& operator[](unsigned int type) const { return components[type]; }
};