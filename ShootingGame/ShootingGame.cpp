//-> 追加分_
#define STRICT
#define DIRECTINPUT_VERSION 0x0800
#define _CRT_SECURE_NO_DEPRECATE
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif
//-> 追加分^
#include "stdafx.h"
#include <crtdbg.h>
#include <tchar.h>
#include <stdio.h>
#include <d2d1.h>
#include <dwrite.h>    //  文字表示に使用
#include "Selector.h"

#pragma comment(lib,"d2d1.lib")    //  direct2d に必要
#pragma comment(lib,"dwrite.lib")  //  文字表示に使用

//-> 追加分_
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <commctrl.h>
#include <basetsd.h>

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>
#include <assert.h>
#include <oleauto.h>
#include <shellapi.h>
#include "resource.h"

//-> 追加分^

#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

//-> 追加分_

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
BOOL CALLBACK    EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
BOOL CALLBACK    EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
HRESULT InitDirectInput(HWND hDlg);
VOID FreeDirectInput();
HRESULT UpdateInputState(HWND hDlg);

// Stuff to filter out XInput devices
#include <wbemidl.h>
HRESULT SetupForIsXInputDevice();
bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);
void CleanupForIsXInputDevice();


struct XINPUT_DEVICE_NODE
{
	DWORD dwVidPid;
	XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};

bool                    g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE*     g_pXInputDeviceList = nullptr;



//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }

LPDIRECTINPUT8          g_pDI = nullptr;
LPDIRECTINPUTDEVICE8    g_pJoystick = nullptr;

//-> 追加分^


//  グローバル変数
ID2D1Factory             *g_pD2DFactory = NULL;      //  D2D1Factoryオブジェクト
ID2D1HwndRenderTarget    *g_pRenderTarget = NULL;    //  描画ターゲット

double      g_dblDenominator;
double      g_dblFrame;
__int64     g_i64OldTimer;

#define FPS   30
#define INTERVAL   (1.0/FPS)

CSelector    *g_pSelector = NULL;  //  ゲームシステムオブジェクト

								   //
								   //  関数：WinMain
								   //  説明：アプリケーションのエントリーポイント
								   //
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CoInitialize(NULL);

	//  変数宣言
	WNDCLASSEX wcex;  //  ウィンドウクラス構造体
	HWND hWnd;        //  ウィンドウハンドル
	RECT    bounds, client;  //  RECT 構造体

							 //  (1)初期化部分

							 //  (1)-a ウィンドウクラスの登録
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;  //  ウィンドウプロシージャの登録
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;  //  アプリケーションインスタンス
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("ShootingGame");  //  ウィンドウクラス名
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	//  (1)-b ウィンドウの生成
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wcex.lpszClassName, _T("Shooting Game"),
		WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 300, 300, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;

	//  ウィンドウサイズの調整
	//  ここは無くても動く
	GetWindowRect(hWnd, &bounds);
	GetClientRect(hWnd, &client);
	MoveWindow(hWnd, bounds.left, bounds.top,
		300 * 2 - client.right,
		300 * 2 - client.bottom,
		false);

	//  ウィンドウの再表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//if (FAILED(InitDirectInput(hWnd)))
	//{
	//	MessageBox(nullptr, TEXT("Error Initializing DirectInput"),
	//		TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK);
	//	EndDialog(hWnd, 0);
	//}


	//  再表示の際、WndProc が呼び出される事にも注意！
	//  Direct2D 初期化
	{
		HRESULT hr;
		//  Direct2D Factory の生成
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
		GetClientRect(hWnd, &client);
		D2D1_SIZE_U size = D2D1::SizeU(
			client.right - client.left,
			client.bottom - client.top
		);

		//  Direct2D 描画ターゲットの生成
		hr = g_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&g_pRenderTarget
		);
	}

	//  ゲームシステムの初期化
	g_pSelector = new CSelector(g_pRenderTarget);

	//->追加分_
	if (FAILED(InitDirectInput(hWnd)))
	{
		MessageBox(nullptr, TEXT("Error Initializing DirectInput"),
			TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK);
		EndDialog(hWnd, 0);
	}

	// Set a timer to go off 30 times a second. At every timer message
	// the input device will be read
	SetTimer(hWnd, 0, 1000 / 30, nullptr);

	//->追加分^


	//  タイマーの初期化
	::QueryPerformanceCounter((LARGE_INTEGER*)&g_i64OldTimer);
	__int64   i64Tmp;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&i64Tmp);
	g_dblDenominator = 1.0 / (double)i64Tmp;
	g_dblFrame = 0.0f;
	//  (2)メッセージループ
	MSG        msg;
	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			double   t;
			::QueryPerformanceCounter((LARGE_INTEGER*)&i64Tmp);
			t = (i64Tmp - g_i64OldTimer) * g_dblDenominator;
			g_i64OldTimer = i64Tmp;
			g_dblFrame += t;

			if (g_dblFrame >= INTERVAL) {
				int   c = (int)(g_dblFrame / INTERVAL);
				g_dblFrame -= INTERVAL * c;

				if (g_pSelector)
					g_pSelector->doAnim();

				InvalidateRect(hWnd, NULL, false);  //  書き換えの実行
			}
		}
	}

	//  (3)終了処理
	SAFE_DELETE(g_pSelector);
	SAFE_RELEASE(g_pRenderTarget);
	SAFE_RELEASE(g_pD2DFactory);

	CoUninitialize();

	return (int)msg.wParam;
}

