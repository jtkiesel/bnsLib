#pragma systemFile

#if !defined(GYRO_C_)
#define GYRO_C_

#include "../util/math.c"
#include "../util/string.c"

typedef struct {
	tSensors port;
	float angle;

	float bias;
	float deadzone;
	float scale;

	long time;

	TSemaphore sem;
} Gyro;

Gyro *newGyro(Gyro *this, tSensors port, float angle) {
	if (this) {
		this->port = port;
		this->angle = angle;

		this->bias = 1869.8;   // Should be 1.5V * 1.511 * (2 / 3) * (4095 / 3.3V) = 1875.01363636... .
		this->deadzone = 3.0;  // Should be ~1.0.
		this->scale = 1330.0;  // Should be 11V/deg/ms * 1.511 * (2 / 3) * (4095 / 3.3V) = 1375.01/deg/ms.

		this->time = NULL;

		semaphoreInitialize(this->sem);

		SensorType[port] = sensorAnalog;
	}
	return this;
}

Gyro *newGyro(Gyro *this, tSensors port) {
	return newGyro(this, port, 0.0);
}

tSensors getPort(Gyro *this) {
	return this ? this->port : (tSensors)-1;
}

void setPort(Gyro *this, tSensors port) {
	if (this) {
		this->port = port;

		SensorType[port] = sensorAnalog;
	}
}

float getAngle(Gyro *this) {
	return this ? this->angle : 0.0;
}

void setAngle(Gyro *this, float angle) {
	if (this) {
		semaphoreLock(this->sem);

		this->angle = boundAngle0To360Degrees(angle);

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
}

float getBias(Gyro *this) {
	return this ? this->bias : 0.0;
}

void setBias(Gyro *this, float bias) {
	if (this) {
		this->bias = bias;
	}
}

float getDeadzone(Gyro *this) {
	return this ? this->deadzone : 0.0;
}

void setDeadzone(Gyro *this, float deadzone) {
	if (this) {
		this->deadzone = deadzone;
	}
}

float getScale(Gyro *this) {
	return this ? this->scale : 0.0;
}

void setScale(Gyro *this, float scale) {
	if (this) {
		this->scale = scale;
	}
}

void calibrate(Gyro *this, short samples, long delay) {
	if (this == NULL) {
		return;
	}
	int sum = 0;
	for (short i = 0; i < samples; i++) {
		sum += SensorValue[this->port];

		sleep(delay);
	}
	this->bias = (float)sum / samples;
}

void update(Gyro *this) {
	if (this == NULL) {
		return;
	}
	if (this->time == NULL) {
		this->time = nSysTime;

		return;
	}
	long dt = nSysTime - this->time;
	float da = SensorValue[this->port] - this->bias;

	if (fabs(da) > this->deadzone) {
		semaphoreLock(this->sem);

		this->angle = boundAngle0To360Degrees(this->angle + (da / this->scale) * dt);

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
	this->time += dt;
}

void print(Gyro *this) {
	if (this == NULL) {
		return;
	}
	writeDebugStream("Port: %s\n", toString(this->port));
	writeDebugStream("Angle: %f\n", this->angle);
	writeDebugStream("Bias: %f\n", this->bias);
	writeDebugStream("Deadzone: %f\n", this->deadzone);
	writeDebugStream("Scale: %f\n", this->scale);
}

#endif  // GYRO_C_
