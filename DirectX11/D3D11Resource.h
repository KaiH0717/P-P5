#pragma once
#include "D3D11Manager.h"

class D3D11Resource
{
protected:
	ID3D11Device* const GetDevice(const D3D11Manager& d3d11Manager);
	ID3D11DeviceContext* const GetDeviceContext(const D3D11Manager& d3d11Manager);
public:
	virtual void Bind(const D3D11Manager& d3d11Manager) = 0;
	virtual ~D3D11Resource() = default;
};

