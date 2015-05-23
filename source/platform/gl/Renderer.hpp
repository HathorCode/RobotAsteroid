/*
	OpenGL renderer object to handle global state.
*/
#pragma once
#include "VertexBuffer.hpp"
#include "GLProcPtrs.hpp"

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>

namespace tc {
	struct RendererType {
		VertexBuffer vertexBuffer;

		void Init() {
			//Set default state
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				//Clear to black
			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Only consider source alpha level
			glBlendEquation(GL_FUNC_ADD);						//final color = dest * 1 - a + src * a
			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);									//Enable alpha blending for transperency

			//Generate a buffer to store square coordinates for rendering sprites
			vertexBuffer.Init();
			vertexBuffer.Bind();
			vertexBuffer.SetToDefault(bytesSpriteVerticesSize*5);
		}
	} renderer;
}
