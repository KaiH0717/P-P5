#pragma once
#include "D3D11Renderer.h"

namespace
{
	constexpr float fps = 1.0f / 30.0f;
	constexpr size_t MAX_MODEL_VERTEX_COUNT = 4096 << 3;
	constexpr size_t MAX_DEBUG_VERTEX_COUNT = 4096;
	namespace Color
	{
		constexpr XMFLOAT4 RED = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		constexpr XMFLOAT4 GREEN = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		constexpr XMFLOAT4 BLUE = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		constexpr XMFLOAT4 WHITE = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	namespace DefaultVector
	{
		constexpr XMVECTOR LEFT = { 1.0f, 0.0f, 0.0f, 0.0f };
		constexpr XMVECTOR UP = { 0.0f, 1.0f, 0.0f, 0.0f };
		constexpr XMVECTOR FORWARD = { 0.0f, 0.0f, 1.0f, 0.0f };
	}
}

struct ColoredVertex
{
	XMFLOAT4 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;
};

class D3D11DebugRenderer : public D3D11Renderer
{
	typedef enum DEBUG_TYPE
	{
		DEBUG,
		DEBUGMODEL
	} DEBUG_TYPE;
private:
	// for various geometry
	ID3D11Buffer* pDebugVertexBuffer;
	size_t debugVertexCount;
	ColoredVertex* pDebugVertices;
	// for models
	ID3D11Buffer* pDebugModelVertexBuffer;
	size_t debugModelVertexCount;
	ColoredVertex* pDebugModelVertices;
private:
	void UpdateDebugPipeline(DEBUG_TYPE type);
	void AddLine(const XMVECTOR& aPos, const XMVECTOR& bPos, const XMVECTOR& aNormal, const XMVECTOR& bNormal, const XMFLOAT4& aColor, const XMFLOAT4& bColor, DEBUG_TYPE type);
	void AddLine(const XMFLOAT4& aPos, const XMFLOAT4& bPos, const XMFLOAT3& aNormal, const XMFLOAT3& bNormal, const XMFLOAT4& aColor, const XMFLOAT4& bColor, DEBUG_TYPE type);
	void AddWorldMatrixAxis(const XMMATRIX& worldMatrix);
	void CreateGrid(unsigned int width, unsigned int height);
public:
	D3D11DebugRenderer();
	~D3D11DebugRenderer() = default;

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
	void Update();
	//----------------------------------------
	// All draw/vertex/index/pipeline setup calls should happen here
	//----------------------------------------
	void Render();
	//----------------------------------------
	// Ends current frame and present the back buffer
	//----------------------------------------
	void EndFrame();
	//****************************************
};

