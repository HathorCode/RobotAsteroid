#pragma once
#include "GLProcPtrs.hpp"

namespace tc {
	const uint32 bytesSpriteVerticesSize = sizeof(GLfloat) * 8;
	GLvoid* const bytesPlayerSpriteBufferOffset = (GLvoid*)(bytesSpriteVerticesSize * 2); //Location of toastcat in VBO
	//This is always at the top of some vertex buffer
	GLfloat defaultVertices[] = {
		//Texture coordinates
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,

		//Fullscreen quad coordinates
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, -1.0f
	};

	struct VertexBuffer {
		GLuint id;

		void Init() {
			glGenBuffers(1, &id);
			if (id != 0) {
				return;
			} else {
				log.Write("[OpenGL] Failed to create vertex buffer; probably out of GPU memory.");
				ExitProgram();
			}
		}
		void Cleanup() {
			glDeleteBuffers(1, &id);
		}
		void SetToDefault(uintptr extraReserve) {
			SetWithReserve(sizeof(defaultVertices) + extraReserve + bytesSpriteVerticesSize,
			               sizeof(defaultVertices),
			               defaultVertices);
		}
		void Set(const GLuint bytesSize, const GLfloat vertices[]) {
			assert(IsBound());
			glBufferData(GL_ARRAY_BUFFER, bytesSize, vertices, GL_STATIC_DRAW);
		}
		void SetWithReserve(const GLuint bytesSize, const GLuint bytesVerticesSize, const GLfloat vertices[]) {
			assert(IsBound());
			glBufferData(GL_ARRAY_BUFFER, bytesSize, nullptr, GL_DYNAMIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, bytesVerticesSize, vertices);
		}
		void AddSpriteVertices(const GLuint bytesOffset, const float32 mWrldXLen, const float32 mWrldYLen) {
			assert(IsBound());
			const GLfloat vertices[] = {
				//Quad coordinates
				-mWrldXLen, mWrldYLen,
				-mWrldXLen, -mWrldYLen,
				mWrldXLen, mWrldYLen,
			   	mWrldXLen, -mWrldYLen,
			};
			glBufferSubData(GL_ARRAY_BUFFER, bytesOffset, sizeof(vertices), vertices);
		}

		void Bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, id);
		}
		bool IsBound() const {
			GLuint bound;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&bound);
			return bound == id;
		}
	};
}
