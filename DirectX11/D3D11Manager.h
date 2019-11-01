#pragma once
#include <wrl.h>
using namespace Microsoft;
// Include DirectX11 and DirectXMath for interface and math operation access
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class D3D11Manager
{
	friend class D3D11DebugRenderer;
	friend class D3D11Renderer;
	friend class D3D11Resource;
private:
	unsigned int windowWidth;
	unsigned int windowHeight;
	float aspectRatio;
	bool vSync;

	// HWND is 'shared pointer'
	HWND windowHandle;
	WRL::ComPtr<ID3D11Device> pDevice;
	WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
	WRL::ComPtr<IDXGISwapChain> pSwapChain;
public:
	D3D11Manager();
	~D3D11Manager() = default;

	//--------------------
	// Getters
	//--------------------
	unsigned int GetWindowWidth() const;
	unsigned int GetWindowHeight() const;
	float GetAspectRatio() const;
	bool IsVSyncEnabled() const;
	
	//--------------------
	// Setters
	// No setters for width, height, and aspect ratio
	// They will be handled in resize function
	//--------------------
	void SetVSync(bool enable);

	//--------------------
	// Initializes d3dmanager
	//--------------------
	void Initialize(HWND hwnd, bool enableVSync);
	void ShutDown();

	//--------------------
	// Present the back buffer
	// Utilizes vSync
	//--------------------
	void EndFrame();

	//--------------------
	// Recreates any resource referring to the swapchain
	// Should be called in the case of window resize
	// Also updates width/height/aspect ratio
	//--------------------
	void Resize(ID3D11RenderTargetView** ppOutRTV, ID3D11DepthStencilView** ppOutDSV);

	//--------------------
	// Can be called for debugging purpose
	// Must be called before shutdown
	//--------------------
	void ReportLiveObjects();
};

