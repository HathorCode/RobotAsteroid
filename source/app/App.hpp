/*
	Defines the game-state update procedures.
*/
#pragma once
#include "Balance.hpp"

#include <engine/renderer/Camera.hpp>
#include <engine/renderer/Overlay.hpp>

#include <platform/Win.hpp>

namespace robitRabit {
	struct {
		Camera camera;
		Overlay hud;
		
		void Init() {
			camera.mScale = 10.0f;  //Screen width is 10 meters
			camera.Init(glm::mat3(1.0f/camera.mScale, 0.0f, 0.0f,
			                      0.0f, 1.0f/camera.mScale * win.aspectRatio, 0.0f,
			                      0.0f, 0.0f, 1.0f));
		}
		void Update() {
			//Handle controls
		}
		void Render(float32 delta) {
		
		}
	} app;
}
