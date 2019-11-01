#pragma once
#include "Mesh.h"

class Model
{
private:
	// DO NOT CALL RELEASE, IT WILL BE HANDLED BY SHADER MANAGER CLASS
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;
	// Model attributes
	std::vector<Mesh*> meshes;
	std::vector<FileMaterial> materials;
	// Model's position relative to the world
	XMFLOAT4X4 world;
private:
	void SetPipeline(ID3D11DeviceContext* context, unsigned int index);
public:
	Model();
	~Model();

	//////////////////////////////
	// getters
	//////////////////////////////
	std::vector<Mesh*>& GetMeshes();
	std::vector<FileMaterial>& GetMaterials();
	XMMATRIX GetWorldMatrix();

	//////////////////////////////
	// setters
	//////////////////////////////
	void SetWorldMatrix(const XMMATRIX& world);
	void SetVertexShader(ID3D11VertexShader* vertexShader);
	void SetPixelShader(ID3D11PixelShader* pixelShader);
	void SetGeometryShader(ID3D11GeometryShader* geometryShader);

	//////////////////////////////
	// Methods
	//////////////////////////////
	void LoadFBXModel(ID3D11Device* device, const char** fileName, float scale = 1.0f);

	//////////////////////////////
	// draw calls
	//////////////////////////////
	void DrawIndexed(ID3D11DeviceContext* context, D3D11_VIEWPORT* viewPort);
	void DrawIndexInstanced(ID3D11DeviceContext* context, D3D11_VIEWPORT* viewPort, unsigned int numberOfInstance);

	//////////////////////////////
	// resource initailization
	// REMARKS: MUST HAVE VALID VERTICES AND INDCIES
	//////////////////////////////
	void CreateVertexBuffer(ID3D11Device* device);
	void CreateIndexBuffer(ID3D11Device* device);
	void CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName, TEXTURE_FILE_FORMAT type);
	void CreateSamplerState(ID3D11Device* device);
};
