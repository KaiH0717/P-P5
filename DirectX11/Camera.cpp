#include "Camera.h"

Camera::Camera()
{
	FOV = 90.0f;
	aspectRatio = 1.0f;
	nearPlane = 0.1f;
	farPlane = 10.0f;
	epsilon = 0.0000001f;
	XMStoreFloat4x4(&view, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&world, XMMatrixInverse(nullptr, XMLoadFloat4x4(&view)));
	XMStoreFloat4x4(&projection, XMMatrixIdentity());

	ZeroMemory(&pCurrentCursorPosition, sizeof(pCurrentCursorPosition));
	ZeroMemory(&pPreviousCursorPosition, sizeof(pPreviousCursorPosition));
	GetCursorPos(&pCurrentCursorPosition);
	GetCursorPos(&pPreviousCursorPosition);
}

float Camera::GetFOV() const { return FOV; }
float Camera::GetAspectRatio() const { return aspectRatio; }
float Camera::GetNearPlane() const { return nearPlane; }
float Camera::GetFarPlane() const { return farPlane; }
XMMATRIX Camera::GetWorldMatrix() const { return XMLoadFloat4x4(&world); }
XMMATRIX Camera::GetViewMatrix() const { return XMLoadFloat4x4(&view); }
XMMATRIX Camera::GetProjectionMatrix() const { return XMLoadFloat4x4(&projection); }
XMMATRIX Camera::GetViewProjectionMatrix() const { return XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix()); }
XMFLOAT4 Camera::GetWorldPosition() const { return XMFLOAT4(world._41, world._42, world._43, 1.0f); }
XMFLOAT4 Camera::GetLocalPosition() const { return XMFLOAT4(view._41, view._42, view._43, 1.0f); }
XMVECTOR Camera::GetWorldPositionVector() const { return GetWorldMatrix().r[3]; }
XMVECTOR Camera::GetLocalPositionVector() const { return GetViewMatrix().r[3]; }

void Camera::SetFOV(float fov) { this->FOV = fov; SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane); }
void Camera::SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio + epsilon; SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane); }
void Camera::SetNearPlane(float nearPlane) { this->nearPlane = nearPlane; SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane); }
void Camera::SetFarPlane(float farPlane) { this->farPlane = farPlane; SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane); }
void Camera::SetProjection(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	this->FOV = FOV;
	this->aspectRatio = aspectRatio + epsilon;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(this->FOV), this->aspectRatio, this->nearPlane, this->farPlane);
	XMStoreFloat4x4(&projection, projectionMatrix);
}

void Camera::SetPosition(float x, float y, float z)
{
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMVectorSet(x, y, z, 0.0f), XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMStoreFloat4x4(&view, viewMatrix);
	XMMATRIX worldMatrix = XMMatrixInverse(nullptr, viewMatrix);
	XMStoreFloat4x4(&world, worldMatrix);
}

