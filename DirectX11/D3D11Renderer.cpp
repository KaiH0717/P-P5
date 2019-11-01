#include "D3D11Renderer.h"

void D3D11Renderer::CreateDefaultViewPort()
{
	// Setup viewport
	viewPort[VIEW_PORT_TYPE::DEFAULT].Width = static_cast<FLOAT>(d3d11Manager.windowWidth);
	viewPort[VIEW_PORT_TYPE::DEFAULT].Height = static_cast<FLOAT>(d3d11Manager.windowHeight);
	viewPort[VIEW_PORT_TYPE::DEFAULT].MinDepth = 0.0f;
	viewPort[VIEW_PORT_TYPE::DEFAULT].MaxDepth = 1.0f;
	viewPort[VIEW_PORT_TYPE::DEFAULT].TopLeftX = 0.0f;
	viewPort[VIEW_PORT_TYPE::DEFAULT].TopLeftY = 0.0f;
}

void D3D11Renderer::CreateDefaultRenderTargetView()
{
	HRESULT hr = S_OK;
	// Obtain pointer to the back buffer of swap chain and create our main render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = d3d11Manager.pSwapChain->GetBuffer(0, __uuidof(pBackBuffer), reinterpret_cast<void**>(&pBackBuffer));
	assert(!FAILED(hr));

	// Create main render target view
	hr = d3d11Manager.pDevice->CreateRenderTargetView(pBackBuffer, NULL, pRenderTargetView.GetAddressOf());
	assert(!FAILED(hr));
	pBackBuffer->Release();
}

void D3D11Renderer::CreateDefaultDepthStencilView()
{
	HRESULT hr = S_OK;
	// Depth buffer
	ID3D11Texture2D* pDepthBuffer; // temporary depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = (UINT)viewPort[VIEW_PORT_TYPE::DEFAULT].Width;
	depthBufferDesc.Height = (UINT)viewPort[VIEW_PORT_TYPE::DEFAULT].Height;
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
	hr = d3d11Manager.pDevice->CreateTexture2D(&depthBufferDesc, nullptr, &pDepthBuffer);
	assert(!FAILED(hr));

	// Depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view
	hr = d3d11Manager.pDevice->CreateDepthStencilView(pDepthBuffer, &depthStencilViewDesc, pDepthStencilView.GetAddressOf());
	assert(!FAILED(hr));

	pDepthBuffer->Release();

	// Depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	// Create depth stencil state
	hr = d3d11Manager.pDevice->CreateDepthStencilState(&depthStencilStateDesc, pDepthStencilState.GetAddressOf());
	assert(!FAILED(hr));
}

void D3D11Renderer::CreateDefaultRasterizer()
{
	HRESULT hr = S_OK;
	// Rasterizer state
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = false;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	// Create rasterizer state
	hr = d3d11Manager.pDevice->CreateRasterizerState(&rasterizerDesc, pRasterizerState.GetAddressOf());
	assert(!FAILED(hr));
}

void D3D11Renderer::CreateInputLayouts()
{
	HRESULT hr = S_OK;
	// Input layout
	D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{"POSITION"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	  ,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	  ,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	  ,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT	  ,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT		  ,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES"	, 0, DXGI_FORMAT_R32G32B32A32_UINT	  , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHT"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	  , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	char* byteCode = nullptr;
	size_t byteLength = 0;
	sceneManager.shaderManager.ReadCSOFromFile("VertexShader.cso", &byteCode, byteLength);
	// Create input layout
	hr = d3d11Manager.pDevice->CreateInputLayout(vertexLayout, ARRAYSIZE(vertexLayout), byteCode, byteLength, &pInputLayout[INPUT_LAYOUT_TYPE::VERTEX]);
	delete[] byteCode;
	byteCode = nullptr;
	assert(!FAILED(hr));

	// Colored vertex input layout
	D3D11_INPUT_ELEMENT_DESC coloredVertexLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT	  , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	sceneManager.shaderManager.ReadCSOFromFile("DebugVertexShader.cso", &byteCode, byteLength);
	// Create input layout for colored vertex
	hr = d3d11Manager.pDevice->CreateInputLayout(coloredVertexLayout, ARRAYSIZE(coloredVertexLayout), byteCode, byteLength, &pInputLayout[INPUT_LAYOUT_TYPE::COLOREDVERTEX]);
	delete[] byteCode;
	byteCode = nullptr;
	assert(!FAILED(hr));
}

void D3D11Renderer::CreateConstantBuffers()
{
	gpuVSMatrixCB.Initialize(d3d11Manager, matrixConstantBuffer, 0);
	gpuVSJointMatrixCB.Initialize(d3d11Manager, jointMatrixConstantBuffer, 1);
	gpuPSMaterialCB.Initialize(d3d11Manager, materialConstantBuffer, 0);
	gpuPSLightCB.Initialize(d3d11Manager, lightConstantBuffer, 1);
}

HRESULT D3D11Renderer::CreateBuffer(ID3D11Buffer** buffer, UINT bindFlag, UINT byteWidth, const void* pSysMem)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subRsrcData;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subRsrcData, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	subRsrcData.pSysMem = pSysMem;
	switch (bindFlag)
	{
	case D3D11_BIND_VERTEX_BUFFER:
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		return d3d11Manager.pDevice->CreateBuffer(&bufferDesc, &subRsrcData, buffer);
	case D3D11_BIND_INDEX_BUFFER:
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		return d3d11Manager.pDevice->CreateBuffer(&bufferDesc, &subRsrcData, buffer);
	case D3D11_BIND_CONSTANT_BUFFER:
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		return d3d11Manager.pDevice->CreateBuffer(&bufferDesc, nullptr, buffer);
	case D3D11_BIND_SHADER_RESOURCE:
		return E_NOTIMPL;
	case D3D11_BIND_STREAM_OUTPUT:
		return E_NOTIMPL;
	case D3D11_BIND_RENDER_TARGET:
		return E_NOTIMPL;
	case D3D11_BIND_DEPTH_STENCIL:
		return E_NOTIMPL;
	case D3D11_BIND_UNORDERED_ACCESS:
		return E_NOTIMPL;
	case D3D11_BIND_DECODER:
		return E_NOTIMPL;
	case D3D11_BIND_VIDEO_ENCODER:
		return E_NOTIMPL;
	default:
		return S_FALSE;
	}
}

