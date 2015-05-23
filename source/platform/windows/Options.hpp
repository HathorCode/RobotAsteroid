#pragma once
#include "Platform.hpp"

namespace tc {
	struct Options {
		intptr bytesMaxMem; //The maximum amount of memory the game can allocate
		int32 pxResX;
		int32 pxResY;
		float32 aspectRatio;
		enum FullscreenMode {
			windowed,
			windowedFullscreen,	//Fake fullscreen by moving the window to the edges of the screen
			fullscreen,
		} fullscreenMode;
	
		void ReadFromFile() {
			//TODO: This
			options = defaultOptions;   //Fake it
		}
		void WriteToFile() {
			//TODO: This
		}

		void UpdateAspectRatio() {
			this->aspectRatio = ((float32)this->pxResX) / this->pxResY;
		}
			
		static void Init() {
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			bytesPageSize = si.dwPageSize;
			bytesAllocGran = si.dwAllocationGranularity;
				
			if (sizeof(void*) == 8) {
				defaultOptions.bytesMaxMem = 1073741824;
			} else {
				defaultOptions.bytesMaxMem = 573741824; //On 32 bit platforms, memory is more limited
			}
			defaultOptions.bytesMaxMem -= bytesAllocGran - defaultOptions.bytesMaxMem % bytesAllocGran;
			if (defaultOptions.bytesMaxMem < bytesAllocGran) {
				defaultOptions.bytesMaxMem = bytesAllocGran;
			}
			defaultOptions.pxResX = GetSystemMetrics(SM_CXSCREEN);
			defaultOptions.pxResY = GetSystemMetrics(SM_CYSCREEN);
			defaultOptions.UpdateAspectRatio();
			defaultOptions.fullscreenMode = Options::FullscreenMode::fullscreen;
			uintptr bytesMaxMem = 1073741824L;
			bytesMaxMem *= 2L;
			safeModeOptions.bytesMaxMem = bytesMaxMem;
			safeModeOptions.bytesMaxMem -= bytesAllocGran - safeModeOptions.bytesMaxMem % bytesAllocGran;
			if (safeModeOptions.bytesMaxMem < bytesAllocGran) {
				safeModeOptions.bytesMaxMem = bytesAllocGran;
			}
			safeModeOptions.pxResX = 1024;
			safeModeOptions.pxResY = 576;
			safeModeOptions.UpdateAspectRatio();
			safeModeOptions.fullscreenMode = Options::FullscreenMode::windowed;

			options.ReadFromFile();
		}
	} options, defaultOptions, safeModeOptions;
}
