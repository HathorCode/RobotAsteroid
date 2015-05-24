/*
	Provides structures for tracking what controls are activated.
*/
#pragma once

namespace robitRabit {
	struct {
		bool lmouse;
		bool rmouse;
		bool r;
		uint32 pxMousePosX,
			   pxMousePosY;
	} controls;
}
