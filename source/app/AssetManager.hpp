/*
	Manages app asset loading and streaming.
*/

#pragma once
#include <engine/renderer/Sprite.hpp>
#include <engine/renderer/Drawable.hpp>

namespace robitRabit {
	struct {
		Sprite background;
		Sprite obs;
		Drawable sidebar;
	} assets;
}