void D3D11Renderer::UpdateConstantBuffers(Model* model, std::vector<Light>& lights)
{
	// matrix constant buffer udpate
	XMMATRIX viewProjection = sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].GetViewMatrix() * sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].GetProjectionMatrix();
	if (model)
		XMStoreFloat4x4(&matrixConstantBuffer.world, model->GetWorldMatrix());
	else
		XMStoreFloat4x4(&matrixConstantBuffer.world, XMMatrixIdentity());
	XMStoreFloat4x4(&matrixConstantBuffer.viewProjection, viewProjection);
	XMStoreFloat4(&matrixConstantBuffer.time, XMVectorSet((float)time[TIME_TYPE::DEFAULT].Delta(), (float)time[TIME_TYPE::DEFAULT].SmoothDelta(), (float)elapsedTime[TIME_TYPE::DEFAULT], 0.0f));
	XMStoreFloat4(&matrixConstantBuffer.cameraPosition, sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].GetWorldPositionVector());
	// material constant buffer update
	if (model)
	{
		materialConstantBuffer.diffuse = XMFLOAT4(model->GetMaterials()[0][Phong::Diffuse].color.x, model->GetMaterials()[0][Phong::Diffuse].color.y, model->GetMaterials()[0][Phong::Diffuse].color.z, model->GetMaterials()[0][Phong::Diffuse].factor);
		materialConstantBuffer.emissive = XMFLOAT4(model->GetMaterials()[0][Phong::Emissive].color.x, model->GetMaterials()[0][Phong::Emissive].color.y, model->GetMaterials()[0][Phong::Emissive].color.z, model->GetMaterials()[0][Phong::Emissive].factor);
		materialConstantBuffer.specular = XMFLOAT4(model->GetMaterials()[0][Phong::Specular].color.x, model->GetMaterials()[0][Phong::Specular].color.y, model->GetMaterials()[0][Phong::Specular].color.z, model->GetMaterials()[0][Phong::Specular].factor);
	}
	// light constant buffer update
	//lights[0].SetWorldMatrix(XMMatrixRotationY(0.01f));
	lights[0].SetWorldMatrix(XMMatrixIdentity());
	lights[0].UpdatePosition();
	XMStoreFloat4(&lightConstantBuffer.lightPos[0], lights[0].GetPositionVector());
	XMStoreFloat4(&lightConstantBuffer.lightNormal[0], lights[0].GetNormalVectorNormalized());
	XMStoreFloat4(&lightConstantBuffer.lightColor[0], lights[0].GetColor());
	lights[1].SetWorldMatrix(XMMatrixRotationY(0.01f));
	lights[1].UpdatePosition();
	XMStoreFloat4(&lightConstantBuffer.lightPos[1], lights[1].GetPositionVector());
	XMStoreFloat4(&lightConstantBuffer.lightNormal[1], lights[1].GetNormalVectorNormalized());
	XMStoreFloat4(&lightConstantBuffer.lightColor[1], lights[1].GetColor());
	float radius = 9.0f;
	XMVECTOR lightRad = XMVectorSet(radius, 0.0f, (float)elapsedTime[TIME_TYPE::DEFAULT], 0.0f);
	XMStoreFloat4(&lightConstantBuffer.lightRadius, lightRad);
	for (size_t j = 0; j < ARRAYSIZE(jointMatrixConstantBuffer.worldMatrices); j++)
	{
		XMStoreFloat4x4(&jointMatrixConstantBuffer.worldMatrices[j], XMMatrixIdentity());
	}
	if (model)
	{
		for (size_t i = 0; i < model->GetMeshes().size(); i++)
		{
			Mesh* currentMesh = sceneManager.models[0]->GetMeshes()[i];
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
				newMat = XMMatrixMultiply(XMLoadFloat4x4(&currentMesh->inverseBindMatrices[j]), newMat);
				XMStoreFloat4x4(&jointMatrixConstantBuffer.worldMatrices[j], newMat);
				if (elapsedTime[TIME_TYPE::DEFAULT] > currentFrameTime &&
					(elapsedTime[TIME_TYPE::DEFAULT] < nextFrameTime || nextFrameCounter == 1))
				{
					interpolatedTime = ((float)elapsedTime[TIME_TYPE::DEFAULT] - currentFrameTime) / abs(nextFrameTime - currentFrameTime);
					frameCounter++;
				}
			}
		}
	}

	// map and unmap for cb
	gpuVSMatrixCB.Update(d3d11Manager, matrixConstantBuffer);
	gpuVSJointMatrixCB.Update(d3d11Manager, jointMatrixConstantBuffer);
	gpuPSMaterialCB.Update(d3d11Manager, materialConstantBuffer);
	gpuPSLightCB.Update(d3d11Manager, lightConstantBuffer);
}

