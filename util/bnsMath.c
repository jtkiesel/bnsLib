#pragma systemFile

#if !defined(BNSMATH_C_)
#define BNSMATH_C_

short max(short a, short b) {
	return (a >= b) ? a : b;
}

int max(int a, int b) {
	return (a >= b) ? a : b;
}

float max(float a, float b) {
	if (a != a) {
		return a;  // a is NaN.
	}
	return (a >= b) ? a : b;
}

short min(short a, short b) {
	return (a <= b) ? a : b;
}

int min(int a, int b) {
	return (a <= b) ? a : b;
}

float min(float a, float b) {
	if (a != a) {
		return a;  // a is NaN.
	}
	return (a <= b) ? a : b;
}

float tan(float angle) {
	return sin(angle) / cos(angle);  // Less than ideal solution.
}

float degreesToRadians(float angdeg) {
	return angdeg / 180.0 * PI;
}

float boundAngle0to2PiRadians(float angle) {
	// Naive algorithm.
	while (angle >= 2.0 * PI) {
		angle -= 2.0 * PI;
	}
	while (angle < 0.0) {
		angle += 2.0 * PI;
	}
	return angle;
}

float boundAngleNegPiToPiRadians(float angle) {
	// Naive algorithm.
	while (angle >= PI) {
		angle -= 2.0 * PI;
	}
	while (angle < -PI) {
		angle += 2.0 * PI;
	}
	return angle;
}

/**
 * Get the difference in angle between two angles.
 *
 * @param 	from	The first angle.
 * @param 	to  	The second angle.
 *
 * @return	The change in angle from the first argument necessary to line up
 *        	with the second. Always between -Pi and Pi.
 */
float getDifferenceInAngleRadians(float from, float to) {
	return boundAngleNegPiToPiRadians(to - from);
}

/**
 * Limits the given input to the given magnitude.
 *
 * @param 	v    	The value to limit.
 * @param 	limit	The magnitude to limit to.
 *
 * @return	The limited value.
 */
float limit(float v, float limit) {
	return (fabs(v) < limit) ? v : sgn(v) * limit;
}

#endif  // BNSMATH_C_
