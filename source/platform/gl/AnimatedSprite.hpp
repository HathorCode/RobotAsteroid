/*
	A set of rectangular textures,
	renderable when combined with an SRT.
*/
#pragma once
#include "Texture.hpp"
#include "Renderer.hpp"

#include <glm/mat3x3.hpp>

namespace tc {

	struct AnimatedSprite {
		GLvoid* bytesBufferOffset;   //Offset into buffer of vertices

		Texture textureArray;
		uint32 countTextures;

		//Animation information
		float32 curFrame;
		float32 fpt;  //Frames per tick

		//Does not initialize textures
		void Init(const uint32 initCountTextures,
		          const float32 initFPT,
				  const float32 pxpm,   //Number of pixels in a meter for this texture
		          const uint32 pxTextureLenX,
		          const uint32 pxTextureLenY,
		          GLvoid* const initBytesBufferOffset) {
			countTextures = initCountTextures;
			curFrame = 0;
			fpt = initFPT;
			bytesBufferOffset = initBytesBufferOffset;
			float32 mWrldXLen = ((float32)pxTextureLenX)/pxpm;
			float32	mWrldYLen = ((float32)pxTextureLenY)/pxpm;
			renderer.vertexBuffer.AddSpriteVertices((GLuint)bytesBufferOffset,
			                                        mWrldXLen, mWrldYLen);
		}

		//Called once per tick unless animation is paused
		void Update() {
			curFrame += fpt;
		}

		void BindActiveTexture() {
			return ;
		}
	};

}
