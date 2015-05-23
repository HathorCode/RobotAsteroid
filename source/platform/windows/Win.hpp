/*
  Struct for window management.
*/
#pragma once
#include <app/Controls.hpp>

namespace robitRabit {
	struct Win {
		HWND handle;
		HDC deviceContext;
		WNDCLASSEX mainWndClass;
		uint32 pxWorkingWinSizeX;
		uint32 pxWorkingWinSizeY;
		float32 aspectRatio;

		//TODO: split this up into separate handlers for different game states
		static LRESULT __stdcall WndHandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
				case WM_KEYDOWN:{
					switch (wParam) {
					
					}
					break;
				}
				case WM_KEYUP:{
					switch (wParam) {
					
					}
					break;
				}
				case WM_CLOSE:{
					ExitProgram();
				}
				case WM_QUIT:{
					ExitProgram();
				}
				//Normally window resolution is capped to stop you from going offscreen, so increase the maximum resolution.
				case WM_GETMINMAXINFO:{
					DefWindowProc(hWnd, msg, wParam, lParam);
					MINMAXINFO *pmmi = (MINMAXINFO*)lParam;
					pmmi->ptMaxTrackSize.x *= 2;
					pmmi->ptMaxTrackSize.y *= 2;
					break;
				}
				default:{
					return DefWindowProc(hWnd, msg, wParam, lParam);
				}
			}
			return 0;
		}
		void RegisterWndClass() {
			mainWndClass.cbSize = sizeof(WNDCLASSEX);
			mainWndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
			mainWndClass.lpfnWndProc = &WndHandleInput;
			mainWndClass.cbClsExtra = 0;
			mainWndClass.cbWndExtra = 0;
			mainWndClass.hInstance = hInstance;
			mainWndClass.hIcon = nullptr;
			mainWndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
			mainWndClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
			mainWndClass.lpszMenuName = nullptr;
			mainWndClass.lpszClassName = "RobitClass";
			mainWndClass.hIconSm = nullptr;
			ATOM succeeded0 = RegisterClassEx(&mainWndClass);
			assert(succeeded0 != 0);
		}
		bool Init() {
			handle = CreateWindowEx(0,
			                        mainWndClass.lpszClassName,
			                        "RobitRabit",
									WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
									0,
									0,
			                        1600,   //1650
			                        900,    //990
			                        nullptr,
			                        nullptr,
			                        mainWndClass.hInstance,
			                        nullptr);
			RECT dim;
			GetClientRect(handle, &dim);
			pxWorkingWinSizeX = dim.right - dim.left;
			pxWorkingWinSizeY = dim.bottom - dim.top;
			pxWorkingWinSizeX;
			pxWorkingWinSizeY;
			aspectRatio = ((float32)pxWorkingWinSizeX)/pxWorkingWinSizeY;
			if (handle != nullptr) {
				deviceContext = GetDC(handle);
				assert(deviceContext != nullptr);
				ShowWindow(handle, SW_SHOWDEFAULT);
				UpdateWindow(handle);
				return true;
			} else {
				return false;
			}
		}
		void Cleanup() {
			BOOL worked = DestroyWindow(handle);
			assert(worked);
			if (debug) {
				deviceContext = nullptr;
			}
		}
		//Common Function for all game states
		void HandleInput() {
			static MSG wndMsg;
			if (PeekMessage(&wndMsg, handle, 0, 0, true)) {
				TranslateMessage(&wndMsg);
				DispatchMessage(&wndMsg);
			}
			SwapBuffers(deviceContext);
		}
	} win;

	//Circumvent circular dependencies
	HWND GetSharedWndHandle() {
		return win.handle;
	}
}
