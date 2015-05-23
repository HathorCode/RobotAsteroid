/*
	Defines the polymorphic interface that entities inherit from.
*/
#pragma once

namespace tc {
	//Entities are implimented as a linked list
	struct EntityNode {
		EntityNode* prev;
		EntityNode* next;
		Drawable drawable;
		virtual void Update() = 0;
	};
}