//
//  関数：WndProc
//  説明：ウインドウに渡されたイベントのハンドラ
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {

	case WM_PAINT:
		if (g_pRenderTarget) {
			g_pRenderTarget->BeginDraw();  //  描画開始

			g_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity()); //  Transform の設定

			g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));//  画面のクリア

			if (g_pSelector)
				g_pSelector->doDraw(g_pRenderTarget);

			g_pRenderTarget->EndDraw();  //  描画終了
		}
		ValidateRect(hWnd, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//->追加分_

	case WM_INITDIALOG:
		if (FAILED(InitDirectInput(hWnd)))
		{
			MessageBox(nullptr, TEXT("Error Initializing DirectInput"),
				TEXT("DirectInput Sample"), MB_ICONERROR | MB_OK);
			EndDialog(hWnd, 0);
		}

		// Set a timer to go off 30 times a second. At every timer message
		// the input device will be read
		SetTimer(hWnd, 0, 1000 / 30, nullptr);
		return TRUE;

	case WM_ACTIVATE:
		if (WA_INACTIVE != wParam && g_pJoystick)
		{
			// Make sure the device is acquired, if we are gaining focus.
			g_pJoystick->Acquire();
		}
		return TRUE;

	case WM_TIMER:
		// Update the input device every timer message
		if (FAILED(UpdateInputState(hWnd)))
		{
			KillTimer(hWnd, 0);
			MessageBox(nullptr, TEXT("Error Reading Input State. ") \
				TEXT("The sample will now exit."), TEXT("DirectInput Sample"),
				MB_ICONERROR | MB_OK);
			EndDialog(hWnd, TRUE);
		}
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}

		//->追加分^

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return    0;
}

//-> 追加分_


//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT InitDirectInput(HWND hDlg)
{
	HRESULT hr;

	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create a DInput object
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (VOID**)&g_pDI, nullptr)))
		return hr;


	if (g_bFilterOutXinputDevices)
		SetupForIsXInputDevice();

	DIJOYCONFIG PreferredJoyCfg = { 0 };
	DI_ENUM_CONTEXT enumContext;
	enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
	enumContext.bPreferredJoyCfgValid = false;

	IDirectInputJoyConfig8* pJoyConfig = nullptr;
	if (FAILED(hr = g_pDI->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&pJoyConfig)))
		return hr;

	PreferredJoyCfg.dwSize = sizeof(PreferredJoyCfg);
	if (SUCCEEDED(pJoyConfig->GetConfig(0, &PreferredJoyCfg, DIJC_GUIDINSTANCE))) // This function is expected to fail if no joystick is attached
		enumContext.bPreferredJoyCfgValid = true;
	SAFE_RELEASE(pJoyConfig);

	// Look for a simple joystick we can use for this sample program.
	if (FAILED(hr = g_pDI->EnumDevices(DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		&enumContext, DIEDFL_ATTACHEDONLY)))
		return hr;

	if (g_bFilterOutXinputDevices)
		CleanupForIsXInputDevice();

	// Make sure we got a joystick
	if (!g_pJoystick)
	{
		MessageBox(nullptr, TEXT("Joystick not found. The sample will now exit."),
			TEXT("DirectInput Sample"),
			MB_ICONERROR | MB_OK);
		EndDialog(hDlg, 0);
		return S_OK;
	}

	// Set the data format to "simple joystick" - a predefined data format 
	//
	// A data format specifies which controls on a device we are interested in,
	// and how they should be reported. This tells DInput that we will be
	// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	if (FAILED(hr = g_pJoystick->SetDataFormat(&c_dfDIJoystick2)))
		return hr;

	// Set the cooperative level to let DInput know how this device should
	// interact with the system and with other DInput applications.
	if (FAILED(hr = g_pJoystick->SetCooperativeLevel(hDlg, DISCL_EXCLUSIVE |
		DISCL_FOREGROUND)))
		return hr;

	// Enumerate the joystick objects. The callback function enabled user
	// interface elements for objects that are found, and sets the min/max
	// values property for discovered axes.
	if (FAILED(hr = g_pJoystick->EnumObjects(EnumObjectsCallback,
		(VOID*)hDlg, DIDFT_ALL)))
		return hr;

	return S_OK;
}