void Camera::IncreaseFOV(float offset)
{
	this->FOV += offset;
	if (this->FOV >= 135.0f)
		this->FOV = 135.0f;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::DecreaseFOV(float offset)
{
	this->FOV -= offset;
	if (this->FOV <= 45.0f)
		this->FOV = 45.0f;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::IncreaseNearPlane(float offset)
{
	this->nearPlane += offset;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::DecreaseNearPlane(float offset)
{
	this->nearPlane -= offset;
	if (this->nearPlane < epsilon)
		this->nearPlane = epsilon;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::IncreaseFarPlane(float offset)
{
	this->farPlane += offset;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::DecreaseFarPlane(float offset)
{
	this->farPlane -= offset;
	if (this->farPlane < 2.0f)
		this->farPlane = 2.0f;
	SetProjection(this->FOV, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::UpdateView()
{
	XMMATRIX localViewMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&view, localViewMatrix);
}

void Camera::MoveX(float offset)
{
	XMMATRIX translation = XMMatrixTranslation(offset, 0.0f, 0.0f);
	XMMATRIX worldViewMatrix = XMMatrixMultiply(translation, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::MoveY(float offset)
{
	XMMATRIX translation = XMMatrixTranslation(0.0f, offset, 0.0f);
	XMMATRIX worldViewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&world), translation);
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::MoveZ(float offset)
{
	XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, offset);
	XMMATRIX worldViewMatrix = XMMatrixMultiply(translation, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::Pitch(float degrees)
{
	// local rotation
	XMMATRIX rotation = XMMatrixRotationX(degrees);
	XMMATRIX worldViewMatrix = XMMatrixMultiply(rotation, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::Yaw(float degrees)
{
	// global rotation
	XMMATRIX rotation = XMMatrixRotationY(degrees);
	XMMATRIX worldViewMatrix = XMLoadFloat4x4(&world);
	XMVECTOR originalPos = worldViewMatrix.r[3];
	worldViewMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	worldViewMatrix = XMMatrixMultiply(worldViewMatrix, rotation);
	worldViewMatrix.r[3] = originalPos;
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::Roll(float degrees)
{
	// local rotation
	XMMATRIX rotation = XMMatrixRotationZ(degrees);
	XMMATRIX worldViewMatrix = XMMatrixMultiply(rotation, XMLoadFloat4x4(&world));
	XMStoreFloat4x4(&world, worldViewMatrix);
}

void Camera::LookAt(const XMVECTOR& target)
{
	XMMATRIX worldViewMatrix = XMMatrixLookAtLH(XMLoadFloat4(&this->GetWorldPosition()), target, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	worldViewMatrix = XMMatrixInverse(nullptr, worldViewMatrix);
	XMStoreFloat4x4(&world, worldViewMatrix);
}

XMMATRIX Camera::LookAt(const XMVECTOR& target, const XMMATRIX& worldMatrix, const XMVECTOR& up)
{
	XMVECTOR zVector = target - worldMatrix.r[3];
	zVector = XMVector4Normalize(zVector);

	XMVECTOR xVector = XMVector4Normalize(XMVector3Cross(zVector, up));
	XMVECTOR yVector = XMVector4Normalize(XMVector3Cross(xVector, zVector));
	XMMATRIX lookAt = XMMatrixIdentity();
	lookAt.r[0] = xVector;
	lookAt.r[1] = yVector;
	lookAt.r[2] = zVector;
	lookAt.r[3] = worldMatrix.r[3];
	return lookAt;
}

XMMATRIX Camera::TurnTo(const XMVECTOR& target, const XMMATRIX& worldMatrix, float sensitivity, float delta)
{
	XMVECTOR zVector = target - worldMatrix.r[3];
	zVector = XMVector4Normalize(zVector);

	XMMATRIX turnTo = worldMatrix;
	// take the dot product of z and cameras row1(xvector) to get the turn rate
	float yTurnRate = sensitivity * delta * XMVector3Dot(zVector, worldMatrix.r[0]).m128_f32[0];
	// perform operation on global rotation
	XMVECTOR originalPosition = turnTo.r[3];
	turnTo.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	turnTo = XMMatrixMultiply(turnTo, XMMatrixRotationY(yTurnRate));
	turnTo.r[3] = originalPosition;

	// take the dot product of z and cameras row2(yvector) to get the turn rate
	float xTurnRate = sensitivity * delta * XMVector3Dot(zVector, worldMatrix.r[1]).m128_f32[0] * -1.0f;
	turnTo = XMMatrixMultiply(XMMatrixRotationX(xTurnRate), turnTo);

	// Orthonormalize
	turnTo.r[0] = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), turnTo.r[2]));
	turnTo.r[1] = XMVector3Normalize(XMVector3Cross(turnTo.r[2], turnTo.r[0]));

	return turnTo;
}

void Camera::Update(float delta)
{
	// move forward
	if (GetAsyncKeyState('W'))
	{
		this->MoveZ(translateSpeed * delta);
	}
	// move left
	if (GetAsyncKeyState('A'))
	{
		this->MoveX(-translateSpeed * delta);
	}
	// move backwards
	if (GetAsyncKeyState('S'))
	{
		this->MoveZ(-translateSpeed * delta);
	}
	// move right
	if (GetAsyncKeyState('D'))
	{
		this->MoveX(translateSpeed * delta);
	}
	// move up
	if (GetAsyncKeyState('Q'))
	{
		this->MoveY(translateSpeed * delta);
	}
	// move down
	if (GetAsyncKeyState('E'))
	{
		this->MoveY(-translateSpeed * delta);
	}
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		pPreviousCursorPosition.x = pCurrentCursorPosition.x;
		pPreviousCursorPosition.y = pCurrentCursorPosition.y;
		GetCursorPos(&pCurrentCursorPosition);
		int xDelta = pCurrentCursorPosition.x - pPreviousCursorPosition.x;
		int yDelta = pCurrentCursorPosition.y - pPreviousCursorPosition.y;
		this->Yaw((float)xDelta * delta);
		this->Pitch((float)yDelta * delta);
	}
	else
	{
		GetCursorPos(&pCurrentCursorPosition);
		GetCursorPos(&pPreviousCursorPosition);
	}
	this->UpdateView();
}
