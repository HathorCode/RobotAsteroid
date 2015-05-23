#pragma once
#include "Player.hpp"
#include "Entity.hpp"

#include <engine/renderer/Drawable.hpp>

namespace tc {
	struct Planet : public EntityNode {
		Player::Condiment condiment;
		void Update() override {
			return;
		}
	};
}
