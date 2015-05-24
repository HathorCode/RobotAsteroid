/*
  Struct for window management.
*/
#pragma once
#include <app/Controls.hpp>

#include <Windows.h>
#include <Windowsx.h>

namespace robitRabit {
	struct Win {
		HWND handle;
		HDC deviceContext;
		WNDCLASSEX mainWndClass;
		uint32 pxWorkingWinSizeX;
		uint32 pxWorkingWinSizeY;
		float32 aspectRatio;

		static LRESULT __stdcall WndHandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
				case WM_KEYDOWN:{
					switch (wParam) {
						case 'R':
							controls.r = true;
							break;
						case 'W':
							controls.w = true;
							break;
						case 'A':
							controls.a = true;
							break;
						case 'S':
							controls.s = true;
							break;
						case 'D':
							controls.d = true;
							break;
					}
					break;
				} case WM_KEYUP:{
					switch (wParam) {
						case 'R':
							controls.r = false;
							break;
						case 'W':
							controls.w = false;
							break;
						case 'A':
							controls.a = false;
							break;
						case 'S':
							controls.s = false;
							break;
						case 'D':
							controls.d = false;
							break;
					}
					break;
				} case WM_LBUTTONDOWN:{
					controls.lmouse = true;
					break;
				} case WM_RBUTTONDOWN:{
					controls.rmouse = true;			
					break;
				} case WM_LBUTTONUP:{
					controls.lmouse = false;				
					break;
				} case WM_RBUTTONUP:{
					controls.rmouse = false;				
					break;
				} case WM_MOUSEMOVE:{
					controls.pxMousePosX = GET_X_LPARAM(lParam);
					controls.pxMousePosY = GET_Y_LPARAM(lParam);
					break;
				} case WM_CLOSE:{
					ExitProgram();
				} case WM_QUIT:{
					ExitProgram();
				}
				//Normally window resolution is capped to stop you from going offscreen, so increase the maximum resolution.
				case WM_GETMINMAXINFO:{
					DefWindowProc(hWnd, msg, wParam, lParam);
					MINMAXINFO *pmmi = (MINMAXINFO*)lParam;
					pmmi->ptMaxTrackSize.x *= 2;
					pmmi->ptMaxTrackSize.y *= 2;
					break;
				} default:{
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
			RECT adjust;
			adjust.left = 0;
			adjust.right = 1600;
			adjust.top = 0;
			adjust.bottom = 900;
			BOOL worked = AdjustWindowRectEx(&adjust, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE, 0);
			assert(worked);
			handle = CreateWindowEx(0,
			                        mainWndClass.lpszClassName,
			                        "RobitRabit",
									WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
									0,
									0,
			                        adjust.right - adjust.left,   //1650
			                        adjust.bottom - adjust.top,    //990
			                        nullptr,
			                        nullptr,
			                        mainWndClass.hInstance,
			                        nullptr);
			RECT dim;
			GetClientRect(handle, &dim);
			pxWorkingWinSizeX = dim.right - dim.left;
			pxWorkingWinSizeY = dim.bottom - dim.top;
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
