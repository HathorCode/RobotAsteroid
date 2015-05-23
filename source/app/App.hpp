/*
	Defines the game-state update procedures.
*/
#pragma once
#include "AssetManager.hpp"
#include "Balance.hpp"

#include <engine/renderer/Camera.hpp>

#include <platform/Win.hpp>

namespace robitRabit {
	struct {
		Camera camera;
		
		void Init() {
			camera.mScale = 1.0f;  //Screen width is 10 meters
			camera.Init(glm::mat3(1.0f/camera.mScale, 0.0f, 0.0f,
			                      0.0f, 1.0f/camera.mScale * win.aspectRatio, 0.0f,
			                      0.0f, 0.0f, 1.0f));
			assets.sidebar.transform.matrix = glm::mat3(1.0f/15.0f, 0.0, 0.0f,
														0.0f, 1.0f, 0.0f,
													   (1.0f/15.0f)-1.0f, 0.0f, 1.0f);
		}
		void Update() {
			//Handle controls
		}
		void Render(const float32 delta) {
			camera.Draw(Drawable::FromSprite(assets.background));
			camera.Draw(assets.sidebar);
		}
	} app;
}
