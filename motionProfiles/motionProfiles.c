#pragma systemFile

#if !defined(MOTIONPROFILE_C_)
#define MOTIONPROFILE_C_

typedef struct {
	float maxVel;
	float maxAcc;

	float v0;
	float v1;
} MotionProfile;

MotionProfile *newMotionProfile(MotionProfile *this, float maxVel,
		float maxAcc, float v0, float v1) {
	if (this) {
		this->maxVel = maxVel;
		this->maxAcc = maxAcc;

		this->v0 = v0;
		this->v1 = v1;
	}
	return this;
}

float update(MotionProfile *this, float error, float t) {
	// Find target velocity based on time (acceleration).
	float v = this->v0 + this->maxAcc * t;  // v = v0 + a*t
	// Keep target velocity within its limit.
	if (v > this->maxVel) {
		v = sgn(v) * this->maxVel;
	}
	// If it is time to decelerate (use v1 in place of v0 to decelerate).
	// ds = (v^2 - v0^2) / (2*a) derived from v^2 = v0^2 + 2*a*ds.
	if (error <= (pow(v, 2) - pow(this->v1, 2)) / (2 * this->maxAcc)) {
		// Find velocity based on error (deceleration).
		// v = sqrt(v0^2 + 2*a*ds) derived from v^2 = v0^2 + 2*a*ds.
		v = sgn(this->maxAcc * error) * sqrt(fabs(pow(this->v1, 2)
				+ 2 * this->maxAcc * error));
	}
	return v;  // Return target velocity.
}

#endif  // MOTION_PROFILE_C_
