#pragma once
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
private:
	// attributes
	float FOV;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	// used to ensure aspect ratio does not fall to 0
	float epsilon;

	//--------------------------------------
	// Camera's transform matrix
	//--------------------------------------
	XMFLOAT4X4 world;
	//--------------------------------------
	// Camera's inverse transform matrix
	//--------------------------------------
	XMFLOAT4X4 view;
	//--------------------------------------
	// Camera's projection matrix
	//--------------------------------------
	XMFLOAT4X4 projection;

	static constexpr float translateSpeed = 10.0f;
	static constexpr float rotationSpeed = 0.35f;

	POINT pPreviousCursorPosition;
	POINT pCurrentCursorPosition;
public:
	Camera();
	~Camera() = default;

	////////////////////////////////////////
	// projection matrix getters
	////////////////////////////////////////
	float GetFOV() const;
	float GetAspectRatio() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	XMMATRIX GetWorldMatrix() const;
	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjectionMatrix() const;
	XMMATRIX GetViewProjectionMatrix() const;
	XMFLOAT4 GetWorldPosition() const;
	XMFLOAT4 GetLocalPosition() const;
	XMVECTOR GetWorldPositionVector() const;
	XMVECTOR GetLocalPositionVector() const;

	//--------------------------------------
	// projection matrix setters
	//--------------------------------------
	void SetFOV(float fov);
	void SetAspectRatio(float aspectRatio);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

	//--------------------------------------
	// view matrix setters
	//--------------------------------------
	void SetPosition(float x, float y, float z);

	//--------------------------------------
	// projection matrix functions
	//--------------------------------------
	void IncreaseFOV(float offset);
	void DecreaseFOV(float offset);
	void IncreaseNearPlane(float offset);
	void DecreaseNearPlane(float offset);
	void IncreaseFarPlane(float offset);
	void DecreaseFarPlane(float offset);
	void UpdateView();

	//--------------------------------------
	// camera functions
	//--------------------------------------
	void MoveX(float offset);
	void MoveY(float offset);
	void MoveZ(float offset);
	void Pitch(float degrees);
	void Yaw(float degrees);
	void Roll(float degrees);
	void LookAt(const XMVECTOR& target);

	//--------------------------------------
	// static matrix construction functions
	//--------------------------------------
	static XMMATRIX LookAt(const XMVECTOR& target, const XMMATRIX& worldMatrix, const XMVECTOR& up);
	static XMMATRIX TurnTo(const XMVECTOR& target, const XMMATRIX& worldMatrix, float sensitivity, float delta);

	void Update(float delta);
};
