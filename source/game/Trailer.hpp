/*
	Code for upcoming trailer level.
*/
#pragma once
#include "Planet.hpp"
#include "World.hpp"

#include <engine/renderer/AnimatedSprite.hpp>

namespace tc {
	struct Trailer {
		World world;
		static const uint32 countPlanets = 3;
		Planet planets[3];
		AnimatedSprite animatedSprite;
		
		Sprite earthPlanetSprite;
		Sprite jamPlanetSprite;

		void Init() {
			planets[0].drawable.sprite = earthPlanetSprite;
			planets[1].drawable.sprite = jamPlanetSprite;
			planets[2].drawable.sprite = jamPlanetSprite;
			planets[0].drawable.transform.Translate(0.0f, -6.0f);
			planets[1].drawable.transform.Translate(7.0f, +4.0f);
			planets[2].drawable.transform.Translate(7.0f, -4.0f);
			for(uint32 pIndex = 0; pIndex < 3; ++pIndex) {
				world.AddEntity(planets + pIndex);
			}
		}
	} trailer;
}
