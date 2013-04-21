#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

long getDTP()
{
   static long last_secP, last_usecP;
   long microsP;
   struct timeval nowP;

   gettimeofday(&nowP, NULL);

   if (nowP.tv_sec > last_secP) microsP = 1000000L; else microsP = 0;
   microsP = microsP + (nowP.tv_usec - last_usecP);

   last_secP = nowP.tv_sec;
   last_usecP = nowP.tv_usec;

   return microsP;
}

long getDTR()
{
   static long last_secR, last_usecR;
   long microsR;
   struct timeval nowR;

   gettimeofday(&nowR, NULL);

   if (nowR.tv_sec > last_secR) microsR = 1000000L; else microsR = 0;
   microsR = microsR + (nowR.tv_usec - last_usecR);

   last_secR = nowR.tv_sec;
   last_usecR = nowR.tv_usec;

   return microsR;
}