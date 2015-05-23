/*
	Defines the windows entrypoint
*/
#include <Global.hpp>

#include "Menu.hpp"
#include "FileIO.hpp"
#include "Platform.hpp"
#include "Log.hpp"
#include "Options.hpp"
#include "GLHeader.hpp"
#include "Win.hpp"

#include <engine/renderer/Renderer.hpp>
#include <engine/renderer/Camera.hpp>

#include <game/Balance.hpp>
#include <game/Game.hpp>

#include <engine/renderer/Font.hpp>

#include <Windows.h>

namespace robitRabit {
	static float32 cyclesPerMS; //Game clock frequency

	//State functions
	void GameState() {
		//Initialize game state
		game.Init();
		LARGE_INTEGER curTimeCycles, oldTimeCycles;
		int64 cyclesElapsed;
		float32 delta = 0.0f;
		QueryPerformanceCounter(&oldTimeCycles);
		while (true) {
			QueryPerformanceCounter(&curTimeCycles);
			cyclesElapsed = curTimeCycles.QuadPart - oldTimeCycles.QuadPart;
			delta += cyclesElapsed / cyclesPerMS;
			oldTimeCycles = curTimeCycles;

			win.HandleInput();
			if (delta > 30.0f) {
				delta = mspt;
			}

			while (delta >= mspt) {
				game.Update();
				delta -= mspt;
			}
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game.Render(delta);
		}
	}
}

//Windows entrypoint
int __stdcall WinMain(HINSTANCE paramHInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	using namespace robitRabit; 
	robitRabit::log.Init();

	//Unfailable initialization
	hInstance = paramHInstance;
	Font::InitFreeType();
	Options::Init();
	LARGE_INTEGER cyclesPerSecondInt;
	QueryPerformanceFrequency(&cyclesPerSecondInt);
	cyclesPerMS = (float32)cyclesPerSecondInt.QuadPart; //Cycles per second
	cyclesPerMS /= 1000.0f;                             //Cycles per ms (cycles/s * s/ms)
	win.RegisterWndClass();
	menu.RegisterWndClass();

	//Try to start a window
	if (!win.Init()) {
		options = safeModeOptions;
		if (!win.Init()) {
			robitRabit::log.Write("Failed to create a basic window in safe mode.\n"
			              		  "Try updating your graphics drivers.");
			ExitProgram();
		}
	}
	gl.Init();                                           
	LoadFiles();
	GameState();
	//Never reached
}
