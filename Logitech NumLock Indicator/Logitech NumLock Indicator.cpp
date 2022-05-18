// Logitech NumLock Indicator.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "LogitechLEDLib.h"
#include "Logitech NumLock Indicator.h"
#include <iostream>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/// <summary>
/// Checks the toggle state of numlock and updates the rgb light accordingly.
/// This can basically be stuck in a while loop in a console app and accomplish the same thing as this entire program.
/// All this other stuff is to make it an interrupt based background process so a dumb numlock light isn't eating away at cpu cycles.
/// </summary>
void SetNumLockLED() {
    if (GetKeyState(VK_NUMLOCK) & 0x1)
        LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::NUM_LOCK, 0, 100, 100); //the last 3 numbers are r, g, b so choose your color there.
    else
        LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::NUM_LOCK, 0, 0, 0);
}

/// <summary>
/// The callback function for the global keyboard hook. Basically checks if the msg
/// is saying numlock was pressed. If it was, create a thread to check toggle state.
/// </summary>
LRESULT CALLBACK ProcessKeyboardMsg(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* msgInfo = (KBDLLHOOKSTRUCT*) lParam;
    if (msgInfo->vkCode == VK_NUMLOCK && wParam == WM_KEYDOWN) //if it was numlock that was pressed and only spawn a thread if it's on keydown. This way it only spawns a thread once per press.
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetNumLockLED, NULL, NULL, NULL);
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    bool LedInitialized = LogiLedInit(); //initialize Logitech's led library

    if (!LedInitialized)
    {
        MessageBoxA(NULL, "LogiLedInit() failed.", "Fatal error", MB_OK);
        LogiLedShutdown();
        return 0;
    }

    LogiLedSetTargetDevice(LOGI_DEVICETYPE_ALL);
    LogiLedSetLighting(100, 0, 0); //Set the keyboard to red (r, g, b)
    SetNumLockLED(); //check toggle state and set led on app startup.


    SetWindowsHookEx(WH_KEYBOARD_LL, ProcessKeyboardMsg, GetModuleHandle(NULL), 0); //apply global hook for keyboard
    MSG msg;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LOGITECHNUMLOCKINDICATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOGITECHNUMLOCKINDICATOR));

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) //keeps callback in place
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//Aside from commenting out showWindow, below is all windows app template stuff.

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGITECHNUMLOCKINDICATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LOGITECHNUMLOCKINDICATOR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow); //Commenting out ShowWindow basically makes it a background process (and is seen as such in task manager)
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
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