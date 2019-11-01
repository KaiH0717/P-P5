#include <string>
#include "FBXExporter.h"
#include "../FBXBinary/FBXBinaryFileIO.h"
#define _WIN32_WINNT 0x0500
#include <windows.h>

int main(int argc, char** argv)
{
	// disable resize
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	const char* outputFileExt[] = { ".KAIVERTEX", ".KAIMATERIAL", ".KAIANIMATION" };
	const char* fileName = "Box_Idle";

	std::string readPath = "../Assets/";
	readPath += fileName; readPath += ".fbx";

	std::string writePath[3] = { "../OutputFiles/", "../OutputFiles/", "../OutputFiles/" };
	writePath[0] += fileName; writePath[0] += outputFileExt[0];
	writePath[1] += fileName; writePath[1] += outputFileExt[1];
	writePath[2] += fileName; writePath[2] += outputFileExt[2];

	FBXBinaryFileIO fileIO;
	FBXExporter exporter;
	exporter.Initialize();
	if (exporter.LoadScene(readPath.data()))
	{
		//exporter.LoadSceneInfo();
		exporter.LoadGlobalLightSettings();
		exporter.LoadGlobalCameraSettings();
		exporter.LoadGlobalTimeSettings();
		exporter.LoadSceneHierarchy();
		exporter.LoadSceneContent();
		exporter.LoadScenePose();
		exporter.LoadSceneAnimation();
		exporter.SaveData(fileIO);
		fileIO.Write(writePath[0].data(), writePath[1].data(), writePath[2].data());
		fileIO.Read(writePath[0].data(), writePath[1].data(), writePath[2].data());
	}
	exporter.ShutDown();
	return 0;
}
