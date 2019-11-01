#include "SceneManager.h"

void SceneManager::AddCamera()
{
}

void SceneManager::AddLight()
{
}

void SceneManager::LoadModel(ID3D11Device* device, const char** fileName, float scale)
{
	Model* model = new Model;
	model->LoadFBXModel(device, fileName, scale);
	model->CreateVertexBuffer(device);
	model->CreateIndexBuffer(device);
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	shaderManager.GetVertexShader(VS_TYPE::DEFAULT, &vs);
	shaderManager.GetPixelShader(PS_TYPE::DEFAULT, &ps);
	model->SetVertexShader(vs);
	model->SetPixelShader(ps);
	//model->SetWorldMatrix(XMMatrixRotationY(XMConvertToRadians(180.0f)));
	model->SetWorldMatrix(XMMatrixIdentity());
	models.push_back(model);
}

void SceneManager::Initialize(ID3D11Device* pDevice)
{
	HRESULT hr = CoInitialize(NULL);
	this->pDevice = pDevice;
	shaderManager.SetDevice(pDevice);
	// shader set up
	shaderManager.AddVertexShader(VS_TYPE::DEBUG, "DebugVertexShader.cso");
	shaderManager.AddPixelShader(PS_TYPE::DEBUG, "DebugPixelShader.cso");
	shaderManager.AddVertexShader(VS_TYPE::DEBUGMODEL, "DebugModelVertexShader.cso");
	shaderManager.AddPixelShader(PS_TYPE::DEBUGMODEL, "DebugModelPixelShader.cso");
	shaderManager.AddVertexShader(VS_TYPE::DEFAULT, "VertexShader.cso");
	shaderManager.AddPixelShader(PS_TYPE::DEFAULT, "PixelShader.cso");
	// camera setup
	Camera camera;
	camera.SetPosition(0.0f, 5.0f, -10.0f);
	camera.SetProjection(90.0f, 1.0f, 0.1f, 100.0f);
	camera.SetAspectRatio(1.0f);
	cameras.push_back(camera);
	// model loading
	//const char* filePaths[] = { "../OutputFiles/Idle.KAIVERTEX", "../OutputFiles/Idle.KAIMATERIAL", "../OutputFiles/Idle.KAIANIMATION" };
	const char* filePaths[] = { "../OutputFiles/Run.KAIVERTEX", "../OutputFiles/Run.KAIMATERIAL", "../OutputFiles/Run.KAIANIMATION" };
	LoadModel(this->pDevice, filePaths);
	// light setup
	Light dLight;
	Light pLight;
	// directional light init
	dLight.SetPosition(0.0f, 0.0f, 0.0f);
	dLight.SetNormal(-0.577f, -0.577f, 0.577f);
	dLight.SetColor(0.45f, 0.45f, 0.64f, 1.0f);
	// point light init
	pLight.SetPosition(2.5f, 5.0f, 0.0f);
	pLight.SetNormal(0.0f, 0.0f, 0.0f);
	pLight.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	lights.push_back(dLight);
	lights.push_back(pLight);
}

void SceneManager::ShutDown()
{
	CoUninitialize();
	this->pDevice = nullptr;
	shaderManager.UnloadAll();
	UnloadScene();
}

void SceneManager::UnloadScene()
{
	shaderManager.UnloadAll();
	cameras.clear();
	lights.clear();
	for (size_t i = 0; i < models.size(); i++)
	{
		delete models[i];
	}
	models.clear();
}

