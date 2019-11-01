#include "D3D11Resource.h"

ID3D11Device* const D3D11Resource::GetDevice(const D3D11Manager& d3d11Manager)
{
	return d3d11Manager.pDevice.Get();
}

ID3D11DeviceContext* const D3D11Resource::GetDeviceContext(const D3D11Manager& d3d11Manager)
{
	return d3d11Manager.pDeviceContext.Get();
}
