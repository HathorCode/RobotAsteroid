#pragma once
#include "Sprite.hpp"
#include "SRT.hpp"
#include "Drawable.hpp"

#include <glm/mat3x3.hpp>

namespace tc {
	struct Camera {
		glm::mat3 wrldViewMatrix;
		float32 mScale;  //Width of screen is this many meters
		
		void Init(const glm::mat3 initWrldViewMatrix) {
			wrldViewMatrix = initWrldViewMatrix;
		}

		//Keep up with the player
		void KeepUp(const glm::vec2 playerTranslation) {
			//Optimize: what is being scaled
			glm::vec2& cameraTranslation = *(glm::vec2*)&wrldViewMatrix[2][0];
			glm::vec2 distFromCenter = playerTranslation + (cameraTranslation * mScale);

			float32 screenBoundsX = mScale * 0.5f * cameraLag;	//TODO: stop recomputing this every call
			float32 screenBoundsY = mScale * 0.5f * cameraLag / options.aspectRatio;	//TODO: stop recomputing this every call

			if (distFromCenter.x > screenBoundsX) {
				cameraTranslation.x -= ((distFromCenter.x - screenBoundsX) / mScale);
			} else if (-distFromCenter.x > screenBoundsX) {
				cameraTranslation.x -= ((distFromCenter.x + screenBoundsX) / mScale);
			}

			if (distFromCenter.y > screenBoundsY) {
				cameraTranslation.y -= ((distFromCenter.y - screenBoundsY) / mScale);
			} else if (-distFromCenter.y > screenBoundsY) {
				cameraTranslation.y -= ((distFromCenter.y + screenBoundsY) / mScale);
			}
		}
		
		void Draw(const Drawable drawable) {
			glm::mat3 mdlViewMatrix = wrldViewMatrix * drawable.transform.GetMatrix();
			mainShader.Draw(drawable.sprite, &mdlViewMatrix);
		}
		void Draw(const World world) {
			Draw(Drawable::FromSprite(world.background));
			for (uint32 drawableIndex = 0; drawableIndex < world.countEntities; ++drawableIndex) {
				Draw(world.GetEntity(drawableIndex)->drawable);
			}
		}
	};
}
