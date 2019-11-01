#include "Light.h"

Light::Light()
{
	position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	normal = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&world, XMMatrixIdentity());
}

Light::Light(const XMVECTOR& position, const XMVECTOR& normal, const XMVECTOR& color, const XMMATRIX& world)
{
	XMStoreFloat4(&this->position, position);
	XMStoreFloat4(&this->normal, normal);
	XMStoreFloat4(&this->color, color);
	XMStoreFloat4x4(&this->world, world);
}

const XMVECTOR Light::GetPositionVector() const { return XMLoadFloat4(&position); }
const XMVECTOR Light::GetNormalVector() const { return XMLoadFloat4(&normal); }
const XMVECTOR Light::GetNormalVectorNormalized() const { return XMVector4Normalize(this->GetNormalVector()); }
const XMVECTOR Light::GetColor() const { return XMLoadFloat4(&color); }
const XMMATRIX Light::GetWorldMatrix() const { return XMLoadFloat4x4(&world); }

void Light::SetPosition(const XMVECTOR& position) { XMStoreFloat4(&this->position, position); }
void Light::SetPosition(float x, float y, float z) { this->position = XMFLOAT4(x, y, z, 1.0f); }
void Light::SetNormal(const XMVECTOR& normal) { XMStoreFloat4(&this->normal, normal); }
void Light::SetNormal(float x, float y, float z) { this->normal = XMFLOAT4(x, y, z, 0.0f); }
void Light::SetColor(const XMVECTOR& color) { XMStoreFloat4(&this->color, color); }
void Light::SetColor(float r, float g, float b, float a) { this->color = XMFLOAT4(r, g, b, a); }
void Light::SetWorldMatrix(const XMMATRIX& world) { XMStoreFloat4x4(&this->world, world); }

void Light::UpdatePosition()
{
	XMVECTOR positionVector = XMVector4Transform(XMLoadFloat4(&position), XMLoadFloat4x4(&world));
	XMStoreFloat4(&position, positionVector);
	XMVECTOR normalVector = XMVector4Transform(XMLoadFloat4(&normal), XMLoadFloat4x4(&world));
	XMStoreFloat4(&normal, normalVector);
	XMMATRIX worldMatrix = GetWorldMatrix();
	worldMatrix.r[3] = positionVector;
	XMStoreFloat4x4(&world, worldMatrix);
}
