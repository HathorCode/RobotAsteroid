/*
	Defines the part of the operating system abstraction called by the game.
*/
#pragma once
#include "Log.hpp"
#include <platform/Enum.hpp>
#include <platform\windows\Serial.hpp>

#include <Windows.h>

namespace robitRabit {
	//Globals used by the other part of the windows platform abstraction
	HINSTANCE hInstance;
	intptr bytesPageSize;
	intptr bytesAllocGran;
	
	typedef uint32 fileId;
	typedef WNDPROC wndCallback;    //Used to make windows callback messages
	
#   define defWindowCallback(name) LRESULT __stdcall name(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	
	void ExitProgram() {
		robitRabit::serial.Close();
		robitRabit::log.Cleanup();
		ExitProcess(0);
	}
}
