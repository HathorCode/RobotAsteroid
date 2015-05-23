/*
	Defines a 2D overlay,
	a series of sprites to be rendered on top of the scene.
*/
#pragma once
#include <engine/renderer/Drawable.hpp>

namespace tc {
	struct Overlay {
		Drawable* elems;
		uint32 countElems;
		
		void Draw(const Drawable drawable) {
			for (uint32 elemIndex = 0; elemIndex < countElems; ++elemIndex) {
				Drawable& elem = elems[elemIndex];
				glm::mat3 mdlViewMatrix = elem.transform.GetMatrix();
				mainShader.Draw(drawable.sprite, &mdlViewMatrix);
			}
		}
	};
}
