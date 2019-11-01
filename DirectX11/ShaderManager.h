#pragma once
// includes for directx 11 api interface
#pragma comment (lib, "D3D11.lib")
#include <d3d11.h>

#include "Enumerations.h"
// Base class/interface for all shader managers
class ShaderManager
{
private:
	// 'shared_ptr'
	ID3D11Device* pDevice;
	
	ID3D11VertexShader* pVShader[(unsigned int)VS_TYPE::COUNT] = { nullptr };
	ID3D11PixelShader* pPShader[(unsigned int)PS_TYPE::COUNT] = { nullptr };
public:
	ShaderManager();
	~ShaderManager();

	void SetDevice(ID3D11Device* pDevice);
	BOOL ReadCSOFromFile(const char* pFilePath, char** ppOutByteCode, size_t& outByteCodeSize);

	//----------------------------------------
	// Vertex Shader functions
	//----------------------------------------
	void AddVertexShader(VS_TYPE type, const char* pCSOFileName);
	void GetVertexShader(VS_TYPE type, ID3D11VertexShader** pOutVShader);
	void UnloadVertexShader();

	//----------------------------------------
	// Pixel Shader functions
	//----------------------------------------
	void AddPixelShader(PS_TYPE type, const char* pCSOFileName);
	void GetPixelShader(PS_TYPE type, ID3D11PixelShader** pOutPShader);
	void UnloadPixelShader();

	//----------------------------------------
	// Unloads all shaders, is called in destructor
	// to ensure no memory leak, may be called
	// during run-time.
	//----------------------------------------
	void UnloadAll();
};

