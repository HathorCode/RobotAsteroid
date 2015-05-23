/*
	Contains data and procedures for rendering menus.
*/
#pragma once
#include "tcVertexBuffer.hpp"
#include "tcSprite.hpp"
#include "tcTexture.hpp"

//TODO: fix allocations
namespace robitRabit {
	struct MenuRenderer {
		VertexBuffer buttonVertexBuffer;
		GLfloat* vertices;
		Texture const* textures;    //Background and button textures
		uint32 numButtons;

		void Init(uint32 initNumButtons,
		          GLfloat* verticesBuffer,
		          Sprite* spritesBuffer,
		          Texture const* initBackground) {
			//Check if fullscreen buffer is on top
			assert(0 == memcmp(verticesBuffer, fullscreenQuadVertices, bytesSpriteVerticesSize));

			this->numButtons = initNumButtons;
			this->vertices = verticesBuffer;
			this->sprites = spritesBuffer;
			this->background = initBackground;
		
			this->buttonVertexBuffer.Init();
			this->buttonVertexBuffer.Set(this->numButtons * bytesSpriteVerticesSize, vertices);
		}

		void Cleanup() {
			free(this->vertices);
			free(this->sprites);
			//We are not responsible for cleaning up background
		}

		void AddButton(GLfloat* newVertices[16], Sprite* newSprite) {
			this->vertices = (GLfloat*)realloc(this->vertices, ++this->numButtons * bytesSpriteVerticesSize);
			memcpy(&this->vertices[this->numButtons - 1], newVertices, bytesSpriteVerticesSize);
			this->sprites = (Sprite*)realloc(this->sprites, this->numButtons * sizeof(Sprite));
			memcpy(&this->sprites[this->numButtons - 1], newSprite, bytesSpriteVerticesSize);
		
			this->buttonVertexBuffer.Set(this->numButtons * bytesSpriteVerticesSize, vertices);
		}
	};
}
