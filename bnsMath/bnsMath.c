#pragma systemFile

#if !defined(BNSMATH_C_)
#define BNSMATH_C_

int max(int a, int b) {
	return (a >= b) ? a : b;
}

long max(long a, long b) {
	return (a >= b) ? a : b;
}

float max(float a, float b) {
	if (a != a) {
		return a;   // a is NaN.
	}
	return (a >= b) ? a : b;
}

int min(int a, int b) {
	return (a <= b) ? a : b;
}

long min(long a, long b) {
	return (a <= b) ? a : b;
}

float min(float a, float b) {
	if (a != a) {
		return a;  // a is NaN.
	}
	return (a <= b) ? a : b;
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

#endif  // BNSMATH_C_
