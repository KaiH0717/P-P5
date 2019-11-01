#pragma once
// Include DirectX11 and DirectXMath for interface and math operation access
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include "DDSTextureLoader/DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include <vector>
#include <wrl.h>

#include "../FBXBinary/FBXBinaryFileIO.h"

typedef enum TEXTURE_FILE_FORMAT
{
	DDS,
	PNG
} TEXTURE_FILE_FORMAT;

struct Vertex
{
	XMFLOAT4 position;
	XMFLOAT4 tangent;
	XMFLOAT4 binormal;
	XMFLOAT3 normal;
	XMFLOAT2 texture;
	XMINT4 joints;
	XMFLOAT4 weights;
};

struct Joint
{
	XMFLOAT4X4 world;
	int parentIndex;
};

struct KeyFrame
{
	double time;
	std::vector<XMFLOAT4X4> jointTransforms;
};

struct AnimationClip
{
	double duration;
	double frameCount;
	std::vector<KeyFrame> frames;
};

struct Mesh
{
	//////////////////////////////
	// attributes
	//////////////////////////////
	Vertex* vertices = nullptr;
	unsigned int* indices = nullptr;
	Joint* joints = nullptr;
	unsigned int vertexCount = 0;
	unsigned int indexCount = 0;
	unsigned int jointCount = 0;
	float scale = 1.0f;
	AnimationClip animClip;
	std::vector<XMFLOAT4X4> inverseBindMatrices;

	//////////////////////////////
	// buffers
	//////////////////////////////
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	//////////////////////////////
	// textures
	//////////////////////////////
	std::vector<ID3D11ShaderResourceView*> shaderResourceViews;
	ID3D11SamplerState* sampler = nullptr;

	//////////////////////////////
	// clean up all the resource and heap memory
	//////////////////////////////
	Mesh() = default;
	~Mesh();

	//////////////////////////////
	// resource initailization
	//////////////////////////////
	HRESULT CreateVertexBuffer(ID3D11Device* device);
	HRESULT CreateIndexBuffer(ID3D11Device* device);
	HRESULT CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName, TEXTURE_FILE_FORMAT type);
	HRESULT CreateSamplerState(ID3D11Device* device);
};

inline Mesh::~Mesh()
{
	// Clear heap memory
	if (vertices) { delete[] vertices; vertices = nullptr; }
	if (indices) { delete[] indices; indices = nullptr; }
	if (joints) { delete[] joints; joints = nullptr; }
	// Release resources
	if (vertexBuffer) { vertexBuffer->Release(); vertexBuffer = nullptr; }
	if (indexBuffer) { indexBuffer->Release(); indexBuffer = nullptr; }
	for (size_t i = 0; i < shaderResourceViews.size(); i++) { if (shaderResourceViews[i]) { shaderResourceViews[i]->Release(); } }
	if (sampler) { sampler->Release(); sampler = nullptr; }
}

inline HRESULT Mesh::CreateVertexBuffer(ID3D11Device* device)
{
	if (device)
	{
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA subRsrcData;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subRsrcData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(Vertex) * this->vertexCount;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		subRsrcData.pSysMem = this->vertices;
		return device->CreateBuffer(&bufferDesc, &subRsrcData, &this->vertexBuffer);
	}
	return E_INVALIDARG;
}

inline HRESULT Mesh::CreateIndexBuffer(ID3D11Device* device)
{
	if (device)
	{
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA subRsrcData;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subRsrcData, sizeof(D3D11_SUBRESOURCE_DATA));
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.ByteWidth = sizeof(unsigned int) * this->indexCount;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		subRsrcData.pSysMem = this->indices;
		return device->CreateBuffer(&bufferDesc, &subRsrcData, &this->indexBuffer);
	}
	return E_INVALIDARG;
}

inline HRESULT Mesh::CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName, TEXTURE_FILE_FORMAT type)
{
	if (device)
	{
		HRESULT hr = S_OK;
		ID3D11ShaderResourceView* srv;
		switch (type)
		{
		case TEXTURE_FILE_FORMAT::DDS:
		{
			hr = CreateDDSTextureFromFile(device, fileName, nullptr, &srv);
		}
		break;
		case TEXTURE_FILE_FORMAT::PNG:
		{
			hr = CreateWICTextureFromFile(device, fileName, nullptr, &srv);
		}
		break;
		}
		shaderResourceViews.push_back(srv);
		return hr;
	}
	return E_INVALIDARG;
}

inline HRESULT Mesh::CreateSamplerState(ID3D11Device* device)
{
	if (device)
	{
		D3D11_SAMPLER_DESC sampleDesc;
		ZeroMemory(&sampleDesc, sizeof(D3D11_SAMPLER_DESC));
		sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampleDesc.MinLOD = 0;
		sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
		return device->CreateSamplerState(&sampleDesc, &this->sampler);
	}
	return E_INVALIDARG;
}
