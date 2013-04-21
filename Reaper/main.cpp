#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <wiringPi.h>
#include <softServo.h>

#include "kalman.h"
#include "time.h"

#include "ar6210/ar6210.h"
#include "itg3200/itg3200.h"
#include "adxl345/adxl345.h"

#define multiply 1

using namespace std;

Kalman kX;
Kalman kY;

double accX;
double rateX;

double accY;
double rateY;

double roll;
double pitch;

unsigned long timer;
unsigned long timerR;
long AR0val;

int pin0 = 0;
int pin1 = 1;
int pin2 = 2;
int pin3 = 3;

void calibrate() {
	printf("Calibrating...\n");
	softServoWrite (pin0,  0) ;
	softServoWrite (pin1,  0) ;
	softServoWrite (pin2,  0) ;
	softServoWrite (pin3,  0) ;
	delay(10000);
	printf("Calibrated!\n");
}

void *loopR(void*) {
	while (1) {
		accX = getRawAccX();
		rateX = getGyroRateX();

		roll = kX.getAngle(accX,rateX,double(getDTR() - timerR)/1000000);
  		timerR = getDTR();

  		usleep(10000);
	}
	return 0;
}

void *loopP(void*) {
	while (1) {
		accY = getRawAccY();
		rateY = getGyroRateY();

		pitch = kY.getAngle(accY,rateY,double(getDTP() - timer)/1000000);
  		timer = getDTP();

  		usleep(10000);
	}
	return 0;
}

float returnPos(float value) {
	if (value > 0) return value;
	else return 0;
}


int main() {
	initITG3200();
	initADXL345();

	if (wiringPiSetup () == -1)
    	exit (1) ;

    softServoSetup (pin0, pin1, pin2, pin3, NULL, NULL, NULL, NULL) ;
	calibrate();

	pthread_t tloopP;
	pthread_t tloopR;
   	pthread_t tAR0;
   	pthread_t tESCcontrol;

   	pthread_create(&tAR0, NULL, &AR0, NULL);
	pthread_create(&tloopP, NULL, &loopP, NULL);
	pthread_create(&tloopR, NULL, &loopR, NULL);

	while(1) {
		AR0val = getAR0pwm();

		softServoWrite(pin0, AR0val + returnPos(pitch*multiply) + returnPos(roll*multiply*-1));
		softServoWrite(pin1, AR0val + returnPos(pitch*multiply) + returnPos(roll*multiply));
		softServoWrite(pin2, AR0val + returnPos(pitch*multiply*-1) + returnPos(roll*multiply*-1));
		softServoWrite(pin3, AR0val + returnPos(pitch*multiply*-1)+returnPos(roll*multiply));

		usleep(10000);
	}
}