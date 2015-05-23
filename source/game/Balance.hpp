/*
	Defines game balance constants for the game.
*/
#pragma once

namespace tc {
	const float32 mspt = (float32)(1.0L/30.0L * 1000.0L);   //30 FPS update speed or 33.3... ms
	const float32 cameraLag = 0.5f;                         /*Camera lag,
	                                                          .5 means that once the player is halfway off of either half of the screen
	                                                          (3/4 the way off of the whole screen),
	                                                          the camera will start chasing*/
#ifdef _DEBUG
	const float32 mptUpSpeed = (float32)(0.00536448 * mspt);      //speed at which player goes forward when debug movement is active
	const float32 mptDownSpeed = mptUpSpeed;      //in meters per tick
	const float32 mptLeftSpeed = mptUpSpeed;
	const float32 mptRightSpeed = mptUpSpeed;
#endif
}
