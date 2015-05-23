/*
	Defines a game world (level).
*/
#pragma once
#include "Planet.hpp"

#include <engine/renderer/Texture.hpp>

#include <game/Entity.hpp>

namespace tc {
	struct World {
		Sprite background;
		
		EntityNode* nodes = nullptr;
		uint32 countEntities = 0;

		EntityNode* GetEntity(uint32 n) const {
			assert(n < countEntities);
			EntityNode* r = nodes;
			for (uint32 entIndex = 0; entIndex < n; ++entIndex) {
				assert(r != nullptr);
				r = r->next;
			}
			return r;
		} EntityNode* operator[](uint32 n) const { return GetEntity(n); }

		EntityNode* GetLastEntity() const {
			return this[0][countEntities - 1];
		}

		void AddEntity(EntityNode* newNode) {
			if (nodes != nullptr) {
				auto lastEntity = GetLastEntity();
				lastEntity->next = newNode;
				newNode->prev = lastEntity;
			} else {
				nodes = newNode;
				newNode->prev = nullptr;
				newNode->next = nullptr;
			}
			++countEntities;
		}
	};
}
