/* Defines a scale-rotation-translation object to be applied
   to a 2d sprite.*/
#pragma once
#include <glm/mat3x3.hpp>

namespace robitRabit {
	struct SRT {
		/*TODO: implement as a smaller object
		  Which can be transformed into the larger matrix*/
		glm::mat3 matrix;

		void Translate(float32 x, float32 y) {
			matrix[2][0] += x;
			matrix[2][1] += y;
		}
		void Rotate(float32 theta) {
			//cos theta sin theta 0
			//-sin theta cos theta 0
			//0           0        1
			glm::mat3 rotMatrix(cos(theta), sin(theta), 0,
			                    -sin(theta), cos(theta), 0,
			                    0, 0, 1);
			matrix = rotMatrix * matrix;
		}

		void SetIdentity() {
			matrix = glm::mat3(1.0f, 0.0f, 0.0f,
			                   0.0f, 1.0f, 0.0f,
			                   0.0f, 0.0f, 1.0f);
		}

		SRT GetTransformed(SRT other) {
			return SRT(other.matrix * matrix);
		}
		glm::mat3 GetMatrix() const {
			return matrix;
		}
		glm::vec2 GetTranslation() const {
			return *(glm::vec2*)&matrix[2][0];
		}
		float32 GetRotation() const {
			//TODO:
		}

		SRT(glm::mat3 initMatrix) {
			matrix = initMatrix;
		}
		SRT() {}
	};
}
