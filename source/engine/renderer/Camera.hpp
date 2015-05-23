#pragma once
#include "Sprite.hpp"
#include "SRT.hpp"
#include "Drawable.hpp"
#include "Shader.hpp"

#include <glm/mat3x3.hpp>

namespace robitRabit {
	struct Camera {
		glm::mat3 wrldViewMatrix;
		float32 mScale;  //Width of screen is this many meters
		
		void Init(const glm::mat3 initWrldViewMatrix) {
			wrldViewMatrix = initWrldViewMatrix;
		}
		
		void Draw(const Drawable drawable) {
			glm::mat3 mdlViewMatrix = wrldViewMatrix * drawable.transform.GetMatrix();
			mainShader.Draw(drawable.sprite, &mdlViewMatrix);
		}
	};
}