D3D11Renderer::D3D11Renderer()
{
	pRenderTargetView = nullptr;
	pDepthStencilView = nullptr;
	pDepthStencilState = nullptr;
	pRasterizerState = nullptr;
	ZeroMemory(&viewPort[VIEW_PORT_TYPE::DEFAULT], sizeof(viewPort[VIEW_PORT_TYPE::DEFAULT]));
}

void D3D11Renderer::Initialize()
{
	d3d11Manager.Initialize(windowHandle, true);
	sceneManager.Initialize(d3d11Manager.pDevice.Get());
	CreateDefaultViewPort();
	CreateDefaultRenderTargetView();
	CreateDefaultDepthStencilView();
	CreateDefaultRasterizer();
	CreateInputLayouts();
	CreateConstantBuffers();
	// Pipeline setup
	d3d11Manager.pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	d3d11Manager.pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);
	d3d11Manager.pDeviceContext->RSSetState(pRasterizerState.Get());
	d3d11Manager.pDeviceContext->RSSetViewports(1, &viewPort[VIEW_PORT_TYPE::DEFAULT]);
	// time
	time[TIME_TYPE::DEFAULT].Restart();
	elapsedTime[TIME_TYPE::DEFAULT] = 0;
}

void D3D11Renderer::ShutDown()
{
	pRenderTargetView.Reset();
	pDepthStencilView.Reset();
	pDepthStencilState.Reset();
	pRasterizerState.Reset();
	if (pInputLayout[INPUT_LAYOUT_TYPE::VERTEX]) { pInputLayout[INPUT_LAYOUT_TYPE::VERTEX]->Release(); }
	if (pInputLayout[INPUT_LAYOUT_TYPE::COLOREDVERTEX]) { pInputLayout[INPUT_LAYOUT_TYPE::COLOREDVERTEX]->Release(); }
	sceneManager.ShutDown();
	d3d11Manager.ReportLiveObjects();
	d3d11Manager.ShutDown();
}

void D3D11Renderer::Update()
{
	time[TIME_TYPE::DEFAULT].Signal();
	elapsedTime[TIME_TYPE::DEFAULT] += time[TIME_TYPE::DEFAULT].Delta();
	sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].SetAspectRatio(d3d11Manager.aspectRatio);
	sceneManager.cameras[VIEW_PORT_TYPE::DEFAULT].Update((float)time[TIME_TYPE::DEFAULT].Delta());
	for (size_t i = 0; i < sceneManager.models.size(); i++)
	{
		UpdateConstantBuffers(sceneManager.models[i], sceneManager.lights);
	}
	// set cb
	gpuVSMatrixCB.Bind(d3d11Manager);
	gpuVSJointMatrixCB.Bind(d3d11Manager);
	gpuPSMaterialCB.Bind(d3d11Manager);
	gpuPSLightCB.Bind(d3d11Manager);
}

void D3D11Renderer::Render()
{
	d3d11Manager.pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	// Update input layout
	d3d11Manager.pDeviceContext->IASetInputLayout(pInputLayout[INPUT_LAYOUT_TYPE::VERTEX]);
	// Update primitive topology
	d3d11Manager.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Clear depth buffer and render target
	d3d11Manager.pDeviceContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	const float bg_Color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	d3d11Manager.pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), bg_Color);

	// Draw calls here
	for (size_t i = 0; i < sceneManager.models.size(); i++)
	{
		sceneManager.models[i]->DrawIndexed(d3d11Manager.pDeviceContext.Get(), &viewPort[VIEW_PORT_TYPE::DEFAULT]);
	}
}

void D3D11Renderer::EndFrame()
{
	d3d11Manager.EndFrame();
}

void D3D11Renderer::Resize()
{
	d3d11Manager.Resize(pRenderTargetView.GetAddressOf(), pDepthStencilView.GetAddressOf());
	// Resize viewport viewport
	viewPort[VIEW_PORT_TYPE::DEFAULT].Width = static_cast<FLOAT>(d3d11Manager.windowWidth);
	viewPort[VIEW_PORT_TYPE::DEFAULT].Height = static_cast<FLOAT>(d3d11Manager.windowHeight);
}