//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it痴 an XInput device
// Unfortunately this information can not be found by just using DirectInput.
// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
// XInput devices.
//
// This function stores the list of xinput devices in a linked list 
// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
//-----------------------------------------------------------------------------
HRESULT SetupForIsXInputDevice()
{
	IWbemServices* pIWbemServices = nullptr;
	IEnumWbemClassObject* pEnumDevices = nullptr;
	IWbemLocator* pIWbemLocator = nullptr;
	IWbemClassObject* pDevices[20] = { 0 };
	BSTR bstrDeviceID = nullptr;
	BSTR bstrClassName = nullptr;
	BSTR bstrNamespace = nullptr;
	DWORD uReturned = 0;
	bool bCleanupCOM = false;
	UINT iDevice = 0;
	VARIANT var;
	HRESULT hr;

	// CoInit if needed
	hr = CoInitialize(nullptr);
	bCleanupCOM = SUCCEEDED(hr);

	// Create WMI
	hr = CoCreateInstance(__uuidof(WbemLocator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWbemLocator),
		(LPVOID*)&pIWbemLocator);
	if (FAILED(hr) || pIWbemLocator == nullptr)
		goto LCleanup;

	// Create BSTRs for WMI
	bstrNamespace = SysAllocString(L"\\\\.\\root\\cimv2"); if (bstrNamespace == nullptr) goto LCleanup;
	bstrDeviceID = SysAllocString(L"DeviceID");           if (bstrDeviceID == nullptr)  goto LCleanup;
	bstrClassName = SysAllocString(L"Win32_PNPEntity");    if (bstrClassName == nullptr) goto LCleanup;

	// Connect to WMI 
	hr = pIWbemLocator->ConnectServer(bstrNamespace, nullptr, nullptr, 0L,
		0L, nullptr, nullptr, &pIWbemServices);
	if (FAILED(hr) || pIWbemServices == nullptr)
		goto LCleanup;

	// Switch security level to IMPERSONATE
	(void)CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, 0);

	// Get list of Win32_PNPEntity devices
	hr = pIWbemServices->CreateInstanceEnum(bstrClassName, 0, nullptr, &pEnumDevices);
	if (FAILED(hr) || pEnumDevices == nullptr)
		goto LCleanup;

	// Loop over all devices
	for (; ; )
	{
		// Get 20 at a time
		hr = pEnumDevices->Next(10000, 20, pDevices, &uReturned);
		if (FAILED(hr))
			goto LCleanup;
		if (uReturned == 0)
			break;

		for (iDevice = 0; iDevice < uReturned; iDevice++)
		{
			if (!pDevices[iDevice])
				continue;

			// For each device, get its device ID
			hr = pDevices[iDevice]->Get(bstrDeviceID, 0L, &var, nullptr, nullptr);
			if (SUCCEEDED(hr) && var.vt == VT_BSTR && var.bstrVal != nullptr)
			{
				// Check if the device ID contains "IG_".  If it does, then it痴 an XInput device
				// Unfortunately this information can not be found by just using DirectInput 
				if (wcsstr(var.bstrVal, L"IG_"))
				{
					// If it does, then get the VID/PID from var.bstrVal
					DWORD dwPid = 0, dwVid = 0;
					WCHAR* strVid = wcsstr(var.bstrVal, L"VID_");
					if (strVid && swscanf_s(strVid, L"VID_%4X", &dwVid) != 1)
						dwVid = 0;
					WCHAR* strPid = wcsstr(var.bstrVal, L"PID_");
					if (strPid && swscanf_s(strPid, L"PID_%4X", &dwPid) != 1)
						dwPid = 0;

					DWORD dwVidPid = MAKELONG(dwVid, dwPid);

					// Add the VID/PID to a linked list
					XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
					if (pNewNode)
					{
						pNewNode->dwVidPid = dwVidPid;
						pNewNode->pNext = g_pXInputDeviceList;
						g_pXInputDeviceList = pNewNode;
					}
				}
			}
			SAFE_RELEASE(pDevices[iDevice]);
		}
	}

