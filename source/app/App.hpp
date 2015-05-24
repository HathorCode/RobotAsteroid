/*
	Defines the game-state update procedures.
*/
#pragma once
#include "AssetManager.hpp"
#include "Balance.hpp"
#include "Obstacle.hpp"

#include <engine/renderer/Camera.hpp>

#include <platform/Win.hpp>

namespace robitRabit {
	struct {
		Camera camera;
		ObstacleInProgress oip;
		
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
			if (controls.lmouse) {
				if (oip.obstacleCreatePhase == ObstacleInProgress::notInCreation) {
					oip.Begin();
					oip.obstacleCreatePhase = ObstacleInProgress::begin;
				} else if (oip.obstacleCreatePhase == ObstacleInProgress::inProgress) {
					oip.End();
					oip.obstacleCreatePhase = ObstacleInProgress::end;
				}
			} else {
				if (oip.obstacleCreatePhase == ObstacleInProgress::begin) {
					oip.obstacleCreatePhase = ObstacleInProgress::inProgress;
				} else if (oip.obstacleCreatePhase == ObstacleInProgress::end) {
					oip.obstacleCreatePhase = ObstacleInProgress::notInCreation;
				}
			}
			
			//Update OIP
			if (oip.obstacleCreatePhase == ObstacleInProgress::begin
			    || oip.obstacleCreatePhase == ObstacleInProgress::inProgress) {
				oip.Update();
			}
		}
		void Render(const float32 delta) {
			camera.Draw(Drawable::FromSprite(assets.background));
			camera.Draw(assets.sidebar);
			if (oip.obstacleCreatePhase == ObstacleInProgress::begin
			    || oip.obstacleCreatePhase == ObstacleInProgress::inProgress) {
				camera.Draw(oip.actual.obsSprite);
			}
			//camera.Draw(Drawable::FromSprite(assets.carEnd)); //To be eventually used 
		}
	} app;
}
