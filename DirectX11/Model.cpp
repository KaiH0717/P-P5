#include "Model.h"
#include <string>

void Model::SetPipeline(ID3D11DeviceContext* context, unsigned int index)
{
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;
	context->IASetVertexBuffers(0, 1, &meshes[index]->vertexBuffer, &strides, &offsets);
	context->IASetIndexBuffer(meshes[index]->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(vertexShader, nullptr, 0);
	// optinal shader stages
	if (geometryShader)
		context->GSSetShader(geometryShader, nullptr, 0);
	else
		context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetShaderResources(0, (UINT)meshes[index]->shaderResourceViews.size(), meshes[index]->shaderResourceViews.data());
	context->PSSetSamplers(0, 1, &meshes[index]->sampler);
}

Model::Model()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	geometryShader = nullptr;
	XMStoreFloat4x4(&world, XMMatrixIdentity());
}

Model::~Model()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		delete meshes[i];
	}
	meshes.clear();
}

std::vector<Mesh*>& Model::GetMeshes() { return meshes; }
std::vector<FileMaterial>& Model::GetMaterials() { return materials; }
XMMATRIX Model::GetWorldMatrix() { return XMLoadFloat4x4(&world); }

void Model::SetWorldMatrix(const XMMATRIX& world) { XMStoreFloat4x4(&this->world, world); }
void Model::SetVertexShader(ID3D11VertexShader* vertexShader) { this->vertexShader = vertexShader; }
void Model::SetPixelShader(ID3D11PixelShader* pixelShader) { this->pixelShader = pixelShader; }
void Model::SetGeometryShader(ID3D11GeometryShader* geometryShader) { this->geometryShader = geometryShader; }

void Model::LoadFBXModel(ID3D11Device* device, const char** fileName, float scale)
{
	FBXBinaryFileIO fileIO;
	fileIO.Read(fileName[0], fileName[1], fileName[2]);
	for (unsigned int i = 0; i < (unsigned int)fileIO.GetMeshes().size(); ++i)
	{
		FileMesh currentMesh = fileIO.GetMeshes()[i];
		Mesh* mesh = new Mesh;
		mesh->vertexCount = (unsigned int)currentMesh.vertices.size();
		mesh->indexCount = (unsigned int)currentMesh.indices.size();;
		mesh->jointCount = (unsigned int)currentMesh.joints.size();
		mesh->vertices = new Vertex[mesh->vertexCount];
		mesh->indices = new unsigned int[mesh->indexCount];
		mesh->joints = new Joint[mesh->jointCount];
		for (unsigned int verticesIndex = 0; verticesIndex < mesh->vertexCount; ++verticesIndex)
		{
			mesh->vertices[verticesIndex].position = currentMesh.vertices[verticesIndex].position;
			XMVECTOR position = XMLoadFloat4(&mesh->vertices[verticesIndex].position) * scale;
			XMStoreFloat4(&mesh->vertices[verticesIndex].position, position);
			mesh->vertices[verticesIndex].position.w = 1.0f;
			mesh->vertices[verticesIndex].tangent = currentMesh.vertices[verticesIndex].tangent;
			mesh->vertices[verticesIndex].binormal = currentMesh.vertices[verticesIndex].binormal;
			mesh->vertices[verticesIndex].normal = currentMesh.vertices[verticesIndex].normal;
			mesh->vertices[verticesIndex].texture = currentMesh.vertices[verticesIndex].texture;
			mesh->vertices[verticesIndex].texture.y = 1.0f - mesh->vertices[verticesIndex].texture.y;
			mesh->vertices[verticesIndex].joints = currentMesh.vertices[verticesIndex].joints;
			mesh->vertices[verticesIndex].weights = currentMesh.vertices[verticesIndex].weights;
		}
		for (unsigned int indicesIndex = 0; indicesIndex < mesh->indexCount; ++indicesIndex)
		{
			mesh->indices[indicesIndex] = currentMesh.indices[indicesIndex];
		}
		for (unsigned int jointIndex = 0; jointIndex < mesh->jointCount; jointIndex++)
		{
			mesh->joints[jointIndex].parentIndex = currentMesh.joints[jointIndex].parentIndex;
			mesh->joints[jointIndex].world = currentMesh.joints[jointIndex].world;
		}
		mesh->animClip.duration = fileIO.GetAnimationClip().duration;
		mesh->animClip.frameCount = fileIO.GetAnimationClip().frameCount;
		mesh->animClip.frames.resize(mesh->animClip.frameCount);
		for (int c = 0; c < mesh->animClip.frameCount; c++)
		{
			mesh->animClip.frames[c].time = fileIO.GetAnimationClip().frames[c].time;
			mesh->animClip.frames[c].jointTransforms = fileIO.GetAnimationClip().frames[c].jointTransforms;
		}
		mesh->inverseBindMatrices = fileIO.GetInverseBindMatrices();
		this->meshes.push_back(mesh);
	}
	for (unsigned int i = 0; i < (unsigned int)fileIO.GetMaterials().size(); ++i)
	{
		FileMaterial& currentMaterial = fileIO.GetMaterials()[i];
		materials.push_back(currentMaterial);
		for (unsigned int j = 0; j < (unsigned int)Phong::Count; j++)
		{
			if (currentMaterial[j].fileSize > 0)
			{
				std::string filePath = "..\\Assets\\";
				filePath += currentMaterial[j].filePath.data();
				std::wstring wFilePath;
				wFilePath.assign(filePath.begin(), filePath.end());
				this->CreateShaderResourceView(device, wFilePath.data(), TEXTURE_FILE_FORMAT::PNG);
			}
		}
	}
	this->CreateSamplerState(device);
}

void Model::DrawIndexed(ID3D11DeviceContext* context, D3D11_VIEWPORT* viewPort)
{
	if (context)
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			SetPipeline(context, (unsigned int)i);
			context->RSSetViewports(1, viewPort);
			context->DrawIndexed(meshes[i]->indexCount, 0, 0);
		}
	}
}

void Model::DrawIndexInstanced(ID3D11DeviceContext* context, D3D11_VIEWPORT* viewPort, unsigned int numberOfInstance)
{
	if (context)
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			SetPipeline(context, (unsigned int)i);
			context->RSSetViewports(1, viewPort);
			context->DrawIndexedInstanced(meshes[i]->indexCount, numberOfInstance, 0, 0, 0);
		}
	}
}

void Model::CreateVertexBuffer(ID3D11Device* device)
{
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		meshes[i]->CreateVertexBuffer(device);
	}
}

void Model::CreateIndexBuffer(ID3D11Device* device)
{
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		meshes[i]->CreateIndexBuffer(device);
	}
}

void Model::CreateShaderResourceView(ID3D11Device* device, const wchar_t* fileName, TEXTURE_FILE_FORMAT type)
{
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		meshes[i]->CreateShaderResourceView(device, fileName, type);
	}
}

void Model::CreateSamplerState(ID3D11Device* device)
{
	for (size_t i = 0; i < this->meshes.size(); i++)
	{
		meshes[i]->CreateSamplerState(device);
	}
}
