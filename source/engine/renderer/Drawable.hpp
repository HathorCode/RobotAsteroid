/*
	Defines a sprite with a transformation as the basic drawable entity.
*/
#pragma once
#include "Sprite.hpp"
#include "SRT.hpp"

namespace robitRabit {
	struct Drawable {
		Sprite sprite;
		SRT transform;

		//Get a drawable from a sprite by applying an identity transformation
		static Drawable FromSprite(Sprite convert) {
			Drawable converted;
			converted.transform.SetIdentity();
			converted.sprite = convert;
			return converted;
		}
	};
}
