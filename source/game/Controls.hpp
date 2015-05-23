/*
	Provides structures for tracking what controls are activated.
*/
#pragma once

namespace tc {
	struct {
#ifdef _DEBUG
		//Direct ToastCat controls for debugging
		bool up,
			 down,
			 left,
			 right;
#endif
	} activeControls;
}
