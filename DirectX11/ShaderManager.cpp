#include "ShaderManager.h"
#include <fstream>

void ShaderManager::SetDevice(ID3D11Device* pDevice)
{
	this->pDevice = pDevice;
}

ShaderManager::ShaderManager()
{
	this->pDevice = nullptr;
}

ShaderManager::~ShaderManager()
{
	this->pDevice = nullptr;
	UnloadAll();
}

BOOL ShaderManager::ReadCSOFromFile(const char* pFilePath, char** ppOutByteCode, size_t& outByteCodeSize)
{
	std::ifstream fin;
	fin.open(pFilePath, std::ios_base::binary);
	if (!fin.is_open())
		return FALSE;
	fin.seekg(0, std::ios_base::end);
	outByteCodeSize = size_t(fin.tellg());
	*ppOutByteCode = new char[outByteCodeSize];
	fin.seekg(0, std::ios_base::beg);
	fin.read(*ppOutByteCode, outByteCodeSize);
	fin.close();
	return TRUE;
}

void ShaderManager::AddVertexShader(VS_TYPE type, const char* pCSOFileName)
{
	char* pByteCode = nullptr;
	size_t byteCodeSize = 0;
	BOOL success = ReadCSOFromFile(pCSOFileName, &pByteCode, byteCodeSize);
	if (success)
	{
		HRESULT hr = pDevice->CreateVertexShader(pByteCode, byteCodeSize, nullptr, &this->pVShader[(unsigned int)type]);
		delete[] pByteCode;
	}
}

void ShaderManager::GetVertexShader(VS_TYPE type, ID3D11VertexShader** pOutVShader)
{
	*pOutVShader = this->pVShader[(unsigned int)type];
}

void ShaderManager::UnloadVertexShader()
{
	for (unsigned int i = 0; i < (unsigned int)VS_TYPE::COUNT; ++i)
		if (this->pVShader[i]) { this->pVShader[i]->Release(); this->pVShader[i] = nullptr; }
}

void ShaderManager::AddPixelShader(PS_TYPE type, const char* pCSOFileName)
{
	char* pByteCode = nullptr;
	size_t byteCodeSize = 0;
	BOOL success = ReadCSOFromFile(pCSOFileName, &pByteCode, byteCodeSize);
	if (success)
	{
		HRESULT hr = pDevice->CreatePixelShader(pByteCode, byteCodeSize, nullptr, &pPShader[(unsigned int)type]);
		delete[] pByteCode;
	}
}

void ShaderManager::GetPixelShader(PS_TYPE type, ID3D11PixelShader** pOutPShader)
{
	*pOutPShader = this->pPShader[(unsigned int)type];
}

void ShaderManager::UnloadPixelShader()
{
	for (unsigned int i = 0; i < (unsigned int)PS_TYPE::COUNT; ++i)
		if (this->pPShader[i]) { this->pPShader[i]->Release(); this->pPShader[i] = nullptr; }
}

void ShaderManager::UnloadAll()
{
	UnloadVertexShader();
	UnloadPixelShader();
}
