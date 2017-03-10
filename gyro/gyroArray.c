#pragma systemFile

#if !defined(GYROARRAY_C_)
#define GYROARRAY_C_

#include "./gyro.c"
#include "../util/math.c"
#include "../util/string.c"

#define NUM_ANALOG_PORTS 8

typedef struct {
	short size;
	tSensors ports[NUM_ANALOG_PORTS];

	float biases[NUM_ANALOG_PORTS];
	float scales[NUM_ANALOG_PORTS];
	float deadzone;

	float angle;

	long time;

	TSemaphore sem;
} GyroArray;

GyroArray *newGyroArray(GyroArray *this, tSensors *ports, unsigned short size, float angle) {
	if (this && ports && size <= NUM_ANALOG_PORTS) {
		this->size = size;

		for (unsigned short i = 0; i < NUM_ANALOG_PORTS; i++) {
			if (i < size) {
				this->ports[i] = ports[i];
				SensorType[ports[i]] = sensorAnalog;
			} else {
				this->ports[i] = (tSensors)-1;
			}
			this->biases[i] = 1869.8;  // Should be 1.5V * 1.511 * (2 / 3) * (4095 / 3.3V) = 1875.01363636... .
			this->scales[i] = 1330.0;  // Should be 11V/deg/ms * 1.511 * (2 / 3) * (4095 / 3.3V) = 1375.01/deg/ms.
		}
		this->deadzone = 3.0;  // Should be ~1.0.

		this->angle = angle;

		this->time = 0;

		semaphoreInitialize(this->sem);
	}
	return this;
}

GyroArray *newGyroArray(GyroArray *this, tSensors *ports, unsigned short size) {
	return newGyroArray(this, ports, size, 0.0);
}

GyroArray *newGyroArray(GyroArray *this) {
	tSensors ports[1] = {(tSensors)-1};

	return newGyroArray(this, ports, 0, 0.0);
}

void addGyro(GyroArray *this, tSensors port) {
	if (this && this->size < NUM_ANALOG_PORTS) {
		this->ports[this->size++] = port;
	}
}

tSensors getPort(GyroArray *this, unsigned short index) {
	return (this && index < NUM_ANALOG_PORTS) ? this->ports[index] : (tSensors)-1;
}

void setPort(GyroArray *this, unsigned short index, tSensors port) {
	if (this && index < this->size) {
		this->ports[index] = port;
		SensorType[port] = sensorAnalog;
	}
}

float getAngle(GyroArray *this) {
	return this ? this->angle : 0.0;
}

void setAngle(GyroArray *this, float angle) {
	if (this) {
		semaphoreLock(this->sem);

		this->angle = boundAngle0To360Degrees(angle);

		if (bDoesTaskOwnSemaphore(this->sem)) {
			semaphoreUnlock(this->sem);
		}
	}
}

float getBias(GyroArray *this, unsigned short index) {
	return (this && index < this->size) ? this->biases[index] : 0.0;
}

void setBias(GyroArray *this, unsigned short index, float bias) {
	if (this && index < this->size) {
		this->biases[index] = bias;
	}
}

float getScale(GyroArray *this, unsigned short index) {
	return (this && index < this->size) ? this->scales[index] : 0.0;
}

void setScale(GyroArray *this, unsigned short index, float scale) {
	if (this && index < this->size) {
		this->scales[index] = scale;
	}
}

float getDeadzone(GyroArray *this) {
	return this ? this->deadzone : 0.0;
}

void setDeadzone(GyroArray *this, float deadzone) {
	if (this) {
		this->deadzone = deadzone;
	}
}

void calibrate(GyroArray *this, unsigned short samples, unsigned long delay) {
	if (this == NULL) {
		return;
	}
	unsigned int sums[NUM_ANALOG_PORTS];
	for (unsigned short i = 0; i < this->size; i++) {
		sums[i] = 0;
	}
	for (unsigned short i = 0; i < samples; i++) {
		for (unsigned short j = 0; j < this->size; j++) {
			sums[j] += SensorValue[this->ports[j]];
		}
		sleep(delay);
	}
	for (unsigned short i = 0; i < this->size; i++) {
		this->biases[i] = (float)sums[i] / samples;
	}
}

void update(GyroArray *this) {
	if (this == NULL) {
		return;
	}
	if (this->time == 0) {
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

void print(GyroArray *this) {
	if (this == NULL) {
		return;
	}
	writeDebugStream("Ports: %s", toString(this->ports[0]));
	for (unsigned short i = 1; i < this->size; i++) {
		writeDebugStream(", %s", toString(this->ports[i]));
	}
	writeDebugStream("\n");
	writeDebugStream("Angle: %f\n", this->angle);
	writeDebugStream("Biases: %f", this->biases[0]);
	for (unsigned short i = 1; i < this->size; i++) {
		writeDebugStream(", %f", this->biases[i]));
	}
	writeDebugStream("\n");
	writeDebugStream("Scales: %f", this->scales[i]);
	for (unsigned short i = 1; i < this->size; i++) {
		writeDebugStream(", %f", this->scales[i]));
	}
	writeDebugStream("\n");
	writeDebugStream("Deadzone: %f\n", this->deadzone);
}

#endif  // GYROARRAY_C_
