#include "D3D11DebugRenderer.h"

namespace
{
	float RandomNumber(float min, float max)
	{
		return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX))* (max - min)) + min;
	}
}

void D3D11DebugRenderer::UpdateDebugPipeline(DEBUG_TYPE type)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subRsrcData, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	UINT strides = sizeof(ColoredVertex);
	UINT offsets = 0;
	switch (type)
	{
	case DEBUG_TYPE::DEBUG:
	{
		if (pDebugVertexBuffer) { pDebugVertexBuffer->Release(); pDebugVertexBuffer = nullptr; }
		bufferDesc.ByteWidth = sizeof(ColoredVertex) * (UINT)MAX_DEBUG_VERTEX_COUNT;
		subRsrcData.pSysMem = pDebugVertices;
		d3d11Manager.pDevice->CreateBuffer(&bufferDesc, &subRsrcData, &pDebugVertexBuffer);
		// set shader
		sceneManager.shaderManager.GetVertexShader(VS_TYPE::DEBUG, &vs);
		sceneManager.shaderManager.GetPixelShader(PS_TYPE::DEBUG, &ps);
		d3d11Manager.pDeviceContext->IASetVertexBuffers(0, 1, &pDebugVertexBuffer, &strides, &offsets);
	}
	break;
	case DEBUG_TYPE::DEBUGMODEL:
	{
		if (pDebugModelVertexBuffer) { pDebugModelVertexBuffer->Release(); pDebugModelVertexBuffer = nullptr; }
		bufferDesc.ByteWidth = sizeof(ColoredVertex) * (UINT)MAX_MODEL_VERTEX_COUNT;
		subRsrcData.pSysMem = pDebugModelVertices;
		d3d11Manager.pDevice->CreateBuffer(&bufferDesc, &subRsrcData, &pDebugModelVertexBuffer);
		// set shader
		sceneManager.shaderManager.GetVertexShader(VS_TYPE::DEBUGMODEL, &vs);
		sceneManager.shaderManager.GetPixelShader(PS_TYPE::DEBUGMODEL, &ps);
		d3d11Manager.pDeviceContext->IASetVertexBuffers(0, 1, &pDebugModelVertexBuffer, &strides, &offsets);
	}
	break;
	}
	d3d11Manager.pDeviceContext->VSSetShader(vs, nullptr, 0);
	d3d11Manager.pDeviceContext->PSSetShader(ps, nullptr, 0);
}

void D3D11DebugRenderer::AddLine(const XMVECTOR& aPos, const XMVECTOR& bPos, const XMVECTOR& aNormal, const XMVECTOR& bNormal, const XMFLOAT4& aColor, const XMFLOAT4& bColor, DEBUG_TYPE type)
{
	ColoredVertex a;
	ColoredVertex b;
	XMStoreFloat4(&a.position, aPos);
	XMStoreFloat4(&b.position, bPos);
	XMStoreFloat3(&a.normal, aNormal);
	XMStoreFloat3(&b.normal, bNormal);
	a.color = aColor;
	b.color = bColor;
	switch (type)
	{
	case DEBUG_TYPE::DEBUG:
	{
		pDebugVertices[debugVertexCount++] = a;
		pDebugVertices[debugVertexCount++] = b;
	}
	break;
	case DEBUG_TYPE::DEBUGMODEL:
	{
		pDebugModelVertices[debugModelVertexCount++] = a;
		pDebugModelVertices[debugModelVertexCount++] = b;
	}
	break;
	}
}

void D3D11DebugRenderer::AddLine(const XMFLOAT4& aPos, const XMFLOAT4& bPos, const XMFLOAT3& aNormal, const XMFLOAT3& bNormal, const XMFLOAT4& aColor, const XMFLOAT4& bColor, DEBUG_TYPE type)
{
	AddLine(XMLoadFloat4(&aPos), XMLoadFloat4(&bPos), XMLoadFloat3(&aNormal), XMLoadFloat3(&bNormal), aColor, bColor, type);
}