LCleanup:
	if (bstrNamespace)
		SysFreeString(bstrNamespace);
	if (bstrDeviceID)
		SysFreeString(bstrDeviceID);
	if (bstrClassName)
		SysFreeString(bstrClassName);
	for (iDevice = 0; iDevice < 20; iDevice++)
		SAFE_RELEASE(pDevices[iDevice]);
	SAFE_RELEASE(pEnumDevices);
	SAFE_RELEASE(pIWbemLocator);
	SAFE_RELEASE(pIWbemServices);

	return hr;
}


//-----------------------------------------------------------------------------
// Returns true if the DirectInput device is also an XInput device.
// Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
//-----------------------------------------------------------------------------
bool IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
	// Check each xinput device to see if this device's vid/pid matches
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while (pNode)
	{
		if (pNode->dwVidPid == pGuidProductFromDirectInput->Data1)
			return true;
		pNode = pNode->pNext;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Cleanup needed for IsXInputDevice()
//-----------------------------------------------------------------------------
void CleanupForIsXInputDevice()
{
	// Cleanup linked list
	XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
	while (pNode)
	{
		XINPUT_DEVICE_NODE* pDelete = pNode;
		pNode = pNode->pNext;
		SAFE_DELETE(pDelete);
	}
}



//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,
	VOID* pContext)
{
	auto pEnumContext = reinterpret_cast<DI_ENUM_CONTEXT*>(pContext);
	HRESULT hr;

	if (g_bFilterOutXinputDevices && IsXInputDevice(&pdidInstance->guidProduct))
		return DIENUM_CONTINUE;

	// Skip anything other than the perferred joystick device as defined by the control panel.  
	// Instead you could store all the enumerated joysticks and let the user pick.
	if (pEnumContext->bPreferredJoyCfgValid &&
		!IsEqualGUID(pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance))
		return DIENUM_CONTINUE;

	// Obtain an interface to the enumerated joystick.
	hr = g_pDI->CreateDevice(pdidInstance->guidInstance, &g_pJoystick, nullptr);

	// If it failed, then we can't use this joystick. (Maybe the user unplugged
	// it while we were in the middle of enumerating it.)
	if (FAILED(hr))
		return DIENUM_CONTINUE;

	// Stop enumeration. Note: we're just taking the first joystick we get. You
	// could store all the enumerated joysticks and let the user pick.
	return DIENUM_STOP;
}




