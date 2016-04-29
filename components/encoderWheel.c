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

static const tSensors kDefaultPort = (tSensors)-1;
static const float kDefaultPulsesPerRev = -1.0;
static const float kDefaultWheelDiamter = -1.0;
static const float kDefaultGearRatio = 1.0;
static const float kDefaultSlipFactor = 1.0;
static const bool kDefaultInverted = false;

static const tSensors kMinInPort = in1;
static const tSensors kMaxInPort = in8;
static const tSensors kMinDgtlPort = dgtl1;
static const tSensors kMaxDgtlPort = dgtl12;
static const tSensors kMinI2cPort = I2C_1;
static const tSensors kMaxI2cPort = I2C_8;
static const string kInPortString = "in";
static const string kDgtlPortString = "dgtl";
static const string kI2cPortString = "I2C_";

static const string kTrueString = "true";
static const string kFalseString = "false";

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiameter, float gearRatio, float slipFactor,
		bool inverted) {
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
		float pulsesPerRev, float wheelDiamter, float gearRatio, float slipFactor) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			slipFactor, kDefaultInverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, float gearRatio, bool inverted) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			kDefaultSlipFactor, inverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, float gearRatio) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter, gearRatio,
			kDefaultSlipFactor, kDefaultInverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, bool inverted) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter,
			kDefaultGearRatio, kDefaultSlipFactor, inverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this, tSensors port,
		float pulsesPerRev, float wheelDiamter, bool inverted) {
	return newEncoderWheel(this, port, pulsesPerRev, wheelDiamter,
			kDefaultGearRatio, kDefaultSlipFactor, kDefaultInverted);
}

EncoderWheel *newEncoderWheel(EncoderWheel *this) {
	return newEncoderWheel(this, kDefaultPort, kDefaultPulsesPerRev,
			kDefaultWheelDiamter, kDefaultGearRatio, kDefaultSlipFactor,
			kDefaultInverted);
}

tSensors getPort(EncoderWheel *this) {
	return this ? this->port : -1;
}

EncoderWheel *setPort(EncoderWheel *this, tSensors port) {
	if (this) {
		this->port = port;
	}
	return this;
}

float getPulsesPerRev(EncoderWheel *this) {
	return this ? this->pulsesPerRev : 0.0;
}

EncoderWheel *setPulsesPerRev(EncoderWheel *this, float pulsesPerRev) {
	if (this) {
		this->pulsesPerRev = pulsesPerRev;
	}
	return this;
}

float getWheelDiameter(EncoderWheel *this) {
	return this ? this->wheelDiameter : kDefaultWheelDiamter;
}

EncoderWheel *setWheelDiameter(EncoderWheel *this, float wheelDiameter) {
	if (this) {
		this->wheelDiameter = wheelDiameter;
	}
	return this;
}

float getGearRatio(EncoderWheel *this) {
	return this ? this->gearRatio : kDefaultGearRatio;
}

EncoderWheel *setGearRatio(EncoderWheel *this, float gearRatio) {
	if (this) {
		this->gearRatio = gearRatio;
	}
	return this;
}

float getSlipFactor(EncoderWheel *this) {
	return this ? this->slipFactor : kDefaultSlipFactor;
}

EncoderWheel *setSlipFactor(EncoderWheel *this, float  slipFactor) {
	if (this) {
		this->slipFactor = slipFactor;
	}
	return this;
}

bool getInverted(EncoderWheel *this) {
	return this ? this->inverted : kDefaultInverted;
}

EncoderWheel *setInverted(EncoderWheel *this, bool inverted) {
	if (this) {
		this->inverted = inverted;
	}
	return this;
}

long getPulses(EncoderWheel *this) {
	long pulses = 0;

	if (this) {
		pulses = (this->inverted ? -1 : 1) * SensorValue[this->port];
	}
	return pulses;
}

float getDistance(EncoderWheel *this) {
	float distance = 0.0;

	if (this) {
		distance = ((this->inverted ? -1.0 : 1.0) * SensorValue[this->port] * PI
				* this->wheelDiameter) / (this->pulsesPerRev * this->gearRatio
				* this->slipFactor);
	}
	return distance;
}

void print(EncoderWheel *this) {
	if (this) {
		string portString = "", invertedString;
		tSensors port = this->port;

		if (port >= kMinInPort && port <= kMaxInPort) {
			portString = kInPortString;
			port -= kMinInPort - 1;
		} else if (port >= kMinDgtlPort && port <= kMaxDgtlPort) {
			portString = kDgtlPortString;
			port -= kMinDgtlPort - 1;
		} else if (port >= kMinI2cPort && port <= kMaxI2cPort) {
			portString = kI2cPortString;
			port -= kMinI2cPort - 1;
		}
		invertedString = this->inverted ? kTrueString : kFalseString;

		writeDebugStream("Port: %s%d\n", portString, port);
		writeDebugStream("Pulses Per Rev: %f\n", this->pulsesPerRev);
		writeDebugStream("Wheel Diameter: %f\n", this->wheelDiameter);
		writeDebugStream("Gear Ratio: %f\n", this->gearRatio);
		writeDebugStream("Slip Factor: %f\n", this->slipFactor);
		writeDebugStream("Inverted: %s\n", invertedString);
		writeDebugStream("Distance: %f\n", getDistance(this));
	}
}

#endif  // ENCODERWHEEL_C_
