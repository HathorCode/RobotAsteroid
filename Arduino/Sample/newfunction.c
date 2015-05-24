#import <elapsedMillis.h>

elapsedMillis timeElapsed;

void keep_straight(int time) {
	timeElapsed = 0;
	const int init_z = MPU.m_dmpEulerPose[VEC3_Z];
	int change;
	int turn;

	while (timeElapsed < time) {
		change = change_circle(init_z, MPU.m_dmpEulerPose[VEC3_Z]);

		turn = 100 + (-change/180)*40;

		//turn the servo to the turn value;

	}
}

int change_circle(int init, int fin) {
	int change = -(fin - init);
		  
	if (abs(change) < 180) {
		return change;
	} else if (change > 0) {
		return -(360 - change);
	} else if (change < 0) {
		return 360 + change;
	} else {
		return 0;
	}
}