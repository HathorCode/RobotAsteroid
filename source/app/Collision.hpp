/*
	Collision detection using AABBs
*/
#pragma once

namespace robitRabit {
	struct AABB {
		int32 pxTLX,   //Top left corner x coordinate
			  pxTLY,
			  pxBRX,   //Bottom right corner x coordinate
			  pxBRY;
		bool Collide(int32 pxPosX, int32 pxPosY) {
			if (pxPosX < pxTLX) {
				return false;
			}
			if (pxPosY < pxTLY) {
				return false;
			}
			if (pxPosX > pxBRX) {
				return false;
			}
			if (pxPosY > pxBRY) {
				return false;
			}
			return true;
		}
		
		bool Collide(AABB a) {
			if (pxTLX < a.pxBRX
			    && pxBRX > a.pxTLX
			    && pxTLY < a.pxBRY
			    && pxBRY > a.pxTLY) {
				return true;
			}
			return false;
		}
	};
}
