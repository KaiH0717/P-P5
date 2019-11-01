#include "D3D11Manager.h"
#include <assert.h>

D3D11Manager::D3D11Manager()
{
	windowWidth = windowHeight = 0;
	aspectRatio = 0.0f;
	vSync = false;
	windowHandle = nullptr;
	pDevice = nullptr;
	pDeviceContext = nullptr;
	pSwapChain = nullptr;
}

unsigned int D3D11Manager::GetWindowWidth() const { return windowWidth; }
unsigned int D3D11Manager::GetWindowHeight() const { return windowHeight; }
float D3D11Manager::GetAspectRatio() const { return aspectRatio; }
bool D3D11Manager::IsVSyncEnabled() const { return vSync; }

void D3D11Manager::SetVSync(bool enable) { this->vSync = enable; }

void D3D11Manager::Initialize(HWND hwnd, bool enableVSync)
{
	windowHandle = hwnd;
	vSync = enableVSync;
	assert(windowHandle);
	HRESULT hr = S_OK;
	// Grab client area
	RECT rect;
	GetClientRect(windowHandle, &rect);

	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	// Setup swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Setup feature levels
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Setup device flag
	D3D11_CREATE_DEVICE_FLAG deviceFlag = D3D11_CREATE_DEVICE_FLAG(0);
#ifdef _DEBUG
	deviceFlag = D3D11_CREATE_DEVICE_FLAG(deviceFlag | D3D11_CREATE_DEVICE_DEBUG);
#endif

	// Create device and swap chain
	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlag,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		pSwapChain.GetAddressOf(),
		pDevice.GetAddressOf(),
		nullptr,
		pDeviceContext.GetAddressOf()
	);
	assert(!FAILED(hr));
}

void D3D11Manager::ShutDown()
{
	windowWidth = windowHeight = 0;
	aspectRatio = 0.0f;
	vSync = false;
	windowHandle = nullptr;
	pDevice.Reset();
	pDeviceContext.Reset();
	pSwapChain.Reset();
}

void D3D11Manager::EndFrame()
{
	if (vSync)
		pSwapChain->Present(1, 0);
	else
		pSwapChain->Present(0, 0);
}

void D3D11Manager::Resize(ID3D11RenderTargetView** ppOutRTV, ID3D11DepthStencilView** ppOutDSV)
{
	// Recreating rtv and dsv, so release
	if ((*ppOutRTV) && (*ppOutDSV))
	{
		(*ppOutRTV)->Release();
		(*ppOutDSV)->Release();
	}
	else return;
	HRESULT hr = S_OK;
	// Grab client area
	RECT rect;
	GetClientRect(windowHandle, &rect);
	// Update current window width/height/aspect ratio
	windowWidth = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;
	aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	// Resize swap chain buffer and target
	pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	//--------------------
	// Recreate render target view
	//--------------------
	// Obtain pointer to the back buffer of swap chain and create our main render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), reinterpret_cast<void**>(&pBackBuffer));
	assert(!FAILED(hr));

	// Create main render target view
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, ppOutRTV);
	assert(!FAILED(hr));
	pBackBuffer->Release();
	//--------------------
	// Recreate depth stencil view
	//--------------------
	// Depth buffer
	ID3D11Texture2D* pDepthBuffer; // temporary depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = windowWidth;
	depthBufferDesc.Height = windowHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create depth buffer
	hr = pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pDepthBuffer);
	assert(!FAILED(hr));

	// Depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view
	hr = pDevice->CreateDepthStencilView(pDepthBuffer, &depthStencilViewDesc, ppOutDSV);
	assert(!FAILED(hr));

	pDepthBuffer->Release();
}

void D3D11Manager::ReportLiveObjects()
{
#if _DEBUG
	ID3D11Debug* debugger = nullptr;
	HRESULT hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugger));
	if (SUCCEEDED(hr))
		hr = debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (debugger) { debugger->Release(); debugger = nullptr; }
#endif
}
