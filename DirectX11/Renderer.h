#pragma once
#include <Windows.h>
#include "XTime.h"
#include "Enumerations.h"
#include "SceneManager.h"

//----------------------------------------
// Base class for all renderers
//----------------------------------------
class Renderer
{
protected:
	// 'shared pointer'
	HWND windowHandle;
public:
	Renderer() : windowHandle(nullptr) {}
	virtual ~Renderer() = default;

	//----------------------------------------
	// This function inits protected members for derived class use
	// MUST BE CALLED
	//----------------------------------------
	inline void SetWindowHandle(HWND hwnd) { windowHandle = hwnd; }

	//----------------------------------------
	// All inherited class must have its own
	// implementations
	//----------------------------------------
	virtual void Initialize() = 0;
	virtual void ShutDown() = 0;

	//****************************************
	// These 3 functions should be called in order!
	//****************************************

	//----------------------------------------
	// All updates should happen here (Ex: time, constant buffer etc..)
	//----------------------------------------
	virtual void Update() = 0;

	//----------------------------------------
	// All draw/vertex/index/pipeline setup calls should happen here
	//----------------------------------------
	virtual void Render() = 0;

	//----------------------------------------
	// Ends current frame and present the back buffer
	//----------------------------------------
	virtual void EndFrame() = 0;

	//----------------------------------------
	// Reset RTV, DST, and viewport
	//----------------------------------------
	virtual void Resize() = 0;
};