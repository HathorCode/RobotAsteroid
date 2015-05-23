/*
	Creates and manages game menus for windows
*/
#pragma once
#include "Options.hpp"
#include "Platform.hpp"
#include "Allocator.hpp"

#include <Windows.h>
#include <cstdio>

namespace tc {
	struct Menu {		
		struct Button {
			float32 xPos, yPos;     //Scale of 0-1, .5 being the center of the screen
			float32 width, height;  //Scale of 0-1, .5 being half the width/height of the screen
			WNDPROC callback;
		};

		WNDCLASSEX buttonsClass;
		HWND* hButtons = nullptr;       //A menu is just a series of buttons, given here as handles
		uint8* buttonPressed = nullptr; //Whether each button in the menu is pressed
		uint32 len = 0;

		bool32 SetMenu(uint32 numButtons, Button buttons[]) {
			assert(hButtons == nullptr && len == 0);
			len = numButtons;
			if(hButtons = (HWND*) allocator.Alloc(len * sizeof(HWND))) {
				for (uint32 i = 0; i < len; ++i) {
					if(!AddButton(i, buttons[i])) {
						return false;
					}
				}
			} else {
				ExitProgram();
			}
			return true;
		}
		//Note: for this to work, the menu memory must be on top of the allocator.
		void FreeMenu() {
			for (uint32 i = 0; i < len; ++i) {
				bool32 worked = DestroyWindow(menu.hButtons[i]);
				assert(worked);
			}
			allocator.Free((uint8*)hButtons);
			if (debug) {
				hButtons = nullptr;
				len = 0;
			}
		}
		void HandleAnyButtonMsgs() {
			static MSG wndMsg;
			for (uint32 i = 0; i < len; ++i) {
				if (PeekMessageA(&wndMsg, hButtons[i], 0, 0, true)) {
					TranslateMessage(&wndMsg);
					DispatchMessageA(&wndMsg);
				}
			}
		}
		void RegisterWndClass() {
			buttonsClass.cbSize = sizeof(WNDCLASSEX);
			buttonsClass.style = 0;
			buttonsClass.lpfnWndProc = nullptr; //Defined on a per-button basis
			buttonsClass.cbClsExtra = 0;
			buttonsClass.cbWndExtra = 0;
			buttonsClass.hInstance = hInstance;
			buttonsClass.hIcon = nullptr;
			buttonsClass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
			buttonsClass.hbrBackground = 0;
			buttonsClass.lpszMenuName = nullptr;
			buttonsClass.lpszClassName = "ToastCatButtonClass";
			buttonsClass.hIconSm = nullptr;
			ATOM succeeded1 = RegisterClassExA(&buttonsClass);
			assert(succeeded1 != 0);
		}

	private:
		bool32 AddButton(uint32 index, Button button) {
			//Circumvent circular dependencies
			HWND GetSharedWndHandle();	//Returns handle of main window

			HWND& hButton = hButtons[index];
			hButton = CreateWindowExA(0,
				buttonsClass.lpszClassName,
				"",
				WS_CHILD,
				(int32)(options.pxResX * button.xPos),
				(int32)(options.pxResY * button.yPos),
				(int32)(options.pxResX * button.width),
				(int32)(options.pxResY * button.height),
				GetSharedWndHandle(),
				nullptr,
				hInstance,
				nullptr);
			if (hButton != nullptr) {
				ShowWindow(hButton, SW_SHOWDEFAULT);
				UpdateWindow(hButton);
				SetWindowLongPtrA(hButton, GWLP_WNDPROC, (LONG_PTR)button.callback);
				return true;
			} else {
				return false;
			}
		}
	} menu;
}
