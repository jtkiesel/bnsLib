#pragma systemFile

#if !defined(NAVIGATOR_C_)
#define NAVIGATOR_C_

#include "../bnsMath/bnsMath.c"
#include "../components/encoderWheel.c"

typedef struct {
  EncoderWheel *leftEncoder;
  EncoderWheel *rightEncoder;
  EncoderWheel *middleEncoder;
  float driveWidth;
  float x;
  float y;
  float heading;
  float lastL;
  float lastR;
  float lastM;
  TSemaphore sem;
} Navigator;

Navigator *newNavigator(Navigator *this, EncoderWheel *leftEncoder,
		EncoderWheel *rightEncoder, EncoderWheel *middleEncoder, float driveWidth,
		float x, float y, float heading) {
	if (this) {
		this->leftEncoder = leftEncoder;
		this->rightEncoder = rightEncoder;
		this->middleEncoder = middleEncoder;
		this->driveWidth = driveWidth;
		this->x = x;
		this->y = y;
		this->heading = heading;
		this->lastL = getDistance(leftEncoder);
		this->lastR = getDistance(rightEncoder);
		this->lastM = getDistance(middleEncoder);
		semaphoreInitialize(this->sem);
	}
	return this;
}

Navigator *newNavigator(Navigator *this, EncoderWheel *leftEncoder,
		EncoderWheel *rightEncoder, EncoderWheel *middleEncoder, float driveWidth) {
	return newNavigator(this, leftEncoder, rightEncoder, middleEncoder,
			driveWidth, 0.0, 0.0, 0.0);
}

Navigator *newNavigator(Navigator *this, EncoderWheel *leftEncoder,
		EncoderWheel *rightEncoder, float driveWidth, float x, float y,
		float heading) {
	return newNavigator(this, leftEncoder, rightEncoder, NULL, driveWidth, x, y,
			heading);
}

Navigator *newNavigator(Navigator *this, EncoderWheel *leftEncoder,
		EncoderWheel *rightEncoder, float driveWidth) {
	return newNavigator(this, leftEncoder, rightEncoder, NULL, driveWidth, 0.0,
			0.0, 0.0);
}

Navigator *newNavigator(Navigator *this) {
	return newNavigator(this, NULL, NULL, NULL, 0.0, 0.0, 0.0, 0.0);
}

Navigator *updateNavigator(Navigator *this) {
	if (this) {
		float diffL = getDistance(this->leftEncoder) - this->lastL;
		float diffR = getDistance(this->rightEncoder) - this->lastR;
		float diffM = getDistance(this->middleEncoder) - this->lastM;

		float diffH = (diffR - diffL) / this->driveWidth;
		float tempHeading = this->heading + (diffH / 2.0);
		float magnitude = (diffL + diffR) / 2.0;

		semaphoreLock(this->sem);

		this->x += (magnitude * sin(tempHeading)) + (diffM * cos(tempHeading));
		this->y += (magnitude * cos(tempHeading)) + (diffM * sin(tempHeading));
		this->heading = boundAngle0to2PiRadians(this->heading + diffH);

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
		this->lastL += diffL;
		this->lastR += diffR;
		this->lastM += diffM;
	}
	return this;
}

EncoderWheel *getLeftEncoder(Navigator *this) {
	return this ? this->leftEncoder : NULL;
}

Navigator *setLeftEncoder(Navigator *this, EncoderWheel *leftEncoder) {
	if (this) {
		this->leftEncoder = leftEncoder;
	}
	return this;
}

EncoderWheel *getRightEncoder(Navigator *this) {
	return this ? this->rightEncoder : NULL;
}

Navigator *setRightEncoder(Navigator *this, EncoderWheel *rightEncoder) {
	if (this) {
		this->rightEncoder = rightEncoder;
	}
	return this;
}

EncoderWheel *getMiddleEncoder(Navigator *this) {
	return this ? this->middleEncoder : NULL;
}

Navigator *setMiddleEncoder(Navigator *this, EncoderWheel *middleEncoder) {
	if (this) {
		this->middleEncoder = middleEncoder;
	}
	return this;
}

float getDriveWidth(Navigator *this) {
	return this ? this->driveWidth : 0.0;
}

Navigator *setDriveWidth(Navigator *this, float driveWidth) {
	if (this) {
		this->driveWidth = driveWidth;
	}
	return this;
}

float getX(Navigator *this) {
	return this ? this->x : 0.0;
}

Navigator *setX(Navigator *this, float x) {
	if (this) {
		semaphoreLock(this->sem);

		this->x = x;

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
	return this;
}

float getY(Navigator *this) {
	return this ? this->y : 0.0;
}

Navigator *setY(Navigator *this, float y) {
	if (this) {
		semaphoreLock(this->sem);

		this->y = y;

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
	return this;
}

float getHeading(Navigator *this) {
	return this ? this->heading : NULL;
}

Navigator *setHeading(Navigator *this, float heading) {
	if (this) {
		semaphoreLock(this->sem);

		this->heading = boundAngle0to2PiRadians(heading);

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
	return this;
}

Navigator *printNavigator(Navigator *this) {
	if (this) {
		writeDebugStream("Drive Width: %d\n", this->driveWidth);
		writeDebugStream("x: %f\n", this->x);
		writeDebugStream("y: %f\n", this->y);
		writeDebugStream("Heading: %f\n", this->heading);
	}
	return this;
}

#endif  // NAVIGATOR_C_
