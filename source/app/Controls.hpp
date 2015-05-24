/*
	Provides structures for tracking what controls are activated.
*/
#pragma once

namespace robitRabit {
	struct {
		bool lmouse;
		bool rmouse;
		bool r;

		bool w;
		bool a;
		bool s;
		bool d;
		uint32 pxMousePosX,
			   pxMousePosY;
	} controls;
}