void D3D11DebugRenderer::AddWorldMatrixAxis(const XMMATRIX& worldMatrix)
{
	XMVECTOR xAxis, yAxis, zAxis;
	xAxis = XMVector4Transform(XMVectorSet(0.5f, 0.0f, 0.0f, 1.0f), worldMatrix);
	yAxis = XMVector4Transform(XMVectorSet(0.0f, 0.5f, 0.0f, 1.0f), worldMatrix);
	zAxis = XMVector4Transform(XMVectorSet(0.0f, 0.0f, 0.5f, 1.0f), worldMatrix);
	AddLine(xAxis, worldMatrix.r[3], DefaultVector::LEFT, DefaultVector::LEFT, Color::RED, Color::RED, DEBUG_TYPE::DEBUG);
	AddLine(yAxis, worldMatrix.r[3], DefaultVector::UP, DefaultVector::UP, Color::GREEN, Color::GREEN, DEBUG_TYPE::DEBUG);
	AddLine(zAxis, worldMatrix.r[3], DefaultVector::FORWARD, DefaultVector::FORWARD, Color::BLUE, Color::BLUE, DEBUG_TYPE::DEBUG);
}

void D3D11DebugRenderer::CreateGrid(unsigned int width, unsigned int height)
{
	// make sure we're always drawing at the origin
	float ndcOffset = static_cast<float>(width >> 1);
	float x = 0.0f;
	float z = 0.0f;
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	// horizontal lines, +1 for the extra line accounted for
	for (unsigned int horizontalLineDrawn = 0; horizontalLineDrawn < height + 1; ++horizontalLineDrawn, ++z)
	{
		AddLine(XMFLOAT4(x - ndcOffset, 0.0f, z - ndcOffset, 1.0f), XMFLOAT4(x + ndcOffset, 0.0f, z - ndcOffset, 1.0f), up, up, Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUG);
	}
	x = 0.0f;
	z = 0.0f;
	// vertical lines, +1 for the extra line accounted for
	for (unsigned int verticalLineDrawn = 0; verticalLineDrawn < width + 1; ++verticalLineDrawn, ++x)
	{
		AddLine(XMFLOAT4(x - ndcOffset, 0.0f, z - ndcOffset, 1.0f), XMFLOAT4(x - ndcOffset, 0.0f, z + ndcOffset, 1.0f), up, up, Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUG);
	}
}

D3D11DebugRenderer::D3D11DebugRenderer()
{
	pDebugVertexBuffer = nullptr;
	debugVertexCount = 0;
	pDebugVertices = new ColoredVertex[MAX_DEBUG_VERTEX_COUNT];

	pDebugModelVertexBuffer = nullptr;
	debugModelVertexCount = 0;
	pDebugModelVertices = new ColoredVertex[MAX_MODEL_VERTEX_COUNT];
}

void D3D11DebugRenderer::Initialize()
{
	D3D11Renderer::Initialize();
	HRESULT hr = CreateBuffer(&pDebugVertexBuffer, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER, sizeof(ColoredVertex) * MAX_DEBUG_VERTEX_COUNT, pDebugVertices);
	hr = CreateBuffer(&pDebugModelVertexBuffer, D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER, sizeof(ColoredVertex) * MAX_MODEL_VERTEX_COUNT, pDebugModelVertices);
}

void D3D11DebugRenderer::ShutDown()
{
	if (pDebugVertexBuffer) { pDebugVertexBuffer->Release(); pDebugVertexBuffer = nullptr; }
	debugVertexCount = 0;
	delete[] pDebugVertices;

	if (pDebugModelVertexBuffer) { pDebugModelVertexBuffer->Release(); pDebugModelVertexBuffer = nullptr; }
	debugModelVertexCount = 0;
	delete[] pDebugModelVertices;
	D3D11Renderer::ShutDown();
}