//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,
	VOID* pContext)
{
	HWND hDlg = (HWND)pContext;

	static int nSliderCount = 0;  // Number of returned slider controls
	static int nPOVCount = 0;     // Number of returned POV controls

								  // For axes that are returned, set the DIPROP_RANGE property for the
								  // enumerated axis in order to scale min/max values.
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -1000;
		diprg.lMax = +1000;

		// Set the range for the axis
		if (FAILED(g_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
			return DIENUM_STOP;

	}


	// Set the UI to reflect what objects the joystick supports
	if (pdidoi->guidType == GUID_XAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_X_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_X_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_YAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Y_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Y_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_ZAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Z_AXIS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Z_AXIS_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RxAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_X_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_X_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RyAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Y_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Y_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_RzAxis)
	{
		EnableWindow(GetDlgItem(hDlg, IDC_Z_ROT), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_Z_ROT_TEXT), TRUE);
	}
	if (pdidoi->guidType == GUID_Slider)
	{
		switch (nSliderCount++)
		{
		case 0:
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER0), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER0_TEXT), TRUE);
			break;

		case 1:
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER1), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_SLIDER1_TEXT), TRUE);
			break;
		}
	}
	if (pdidoi->guidType == GUID_POV)
	{
		switch (nPOVCount++)
		{
		case 0:
			EnableWindow(GetDlgItem(hDlg, IDC_POV0), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV0_TEXT), TRUE);
			break;

		case 1:
			EnableWindow(GetDlgItem(hDlg, IDC_POV1), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV1_TEXT), TRUE);
			break;

		case 2:
			EnableWindow(GetDlgItem(hDlg, IDC_POV2), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV2_TEXT), TRUE);
			break;

		case 3:
			EnableWindow(GetDlgItem(hDlg, IDC_POV3), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_POV3_TEXT), TRUE);
			break;
		}
	}

	return DIENUM_CONTINUE;
}




//-----------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
HRESULT UpdateInputState(HWND hDlg)
{
	HRESULT hr;
	TCHAR strText[512] = { 0 }; // Device state text
	DIJOYSTATE2 js;           // DInput joystick state 

	if (!g_pJoystick)
		return S_OK;

	// Poll the device to read the current state
	hr = g_pJoystick->Poll();
	if (FAILED(hr))
	{
		// DInput is telling us that the input stream has been
		// interrupted. We aren't tracking any state between polls, so
		// we don't have any special reset that needs to be done. We
		// just re-acquire and try again.
		hr = g_pJoystick->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = g_pJoystick->Acquire();

		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of 
		// switching, so just try again later 
		return S_OK;
	}

	// Get the input's device state
	if (FAILED(hr = g_pJoystick->GetDeviceState(sizeof(DIJOYSTATE2), &js)))
		return hr; // The device should have been acquired during the Poll()

	g_pSelector->setJoystic(&js);
				   // Display joystick state to dialog

				   // Axes
	_stprintf_s(strText, 512, TEXT("%ld"), js.lX);
	SetWindowText(GetDlgItem(hDlg, IDC_X_AXIS), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.lY);
	SetWindowText(GetDlgItem(hDlg, IDC_Y_AXIS), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.lZ);
	SetWindowText(GetDlgItem(hDlg, IDC_Z_AXIS), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.lRx);
	SetWindowText(GetDlgItem(hDlg, IDC_X_ROT), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.lRy);
	SetWindowText(GetDlgItem(hDlg, IDC_Y_ROT), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.lRz);
	SetWindowText(GetDlgItem(hDlg, IDC_Z_ROT), strText);

	// Slider controls
	_stprintf_s(strText, 512, TEXT("%ld"), js.rglSlider[0]);
	SetWindowText(GetDlgItem(hDlg, IDC_SLIDER0), strText);
	_stprintf_s(strText, 512, TEXT("%ld"), js.rglSlider[1]);
	SetWindowText(GetDlgItem(hDlg, IDC_SLIDER1), strText);

	// Points of view
	_stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[0]);
	SetWindowText(GetDlgItem(hDlg, IDC_POV0), strText);
	_stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[1]);
	SetWindowText(GetDlgItem(hDlg, IDC_POV1), strText);
	_stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[2]);
	SetWindowText(GetDlgItem(hDlg, IDC_POV2), strText);
	_stprintf_s(strText, 512, TEXT("%lu"), js.rgdwPOV[3]);
	SetWindowText(GetDlgItem(hDlg, IDC_POV3), strText);


	// Fill up text with which buttons are pressed
	_tcscpy_s(strText, 512, TEXT(""));
	for (int i = 0; i < 128; i++)
	{
		if (js.rgbButtons[i] & 0x80)
		{
			TCHAR sz[128];
			_stprintf_s(sz, 128, TEXT("%02d "), i);
			_tcscat_s(strText, 512, sz);
		}
	}

	SetWindowText(GetDlgItem(hDlg, IDC_BUTTONS), strText);

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID FreeDirectInput()
{
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if (g_pJoystick)
		g_pJoystick->Unacquire();

	// Release any DirectInput objects.
	SAFE_RELEASE(g_pJoystick);
	SAFE_RELEASE(g_pDI);
}


//-> 追加分^