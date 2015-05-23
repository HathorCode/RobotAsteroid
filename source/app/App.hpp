/*
	Defines the game-state update procedures.
*/
#pragma once
#include "Player.hpp"
#include "Balance.hpp"
#include "Planet.hpp"
#include "Trailer.hpp"

#include <engine/renderer/Camera.hpp>
#include <engine/renderer/Overlay.hpp>

#include <platform/Options.hpp>
#include <platform/Menu.hpp>

namespace tc {
	struct {
		Player player;
		Camera camera;
		Overlay hud;
		Menu testMenu;
		
		void Init() {
			camera.mScale = 10.0f;  //Screen width is 10 meters
			camera.Init(glm::mat3(1.0f/camera.mScale, 0.0f, 0.0f,
			                      0.0f, 1.0f/camera.mScale * options.aspectRatio, 0.0f,
			                      0.0f, 0.0f, 1.0f));
			trailer.Init();
			//testMenu.SetMenu
		}
		void Update() {
			//Handle controls
			SRT& playerTransform = player.drawable.transform;
			if (activeControls.up) {
				playerTransform.Translate(0, mptUpSpeed);
			}
			if (activeControls.down) {
				playerTransform.Translate(0, -mptDownSpeed);
			}
			if (activeControls.left) {
				playerTransform.Translate(-mptLeftSpeed, 0);
			}
			if (activeControls.right) {
				playerTransform.Translate(mptRightSpeed, 0);
			}
			camera.KeepUp(player.drawable.transform.GetTranslation());
		}
		void Render(float32 delta) {
			camera.Draw(trailer.world);			
			camera.Draw(player.drawable);
		}
	} game;
}
