/*
	A rectangular texture, renderable when combined with an SRT.
*/
#pragma once
#include "Texture.hpp"
#include "Renderer.hpp"

#include <glm/mat3x3.hpp>

namespace robitRabit {

	struct Sprite {
		GLvoid* bytesBufferOffset;   //Offset into buffer of vertices
		Texture texture;

		//Does not initialize texture
		void Init(const float32 pxpm,   //Number of pixels in a meter for this texture
		          const uint32 pxTextureLenX,
		          const uint32 pxTextureLenY,
		          GLvoid* const initBytesBufferOffset) {
			bytesBufferOffset = initBytesBufferOffset;
			float32 mWrldXLen = ((float32)pxTextureLenX)/pxpm;
			float32	mWrldYLen = ((float32)pxTextureLenY)/pxpm;
			renderer.vertexBuffer.AddSpriteVertices((GLuint)bytesBufferOffset,
			                                        mWrldXLen, mWrldYLen);
		}
		void InitResIndependent(GLvoid* const initBytesBufferOffset) {
			bytesBufferOffset = initBytesBufferOffset;
			renderer.vertexBuffer.AddSpriteVertices((GLuint)bytesBufferOffset,
												    1.0f, 1.0f/win.aspectRatio);
		}
	};

}
