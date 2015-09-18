// Pg1_raycast_hyao4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Pg1_raycast_hyao4.h"
#include "model_obj.h"
#include "Triangle.h";
#include "Utils.h"
#include "Space.h"
#include "RayCasting.h"
#include "LocalIllumination.h"
#include <stdio.h>
#include <tchar.h>
#include <string.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

/******************************************My Variables Begin Here***************************************/
static HWND sHwnd;
//Color

bool defaultWinSize = true;
bool defaultLightLocation = true;
bool enableIllumination = true;
bool enableShadow = true;
bool enableScale = true;

Space					g_space;
float					pixelXLength = 256.0f;
float					pixelYLength = 256.0f;
float					coordXLength = 4.0f;
float					coordYLength = 4.0f;
std::vector<float>      viewportCenter = { 0.0f, 0.0f, -2.0f };
std::vector<float>		light = { 0.0f, 5.0f, 0.0f };
std::vector<float>		eye = { 0.0f, 0.0f, -4.0f };

//std::vector<Triangle> triangles;
RayCasting				g_casting;
LocalIllumination		illumination;
/******************************************My Variables End Here****************************************/

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

/************************************My Functions Declaration Begin Here*******************************/
bool    LoadModel(const char *pszFilename);
void SetWindowHandle(HWND hwnd);
void SetPixel(int x, int y, COLORREF& color);
void Draw();
void ReadConfig();
void ReadWindowConfig();
void AddLights(Space &g_space);

/************************************My Functions Declaration End Here*********************************/


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PG1_RAYCAST_HYAO4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PG1_RAYCAST_HYAO4));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PG1_RAYCAST_HYAO4));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_PG1_RAYCAST_HYAO4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	ReadConfig();

	ReadWindowConfig();

	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = pixelXLength;
	rect.bottom = pixelYLength;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		rect.left + 100, rect.top + 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);




	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetWindowHandle(hWnd);
		Draw();
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void SetWindowHandle(HWND hwnd)
{
	sHwnd = hwnd;
}

void SetPixel(int x, int y, COLORREF& color)
{
	if (sHwnd == NULL)
	{
		exit(0);
	}
	HDC hdc = GetDC(sHwnd);
	SetPixel(hdc, x, y, color);
	ReleaseDC(sHwnd, hdc);
	return;
}

void DrawPixel(ModelOBJ::Vertex eyeVertex, float x, float y)
{
	float transformedX, transformedY = 0;

	if (viewportCenter[2] > eyeVertex.position[2])
	{
		transformedX = viewportCenter[0] - coordXLength / 2 + x / ((pixelXLength - 1) / coordXLength);
		transformedY = viewportCenter[1] + coordYLength / 2 - y / ((pixelYLength - 1) / coordYLength);
	}
	else
	{
		transformedX = viewportCenter[0] + coordXLength / 2 - x / ((pixelXLength - 1) / coordXLength);
		transformedY = viewportCenter[1] + coordYLength / 2 - y / ((pixelYLength - 1) / coordYLength);
	}

	ModelOBJ::Vertex viewVertex;
	viewVertex.position[0] = transformedX;
	viewVertex.position[1] = transformedY;
	viewVertex.position[2] = viewportCenter[2];

	Ray sightRay = Ray(eyeVertex, viewVertex);

	PixelInfo pixelInfo
		= g_casting.Cast(eyeVertex, sightRay, g_space, illumination, enableIllumination, enableShadow);

	if (pixelInfo.visible)
	{
		SetPixel(x, y, pixelInfo.color);
	}


}

void Draw()
{
	char fileName[255] = ".\\inputs\\model.obj";

	if (LoadModel(fileName))
	{
		ModelOBJ::Vertex eyeVtx;
		eyeVtx.position[0] = eye[0];
		eyeVtx.position[1] = eye[1];
		eyeVtx.position[2] = eye[2];

		for (int y = 0; y < pixelXLength; y++)
		{
			for (int x = 0; x < pixelYLength; x++)
			{
				DrawPixel(eyeVtx, x, y);

			}
		}
	}
}



bool LoadModel(const char *pszFilename)
{
	// Import the OBJ file and normalize to unit length.

	SetCursor(LoadCursor(0, IDC_WAIT));

	if (!g_space.Load(pszFilename, enableScale))
	{
		MessageBox(NULL, L".obj file or .mtl file is missing!\n"
			L"Please put the file in the same folder of the program.", L"Error", MB_OK);

		SetCursor(LoadCursor(0, IDC_ARROW));
		//throw std::runtime_error("Failed to load model.");
		return false;
	}

	AddLights(g_space);



	return true;
	//g_space.AddLight(3, 0, 0, 0.5, 0.5, 0.5, 255.0f, 255.0f, 255.0f);
	//triangles = space.getTriangles();
}

