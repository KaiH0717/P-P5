#pragma once
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Enumerations.h"
#include "ShaderManager.h"
#include <vector>

class SceneManager
{
	friend class D3D11DebugRenderer;
	friend class D3D11Renderer;
private:
	ID3D11Device* pDevice;
	ShaderManager shaderManager;
	std::vector<Camera> cameras;
	std::vector<Light> lights;
	std::vector<Model*> models;
private:
	void AddCamera();
	void AddLight();
	void LoadModel(ID3D11Device* device, const char** fileName, float scale = 1.0f);
public:
	SceneManager() : pDevice(nullptr) {};
	~SceneManager() = default;

	void Initialize(ID3D11Device* pDevice);
	void ShutDown();

	void UnloadScene();
};

