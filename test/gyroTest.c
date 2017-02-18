#include "../gyro/gyro.c"

Gyro gyro;

task background() {
	while (true) {
		update(&gyro);

		sleep(1);
	}
}

task main() {
	newGyro(&gyro, in1);

	startTask(background);

	while (getAngle(&gyro) < 90.0) {
		motor[port2] = 127;
		motor[port3] = -127;
	}
	motor[port2] = motor[port3] = 0;
}
