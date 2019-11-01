#pragma once
#include "Renderer.h"
#include "D3D11Manager.h"
#include "ShaderManager.h"
#include "ConstantBuffer.h"

struct Matrix_Constant_Buffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 viewProjection;
	XMFLOAT4 time; // x = delta, y = smooth delta, z = elapsed time
	XMFLOAT4 cameraPosition;
};

struct Joint_Matrix_Constant_Buffer
{
	XMFLOAT4X4 worldMatrices[32];
};

struct Material_Constant_Buffer
{
	XMFLOAT4 diffuse;
	XMFLOAT4 emissive;
	XMFLOAT4 specular;
};

struct Light_Constant_Buffer
{
	// [0] = directional lighting, [1] = point lighting, [2] = spot lighting
	XMFLOAT4 lightPos[3];
	XMFLOAT4 lightNormal[3];
	XMFLOAT4 lightColor[3];
	// x = radius, y = rotation, z = time
	XMFLOAT4 lightRadius;
};

class D3D11Renderer : public Renderer
{
	// Protected variable for debug renderer access
protected:
	D3D11Manager d3d11Manager;
	SceneManager sceneManager;

	WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	WRL::ComPtr<ID3D11RasterizerState> pRasterizerState;

	D3D11_VIEWPORT viewPort[VIEW_PORT_TYPE::COUNT];
	ID3D11InputLayout* pInputLayout[INPUT_LAYOUT_TYPE::COUNT] = { nullptr, nullptr };
	XTime time[TIME_TYPE::COUNT];
	double elapsedTime[TIME_TYPE::COUNT];
	float interpolatedTime = 0.0f;
	unsigned int frameCounter = 0;

	Matrix_Constant_Buffer matrixConstantBuffer;
	Joint_Matrix_Constant_Buffer jointMatrixConstantBuffer;
	Material_Constant_Buffer materialConstantBuffer;
	Light_Constant_Buffer lightConstantBuffer;

	VSConstantBuffer<Matrix_Constant_Buffer> gpuVSMatrixCB;
	VSConstantBuffer<Joint_Matrix_Constant_Buffer> gpuVSJointMatrixCB;
	PSConstantBuffer<Material_Constant_Buffer> gpuPSMaterialCB;
	PSConstantBuffer<Light_Constant_Buffer> gpuPSLightCB;
protected:
	//----------------------------------------
	// Helper functions for that is called in Init
	//----------------------------------------
	void CreateDefaultViewPort();
	void CreateDefaultRenderTargetView();
	void CreateDefaultDepthStencilView();
	void CreateDefaultRasterizer();
	void CreateInputLayouts();
	void CreateConstantBuffers();
	HRESULT CreateBuffer(ID3D11Buffer** buffer, UINT bindFlag, UINT byteWidth, const void* pSysMem);
	void UpdateConstantBuffers(Model* model, std::vector<Light>& lights);
public:
	D3D11Renderer();
	~D3D11Renderer() = default;

	//----------------------------------------
	// MUST BE CALLED
	//----------------------------------------
	void Initialize();
	void ShutDown();

	//****************************************
	// These 3 functions should be called in order!
	//----------------------------------------
	// All updates should happen here (Ex: time, constant buffer etc..)
	//----------------------------------------
	virtual void Update();
	//----------------------------------------
	// All draw/vertex/index/pipeline setup calls should happen here
	//----------------------------------------
	virtual void Render();
	//----------------------------------------
	// Ends current frame and present the back buffer
	//----------------------------------------
	virtual void EndFrame();
	//****************************************

	//----------------------------------------
	// Reset RTV, DST, and viewport
	//----------------------------------------
	void Resize();
};

