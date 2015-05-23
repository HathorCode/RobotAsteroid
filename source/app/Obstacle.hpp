/*
	List of obstacles
*/
#pragma once
#include <Global.hpp>
#include "AssetManager.hpp"

#include <engine/renderer/Drawable.hpp>
#include <engine/renderer/Camera.hpp>

#include <platform/Win.hpp>

#include <vector>

namespace robitRabit {
	struct Obstacle {
		Drawable obsSprite;
		int32 pxTLX,   //Top left corner x coordinate
			  pxTLY,
			  pxBRX,   //Bottom right corner x coordinate
			  pxBRY;
		void Make(int32 initPXTLX, int32 initPXTLY, int32 initPXBRX, int32 initPXBRY) {
			pxTLX = initPXTLX;
			pxTLY = initPXTLY;
			pxBRX = initPXBRX;
			pxBRY = initPXBRY;
			obsSprite.sprite = assets.obs;
			obsSprite.transform.SetIdentity();
			
			int32 pxLenX = pxBRX - pxTLX;
			int32 pxLenY = pxBRY - pxTLY;
			float32 scrLenX = ((float32)pxLenX) / win.pxWorkingWinSizeX;
			float32 scrLenY = ((float32)pxLenY) / win.pxWorkingWinSizeY;
			int32 pxTransX = pxTLX + pxLenX/2;
			int32 pxTransY = pxTLY + pxLenY/2;
			float32 scrTransX = ((float32)pxTransX) / win.pxWorkingWinSizeX;
			float32 scrTransY = ((float32)pxTransY) / win.pxWorkingWinSizeY;

			obsSprite.transform.Translate(scrTransX, scrTransY);
			obsSprite.transform.Scale(scrLenX, scrLenY);
		}
	};
	std::vector<Obstacle> obstacles;

	//An Obstacle which is actively being created
	struct ObstacleInProgress {
		bool active;    //Are we creating an obstacle right now?
		int32 pxAnchor0X,
		      pxAnchor0Y,
    		  pxAnchor1X,
	    	  pxAnchor1Y;
		Obstacle actual;

		void Begin() {
			assert(!active);
			pxAnchor0X = controls.pxMousePosX;
			pxAnchor0Y = controls.pxMousePosY;
			active = true;
		}

		void Update() {
			pxAnchor1X = controls.pxMousePosX;
			pxAnchor1Y = controls.pxMousePosY;
			if (pxAnchor1X == pxAnchor0X) {
				++pxAnchor1X;
			}
			if (pxAnchor1Y == pxAnchor0Y) {
				++pxAnchor1Y;
			}
			int32 pxTLX,   //Top left corner x coordinate
			      pxTLY,
			      pxBRX,   //Bottom right corner x coordinate
			      pxBRY;
			GetCorners(pxTLX, pxTLY, pxBRX, pxBRY);
			actual.Make(pxTLX, pxTLY, pxBRX, pxBRY);
		}

		//The anchors can be any two opposite corners, we want particular ones
		void GetCorners(int32& pxTLXOut, int32& pxTLYOut, int32& pxBRXOut, int32& pxBRYOut) {
			if (pxAnchor0X > pxAnchor1X) {
				pxTLXOut = pxAnchor0X;
				pxBRXOut = pxAnchor1X;
			} else {
				pxBRXOut = pxAnchor1X;
				pxTLXOut = pxAnchor0X;
			}
			if (pxAnchor0Y > pxAnchor1Y) {
				pxTLYOut = pxAnchor0Y;
				pxBRYOut = pxAnchor1Y;
			} else {
				pxBRYOut = pxAnchor1Y;
				pxTLYOut = pxAnchor0Y;
			}
		}

		void End() {
			active = false;
			obstacles.push_back(actual);
		}
	};
}
