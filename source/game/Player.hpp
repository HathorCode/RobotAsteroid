#pragma once
#include <engine/renderer/Drawable.hpp>

namespace tc {
	struct Player {
		enum Condiment {
			grape,
			orange
		} condiment;
		Drawable drawable;
		float32 accel = 0.0f;
		float32 vel = 0.0f;

		void Init() {

		}
	};
}
