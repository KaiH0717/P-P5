#pragma once
#include "D3D11Resource.h"

template <class CBufferType>
class ConstantBuffer : public D3D11Resource
{
protected:
	WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	unsigned int slot;
public:
	ConstantBuffer() : pConstantBuffer(nullptr), slot(0) {};

	void Initialize(const D3D11Manager& d3d11Manager, const CBufferType& cBuffer, unsigned int slot)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(cBuffer);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		GetDevice(d3d11Manager)->CreateBuffer(&desc, nullptr, pConstantBuffer.GetAddressOf());
		this->slot = slot;
	}

	void Update(const D3D11Manager& d3d11Manager, const CBufferType& cBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE gpuBuffer;
		GetDeviceContext(d3d11Manager)->Map(pConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
		memcpy(gpuBuffer.pData, &cBuffer, sizeof(CBufferType));
		GetDeviceContext(d3d11Manager)->Unmap(pConstantBuffer.Get(), 0);
	}
};

template <class CBufferType>
class VSConstantBuffer : public ConstantBuffer<CBufferType>
{
	using ConstantBuffer<CBufferType>::pConstantBuffer;
	using ConstantBuffer<CBufferType>::slot;
	using ConstantBuffer<CBufferType>::GetDeviceContext;
public:
	void Bind(const D3D11Manager& d3d11Manager)
	{
		GetDeviceContext(d3d11Manager)->VSSetConstantBuffers(slot, 1, pConstantBuffer.GetAddressOf());
	}
};

template <class CBufferType>
class PSConstantBuffer : public ConstantBuffer<CBufferType>
{
	using ConstantBuffer<CBufferType>::pConstantBuffer;
	using ConstantBuffer<CBufferType>::slot;
	using ConstantBuffer<CBufferType>::GetDeviceContext;
public:
	void Bind(const D3D11Manager& d3d11Manager)
	{
		GetDeviceContext(d3d11Manager)->PSSetConstantBuffers(slot, 1, pConstantBuffer.GetAddressOf());
	}
};