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

#include <softPwm.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <pthread.h>

#define ITG3200  0x69

#define GyroXzero 15.1
#define GyroYzero -2.2

int i2c;
short rawGyroX, rawGyroY, rawGyroZ;

void selectDevice(int i2c, int addr)
{
   if (ioctl(i2c, I2C_SLAVE, addr) < 0)
   {
      fprintf(stderr, "ITG-3200 not present\n");
   }
}

void initITG3200()
{
   if ((i2c = open("/dev/i2c-1", O_RDWR)) < 0)
   {
      perror("Failed to open i2c bus");
      exit(1);
   }
   selectDevice(i2c, ITG3200);

   if (write(i2c, "\x16\b00011000", 2) != 2)
   {
      fprintf(stderr, "Can't write to device\n");
   }
}

void readITG3200(int i2c)
{
   char buf[8];

   selectDevice(i2c, ITG3200);

   if (write(i2c, "\x1D", 1) != 1)
   {
      fprintf(stderr, "Can't write to device\n");
   }
   
   if (read(i2c, buf, 6) != 6)
   {
      fprintf(stderr, "Unable to read from ITG3200\n");
   }
   else
   {
      rawGyroX = buf[0]<<8 | buf[1];
      rawGyroY = buf[2]<<8 | buf[3];
      rawGyroZ = buf[4]<<8 | buf[5];
   }
}

short getRawGyroX() {
   readITG3200(i2c);
   return rawGyroX;
}

short getRawGyroY() {
   readITG3200(i2c);
   return rawGyroY;
}

short getRawGyroZ() {
   readITG3200(i2c);
   return rawGyroZ;
}

double getGyroRateX() {
   double rate = -(((double)getRawGyroX()+GyroXzero)/14.375);  
   return rate;
}

double getGyroRateY() {
   double rate = -(((double)getRawGyroY()+GyroYzero)/14.375);  
   return rate;
}