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

			float32 pxLenX = (float32)(pxBRX - pxTLX);
			float32 pxLenY = (float32)(pxBRY - pxTLY);
			float32 scrLenX = pxLenX / win.pxWorkingWinSizeX;
			float32 scrLenY = pxLenY / win.pxWorkingWinSizeY;
			obsSprite.transform.Scale(abs(scrLenX), abs(scrLenY) / win.aspectRatio);

			float32 pxSpriteCenterX = pxLenX / 2.0f + pxTLX;
			float32 pxSpriteCenterY = pxLenY / 2.0f + pxTLY;
			float32 scrSpriteCenterX = pxSpriteCenterX / win.pxWorkingWinSizeX;
			float32 scrSpriteCenterY = pxSpriteCenterY / win.pxWorkingWinSizeY;
			float32 scrTransX = scrSpriteCenterX - 0.5f;
			float32 scrTransY = scrSpriteCenterY;
			obsSprite.transform.Translate(scrTransX, 0.0f);
		}
	};
	std::vector<Obstacle> obstacles;

	//An Obstacle which is actively being created
	struct ObstacleInProgress {
		//This is so we know how to handle mouse up and mouse down
		//not in creation = mouse up, not creating.
		//begin = mouse down, creating
		//inProgress = mouse up, creating
		//end = mouse down, not creating.
		enum { notInCreation, begin, inProgress, end } obstacleCreatePhase = notInCreation;
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
