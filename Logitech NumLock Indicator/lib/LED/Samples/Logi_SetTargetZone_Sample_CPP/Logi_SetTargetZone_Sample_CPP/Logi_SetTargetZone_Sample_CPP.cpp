// Logi_SetTargetZone_Sample_CPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LogitechLEDLib.h"
#include <iostream>
#include <Windows.h>

void SetNumLockLED() {

    if (GetKeyState(VK_NUMLOCK) & 0x1)
        LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::NUM_LOCK, 0, 100, 100);
    else
        LogiLedSetLightingForKeyWithKeyName(LogiLed::KeyName::NUM_LOCK, 0, 0, 0);

}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetNumLockLED, NULL, NULL, NULL);
    return 0;

}

int _tmain(int argc, _TCHAR* argv[])
{
    // Initialize the LED SDK
    FreeConsole();
    bool LedInitialized = LogiLedInit();

    if (!LedInitialized)
    {
        //std::cout << "LogiLedInit() failed." << std::endl;
        return 0;
    }
    //std::cout << "LED SDK Initialized" << std::endl;

    LogiLedSetLighting(100, 0, 0);
    SetNumLockLED();


    SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (!GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    LogiLedShutdown();
    
    return 0;
}
  