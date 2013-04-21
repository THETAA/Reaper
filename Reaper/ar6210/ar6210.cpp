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

#define AR6210_0 4

long AR0pwm = 0;

void *AR0(void*) {
   bool isTimerRunning = false;
   bool stop = false;
   while (1) {
      struct timeval start, end;
      if (digitalRead(AR6210_0) == 1) {
         if (!(isTimerRunning)) {
            gettimeofday(&start, NULL);
            isTimerRunning = true;
            stop = true;
         }
      } else if (digitalRead(AR6210_0) == 0) {
         if (stop) {
            isTimerRunning = false;
            gettimeofday(&end, NULL);
            AR0pwm = end.tv_usec - start.tv_usec - 1150;
            stop = false;
            usleep(50000);
         }
         
      }
   }
   return 0;
}

long getAR0pwm() {
   return AR0pwm;
}