void D3D11DebugRenderer::Update()
{
	time[TIME_TYPE::DEFAULT].Signal();
	elapsedTime[TIME_TYPE::DEFAULT] += time[TIME_TYPE::DEFAULT].Delta();
	sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].SetAspectRatio(d3d11Manager.aspectRatio);
	sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].Update((float)time[TIME_TYPE::DEFAULT].Delta());
	UpdateConstantBuffers(nullptr, sceneManager.lights);
	this->AddWorldMatrixAxis(sceneManager.lights[0].GetWorldMatrix());
	this->AddWorldMatrixAxis(sceneManager.lights[1].GetWorldMatrix());
	CreateGrid(10, 10);
	for (size_t i = 0; i < sceneManager.models[0]->GetMeshes().size(); i++)
	{
		// connect lines for each vertex indicies
		Mesh* currentMesh = sceneManager.models[0]->GetMeshes()[i];
		for (size_t j = 0; j < currentMesh->indexCount; j += 3)
		{
			AddLine(currentMesh->vertices[currentMesh->indices[j]].position, currentMesh->vertices[currentMesh->indices[j + 1]].position,
				currentMesh->vertices[currentMesh->indices[j]].normal, currentMesh->vertices[currentMesh->indices[j + 1]].normal,
				Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUGMODEL);
			AddLine(currentMesh->vertices[currentMesh->indices[j]].position, currentMesh->vertices[currentMesh->indices[j + 2]].position,
				currentMesh->vertices[currentMesh->indices[j]].normal, currentMesh->vertices[currentMesh->indices[j + 2]].normal,
				Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUGMODEL);
			AddLine(currentMesh->vertices[currentMesh->indices[j + 1]].position, currentMesh->vertices[currentMesh->indices[j + 2]].position,
				currentMesh->vertices[currentMesh->indices[j + 1]].normal, currentMesh->vertices[currentMesh->indices[j + 2]].normal,
				Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUGMODEL);
		}
		// draw bones
		for (size_t j = 0; j < currentMesh->jointCount; j++)
		{
			AddWorldMatrixAxis(XMLoadFloat4x4(&currentMesh->joints[j].world));
			if (currentMesh->joints[j].parentIndex == -1)
				continue;
			XMVECTOR currentJointPosition = XMLoadFloat4x4(&currentMesh->joints[j].world).r[3];
			XMVECTOR parentJointPosition = XMLoadFloat4x4(&currentMesh->joints[currentMesh->joints[j].parentIndex].world).r[3];
			AddLine(currentJointPosition, parentJointPosition, DefaultVector::UP, DefaultVector::UP, Color::WHITE, Color::WHITE, DEBUG_TYPE::DEBUG);
		}
		for (size_t j = 0; j < currentMesh->jointCount; j++)
		{
			if (elapsedTime[TIME_TYPE::DEFAULT] >= currentMesh->animClip.duration)
				elapsedTime[TIME_TYPE::DEFAULT] = 0.0f;
			if (frameCounter >= currentMesh->animClip.frameCount)
				frameCounter = 1;
			// find current frame counter
			unsigned int nextFrameCounter = frameCounter + 1 >= currentMesh->animClip.frameCount ? 1 : frameCounter + 1;
			double currentFrameTime = currentMesh->animClip.frames[frameCounter].time;
			double nextFrameTime = currentMesh->animClip.frames[nextFrameCounter].time;
			XMMATRIX previousMat = XMLoadFloat4x4(&currentMesh->animClip.frames[frameCounter].jointTransforms[j]);
			XMMATRIX animMatrix = XMLoadFloat4x4(&currentMesh->animClip.frames[nextFrameCounter].jointTransforms[j]);
			XMMATRIX newMat;
			newMat.r[0] = XMVectorLerp(previousMat.r[0], animMatrix.r[0], interpolatedTime);
			newMat.r[1] = XMVectorLerp(previousMat.r[1], animMatrix.r[1], interpolatedTime);
			newMat.r[2] = XMVectorLerp(previousMat.r[2], animMatrix.r[2], interpolatedTime);
			newMat.r[3] = XMVectorLerp(previousMat.r[3], animMatrix.r[3], interpolatedTime);
			XMStoreFloat4x4(&currentMesh->joints[j].world, newMat);
			if (elapsedTime[TIME_TYPE::DEFAULT] > currentFrameTime &&
				(elapsedTime[TIME_TYPE::DEFAULT] < nextFrameTime || nextFrameCounter == 1))
			{
				interpolatedTime = ((float)elapsedTime[TIME_TYPE::DEFAULT] - currentFrameTime) / abs(nextFrameTime - currentFrameTime);
				frameCounter++;
			}
		}
	}
	// set cb
	gpuVSMatrixCB.Bind(d3d11Manager);
	gpuPSMaterialCB.Bind(d3d11Manager);
	gpuPSLightCB.Bind(d3d11Manager);
}

void D3D11DebugRenderer::Render()
{
	d3d11Manager.pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	// Update input layout
	d3d11Manager.pDeviceContext->IASetInputLayout(pInputLayout[INPUT_LAYOUT_TYPE::COLOREDVERTEX]);
	// Update primitive topology
	d3d11Manager.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	// Clear depth buffer and render target
	d3d11Manager.pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	const float bg_Color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	d3d11Manager.pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), bg_Color);

	// Draw calls here
	UpdateDebugPipeline(DEBUG_TYPE::DEBUG);
	d3d11Manager.pDeviceContext->Draw((UINT)debugVertexCount, 0);
	debugVertexCount = 0;

	UpdateDebugPipeline(DEBUG_TYPE::DEBUGMODEL);
	d3d11Manager.pDeviceContext->Draw((UINT)debugModelVertexCount, 0);
	debugModelVertexCount = 0;
}

void D3D11DebugRenderer::EndFrame()
{
	d3d11Manager.EndFrame();
}

