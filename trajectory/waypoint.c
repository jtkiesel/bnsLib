#pragma systemFile

#if !defined(WAYPOINT_C_)
#define WAYPOINT_C_

typedef struct Waypoint {
	float x;
	float y;
	float theta;

	struct Waypoint *next;
} Waypoint;

Waypoint *newWaypoint(Waypoint *this, float x, float y, float theta,
		Waypoint *next) {
	if (this) {
		this->x = x;
		this->y = y;
		this->theta = theta;

		this->next = next;
	}
	return this;
}

Waypoint *newWaypoint(Waypoint *this, float x, float y, float theta) {
	return newWaypoint(this, x, y, theta, NULL);
}

Waypoint *newWaypoint(Waypoint *this, Waypoint *next) {
	return newWaypoint(this, 0.0, 0.0, 0.0, next);
}

Waypoint *newWaypoint(Waypoint *this) {
	return newWaypoint(this, 0.0, 0.0, 0.0, NULL);
}

float getX(Waypoint *this) {
	return this ? this->x : 0.0;
}

Waypoint *setX(Waypoint *this, float x) {
	if (this) {
		this->x = x;
	}
	return this;
}

float getY(Waypoint *this) {
	return this ? this->y : 0.0;
}

Waypoint *setY(Waypoint *this, float y) {
	if (this) {
		this->y = y;
	}
	return this;
}

float getTheta(Waypoint *this) {
	return this ? this->theta : 0.0;
}

Waypoint *setTheta(Waypoint *this, float theta) {
	if (this) {
		this->theta = theta;
	}
	return this;
}

Waypoint *getNext(Waypoint *this) {
	return this ? this->next : NULL;
}

Waypoint *setNext(Waypoint *this, Waypoint *next) {
	if (this) {
		this->next = next;
	}
	return this;
}

Waypoint *print(Waypoint *this) {
	if (this) {
		writeDebugStream("x: %f\n", this->x);
		writeDebugStream("y: %f\n", this->y);
		writeDebugStream("theta: %f\n", this->theta);
	}
	return this;
}

#endif  // WAYPOINT_C_