void ReadConfig()
{
	char configFileName[255] = ".\\inputs\\config.txt";

	FILE *pConfigFile = fopen(configFileName, "r");
	if (!pConfigFile)
	{
		MessageBox(NULL, L"config.txt file is not found in the same folder of the program!\n"
			L"The effect ofshadow and local illumination will be enabled.", L"Warning", MB_OK);
	}
	else
	{
		defaultWinSize = false;
		char paramName[256] = { 0 };
		char paramValue[256] = { 0 };
		while (
			fscanf(pConfigFile, "%s", &paramName) != EOF)
		{
			if (paramName[0] == '#')
			{
				fgets(paramName, sizeof(paramName), pConfigFile);
				continue;
			}
			std::string paramNameStr = paramName;

			if (paramNameStr.compare("local_illumination") == 0)
			{
				fscanf(pConfigFile, "%s", &paramValue);
				std::string paramValueStr = paramValue;

				if (paramValueStr.compare("disabled") == 0)
				{
					enableIllumination = false;
				}
			}
			else if (paramNameStr.compare("shadow") == 0)
			{
				fscanf(pConfigFile, "%s", &paramValue);
				std::string paramValueStr = paramValue;

				if (paramValueStr.compare("disabled") == 0)
				{
					enableShadow = false;
				}
			}
			else if (paramNameStr.compare("eye") == 0)
			{
				fscanf(pConfigFile, "%f %f %f", &eye[0], &eye[1], &eye[2]);
			}
			else if (paramNameStr.compare("viewCenter") == 0)
			{
				fscanf(pConfigFile, "%f %f %f", &viewportCenter[0], &viewportCenter[1], &viewportCenter[2]);
			}
			else if (paramNameStr.compare("lenOfXinCoord") == 0)
			{
				fscanf(pConfigFile, "%f", &coordXLength);
			}
			else if (paramNameStr.compare("lenOfYinCoord") == 0)
			{
				fscanf(pConfigFile, "%f", &coordYLength);
			}
			else if (paramNameStr.compare("scale") == 0)
			{
				fscanf(pConfigFile, "%s", &paramValue);
				std::string paramValueStr = paramValue;

				if (paramValueStr.compare("disabled") == 0)
				{
					enableScale = false;
				}
			}

		}
	}
}

void ReadWindowConfig()
{
	char windowFileName[255] = ".\\inputs\\window.txt";

	FILE *pWindowFile = fopen(windowFileName, "r");
	if (!pWindowFile)
	{
		MessageBox(NULL, L"window.txt file is not found in the same folder of the program!\n"
			L"The default window size (256*256) will be used", L"Warning", MB_OK);
	}
	else
	{
		defaultWinSize = false;
		char buffer[256] = { 0 };
		//fscanf(pWindowFile, "%f %f", &pixelXLength, &pixelYLength);

		while (
			fscanf(pWindowFile, "%s", buffer) != EOF)
		{
			if (buffer[0] == '#')
			{
				fgets(buffer, sizeof(buffer), pWindowFile);
				continue;
			}
			sscanf(buffer, "%f", &pixelXLength);
			fscanf(pWindowFile, "%f", &pixelYLength);
			break;
		}
	}
}

void AddLights(Space &g_space)
{
	char lightFileName[255] = ".\\inputs\\lights.txt";

	FILE *pLightFile = fopen(lightFileName, "r");
	if (!pLightFile)
	{
		MessageBox(NULL, L"lights.txt file is not found in the same folder of the program!\n"
			L"The default light location (0,5,0) will be used", L"Warning", MB_OK);

		float ia[3] = { 1, 1, 1 };
		float id[3] = { 1, 1, 1 };
		float is[3] = { 1, 1, 1 };
		g_space.AddLight(light[0], light[1], light[2], ia, id, is);
	}
	else
	{
		char buffer[255];
		float lightX = 0.0f;
		float lightY = 0.0f;
		float lightZ = 0.0f;
		float ri = 0.0f;
		float gi = 0.0f;
		float bi = 0.0f;
		defaultLightLocation = false;
		while (
			fscanf(pLightFile, "%s", buffer) != EOF)
		{
			if (buffer[0] == '#')
			{
				fgets(buffer, sizeof(buffer), pLightFile);
				continue;
			}
			sscanf(buffer, "%f", &lightX);
			fscanf(pLightFile, "%f %f %f %f %f", &lightY, &lightZ, &ri, &gi, &bi);
			float ia[3] = { ri, gi, bi };
			float id[3] = { ri, gi, bi };
			float is[3] = { ri, gi, bi };
			g_space.AddLight(lightX, lightY, lightZ, ia, id, is);
		}

	}
}