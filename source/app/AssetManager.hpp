/*
	Manages app asset loading and streaming.
*/

#pragma once
#include <engine/renderer/Sprite.hpp>
#include <engine/renderer/Drawable.hpp>

namespace robitRabit {
	struct AssetManager {
		Sprite background;
		Sprite obs;
		Drawable sidebar;

		static const uint32 pxObsResX = 432;
		static const uint32 pxObsResY = 539;
	} assets;
}
