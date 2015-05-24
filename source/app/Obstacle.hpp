/*
	List of obstacles
*/
#pragma once
#include "AssetManager.hpp"
#include "Collision.hpp"

#include <engine/renderer/Drawable.hpp>
#include <engine/renderer/Camera.hpp>

#include <platform/Win.hpp>

#include <vector>

namespace robitRabit {
	struct Obstacle {
		Drawable obsSprite;
		AABB aabb;
		void Make(int32 initPXTLX, int32 initPXTLY, int32 initPXBRX, int32 initPXBRY) {
			aabb.pxTLX = initPXTLX;
			aabb.pxTLY = initPXTLY;
			aabb.pxBRX = initPXBRX;
			aabb.pxBRY = initPXBRY;
			obsSprite.sprite = assets.obs;
			obsSprite.transform.SetIdentity();

			float32 pxLenX = (float32)(aabb.pxBRX - aabb.pxTLX);
			float32 pxLenY = (float32)(aabb.pxBRY - aabb.pxTLY);
			float32 scrLenX = pxLenX / win.pxWorkingWinSizeX;
			float32 scrLenY = pxLenY / win.pxWorkingWinSizeY;
			obsSprite.transform.Scale(abs(scrLenX), abs(scrLenY) / win.aspectRatio);

			float32 pxSpriteCenterX = pxLenX / 2.0f + aabb.pxTLX;
			float32 pxSpriteCenterY = pxLenY / 2.0f + aabb.pxTLY;
			float32 pxTransX = pxSpriteCenterX - ((float32)win.pxWorkingWinSizeX) / 2.0f;
			float32 pxTransY = ((float32)win.pxWorkingWinSizeY) / 2.0f - pxSpriteCenterY;
			float32 scrTransX = pxTransX / (win.pxWorkingWinSizeX / 2.0f);
			float32 scrTransY = pxTransY / (win.pxWorkingWinSizeY / 2.0f);
			obsSprite.transform.Translate(scrTransX, scrTransY / win.aspectRatio);
		}
	};

	std::vector<Obstacle> obstacles;
	void RemoveObstacle() {
		int32 pxPosX = (int32)controls.pxMousePosX;
		int32 pxPosY = (int32)controls.pxMousePosY;
		for (uint32 obsIndex = 0; obsIndex < obstacles.size(); ++obsIndex) {
			if (obstacles[obsIndex].aabb.Collide(pxPosX, pxPosY)) {
				obstacles.erase(obstacles.begin() + obsIndex);
				return;	//There can only be one obstacle at a given point
			}
		}
	}

	//An Obstacle which is actively being created
	struct ObstacleInProgress {
		//This is so we know how to handle mouse up and mouse down
		//not in creation = mouse up, not creating.
		//begin = mouse down, creating
		//inProgress = mouse up, creating
		//end = mouse down, not creating.
		enum { notInCreation, begin, inProgress, end } obstacleCreatePhase = notInCreation;
		int32 pxAnchor0X,
		      pxAnchor0Y,
    		  pxAnchor1X,
	    	  pxAnchor1Y;
		Obstacle actual;

		bool Begin() {
			pxAnchor0X = controls.pxMousePosX;
			pxAnchor0Y = controls.pxMousePosY;
			for (uint32 obsIndex = 0; obsIndex < obstacles.size(); ++obsIndex) {
				if (obstacles[obsIndex].aabb.Collide(pxAnchor0X, pxAnchor0Y)) {
					return false;
				}
			}
			return true;
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
			
			if (pxAnchor1X < (1.0 / 15.0f * win.pxWorkingWinSizeX)) {
				pxAnchor1X = (1.0 / 15.0f * win.pxWorkingWinSizeX) + 1;
			}
			int32 pxTLX,   //Top left corner x coordinate
				pxTLY,
				pxBRX,   //Bottom right corner x coordinate
				pxBRY;
			GetCorners(pxTLX, pxTLY, pxBRX, pxBRY);
			actual.Make(pxTLX, pxTLY, pxBRX, pxBRY);
			bool flagFound = false;
			for (uint32 obsIndex = 0; obsIndex < obstacles.size(); ++obsIndex) {
				Obstacle& o = obstacles[obsIndex];
				if (actual.aabb.Collide(o.aabb)) {
					EndWithoutCreating();
				}
			}
			if (flagFound) {
				actual.Make(pxTLX, pxTLY, pxBRX, pxBRY);
			}
		}

		//The anchors can be any two opposite corners, we want particular ones
		void GetCorners(int32& pxTLXOut, int32& pxTLYOut, int32& pxBRXOut, int32& pxBRYOut) {
			if (pxAnchor0X < pxAnchor1X) {
				pxTLXOut = pxAnchor0X;
				pxBRXOut = pxAnchor1X;
			} else {
				pxTLXOut = pxAnchor1X;
				pxBRXOut = pxAnchor0X;
			}
			if (pxAnchor0Y < pxAnchor1Y) {
				pxTLYOut = pxAnchor0Y;
				pxBRYOut = pxAnchor1Y;
			} else {
				pxTLYOut = pxAnchor1Y;
				pxBRYOut = pxAnchor0Y;
			}
		}

		void EndWithoutCreating() {
			if (!controls.lmouse) {
				obstacleCreatePhase = notInCreation;
			} else {
				obstacleCreatePhase = end;
			}
		}

		void End() {
			obstacles.push_back(actual);
		}
	};
}
