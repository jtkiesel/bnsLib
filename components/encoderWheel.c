#pragma systemFile

#if !defined(ENCODERWHEEL_C_)
#define ENCODERWHEEL_C_

typedef struct {
	tSensors port;
	float pulsesPerRev;
	float wheelDiameter;
	float gearRatio;
	float slipFactor;
	bool inverted;
} EncoderWheel;

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiameter, float gearRatio,
		float slipFactor, bool inverted) {
	if (this) {
		this->port = port;
		this->pulsesPerRev = pulsesPerRev;
		this->wheelDiameter = wheelDiameter;
		this->gearRatio = gearRatio;
		this->slipFactor = slipFactor;
		this->inverted = inverted;
	}
	return this;
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, float gearRatio,
		float slipFactor) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			slipFactor, false);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, float gearRatio,
		bool inverted) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			1.0, inverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, float gearRatio) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			1.0, false);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, bool inverted) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, 1.0, 1.0,
			inverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, 1.0, 1.0,
			false);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this) {
	return newEncoderWheel(this, (tSensors)-1, 0.0, 0.0, 1.0, 1.0, false);
}

tSensors getPort(EncoderWheel *this) {
	return this ? this->port : (tSensors)-1;
}

void setPort(EncoderWheel *this, tSensors port) {
	if (this) {
		this->port = port;
	}
	return this;
}

float getPulsesPerRev(EncoderWheel *this) {
	return this ? this->pulsesPerRev : 0.0;
}

void setPulsesPerRev(EncoderWheel *this, float pulsesPerRev) {
	if (this) {
		this->pulsesPerRev = pulsesPerRev;
	}
	return this;
}

float getWheelDiameter(EncoderWheel *this) {
	return this ? this->wheelDiameter : 0.0;
}

void setWheelDiameter(EncoderWheel *this, float wheelDiameter) {
	if (this) {
		this->wheelDiameter = wheelDiameter;
	}
	return this;
}

float getGearRatio(EncoderWheel *this) {
	return this ? this->gearRatio : 1.0;
}

void setGearRatio(EncoderWheel *this, float gearRatio) {
	if (this) {
		this->gearRatio = gearRatio;
	}
	return this;
}

float getSlipFactor(EncoderWheel *this) {
	return this ? this->slipFactor : 1.0;
}

void setSlipFactor(EncoderWheel *this, float slipFactor) {
	if (this) {
		this->slipFactor = slipFactor;
	}
	return this;
}

bool getInverted(EncoderWheel *this) {
	return this ? this->inverted : false;
}

void setInverted(EncoderWheel *this, bool inverted) {
	if (this) {
		this->inverted = inverted;
	}
	return this;
}

long getPulses(EncoderWheel *this) {
	if (this == NULL) {
		return 0;
	}
	return (this->inverted ? -1 : 1) * SensorValue[this->port];
}

float getDistance(EncoderWheel *this) {
	if (this == NULL) {
		return 0.0;
	}
	return (getPulses(this) * PI * this->wheelDiameter)
			/ (this->pulsesPerRev * this->gearRatio * this->slipFactor);
}

void print(EncoderWheel *this) {
	if (this == NULL) {
		return NULL;
	}
	char *portString;
	char *invertedString;
	tSensors port = this->port;

	if (port >= in1 && port <= in8) {
		portString = "in";
		port -= in1 - 1;
	} else if (port >= dgtl1 && port <= dgtl12) {
		portString = "dgtl";
		port -= dgtl1 - 1;
	} else if (port >= I2C_1 && port <= I2C_8) {
		portString = "I2C_";
		port -= I2C_1 - 1;
	}
	invertedString = this->inverted ? "true" : "false";

	writeDebugStream("Port: %s%d\n", portString, port);
	writeDebugStream("Pulses Per Rev: %f\n", this->pulsesPerRev);
	writeDebugStream("Wheel Diameter: %f\n", this->wheelDiameter);
	writeDebugStream("Gear Ratio: %f\n", this->gearRatio);
	writeDebugStream("Slip Factor: %f\n", this->slipFactor);
	writeDebugStream("Inverted: %s\n", invertedString);
	writeDebugStream("Distance: %f\n", getDistance(this));

	return this;
}

#endif  // ENCODERWHEEL_C_
