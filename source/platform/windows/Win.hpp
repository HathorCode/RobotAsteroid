/*
  Struct for window management.
*/
#pragma once
#include "Menu.hpp"

#include <game/Controls.hpp>

namespace tc {
	struct Win {
		HWND handle;
		HDC deviceContext;
		WNDCLASSEX mainWndClass;

		//TODO: split this up into separate handlers for different game states
		static LRESULT __stdcall WndHandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
				case WM_KEYDOWN:{
					switch (wParam) {
					case 'W':
						activeControls.up = true;
						break;
					case 'S':
						activeControls.down = true;
						break;
					case 'A':
						activeControls.left = true;
						break;
					case 'D':
						activeControls.right = true;
						break;
					}
					break;
				}
				case WM_KEYUP:{
					switch (wParam) {
					case 'W':
						activeControls.up = false;
						break;
					case 'S':
						activeControls.down = false;
						break;
					case 'A':
						activeControls.left = false;
						break;
					case 'D':
						activeControls.right = false;
						break;
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
			mainWndClass.lpszClassName = "ToastCatClass";
			mainWndClass.hIconSm = nullptr;
			ATOM succeeded0 = RegisterClassEx(&mainWndClass);
			assert(succeeded0 != 0);
		}
		bool Init() {
			//Calculate window coordinates
			//TODO: Implement true fullscreen
			RECT wndRect;
			if (options.fullscreenMode == Options::FullscreenMode::fullscreen
			    || options.fullscreenMode == Options::FullscreenMode::windowedFullscreen) {
				wndRect.left = 0;
				wndRect.right = GetSystemMetrics(SM_CXSCREEN);
				wndRect.top = 0;
				wndRect.bottom = GetSystemMetrics(SM_CYSCREEN);
				AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);
			} else {
				wndRect.left = (GetSystemMetrics(SM_CXSCREEN) - options.pxResX) / 2;    //Left should be adjusted by half of difference between fullscreen res and options res
				wndRect.right = GetSystemMetrics(SM_CXSCREEN) - wndRect.left;           //Subtract the previously calculated gap on screen since this is the right coordinate not the width
				wndRect.top = (GetSystemMetrics(SM_CYSCREEN) - options.pxResY) / 2;
				wndRect.bottom = GetSystemMetrics(SM_CYSCREEN) - wndRect.top;
				AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);
			}
			handle = CreateWindowEx(0,
			                        mainWndClass.lpszClassName,
			                        "ToastCat",
			                        WS_OVERLAPPEDWINDOW,
			                        wndRect.left,
			                        wndRect.top,
			                        wndRect.right - wndRect.left,
			                        wndRect.bottom - wndRect.top,
			                        nullptr,
			                        nullptr,
			                        mainWndClass.hInstance,
			                        nullptr);
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
			menu.HandleAnyButtonMsgs();
			SwapBuffers(deviceContext);
		}
	} win;

	//Circumvent circular dependencies
	HWND GetSharedWndHandle() {
		return win.handle;
	}
}
