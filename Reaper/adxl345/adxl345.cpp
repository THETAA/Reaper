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

#define ADXL345  0x1D

int i2cA;

short rawAccX, rawAccY, rawAccZ;

void selectDeviceA(int i2c, int addr)
{
   if (ioctl(i2c, I2C_SLAVE, addr) < 0)
   {
      fprintf(stderr, "ADXL345 not present\n");
   }
}

void writeToDeviceA(int i2c, char * buf, int len)
{
   if (write(i2c, buf, len) != len)
   {
      fprintf(stderr, "Can't write to device\n");
   }
}

void initADXL345()
{
   if ((i2cA = open("/dev/i2c-1", O_RDWR)) < 0)
   {
      perror("Failed to open i2c bus");
      exit(1);
   }
   selectDeviceA(i2cA, ADXL345);

   if (write(i2cA, "\x2d\x00", 2) != 2) fprintf(stderr, "Can't write to device\n");
   if (write(i2cA, "\x2d\x10", 2) != 2) fprintf(stderr, "Can't write to device\n");
   if (write(i2cA, "\x2d\x08", 2) != 2) fprintf(stderr, "Can't write to device\n");
   if (write(i2cA, "\x2d\x00", 2) != 2) fprintf(stderr, "Can't write to device\n");
   if (write(i2cA, "\x2d\x0b", 2) != 2) fprintf(stderr, "Can't write to device\n");
}

void readADXL345(int i2c)
{
   char buf[8];
   short tmp;

   selectDeviceA(i2c, ADXL345);

   if (write(i2c, "\x32", 1) != 1) fprintf(stderr, "Can't write to device\n");
   
   if (read(i2c, buf, 6) != 6)
   {
      fprintf(stderr, "Unable to read from ADXL345\n");
   }
   else
   {
      tmp = buf[1]<<8 | buf[0];
      if ((tmp < 128)  && (tmp > (-128))) rawAccX = tmp;

      tmp = buf[3]<<8 | buf[2];
      rawAccY = tmp;

      tmp = buf[5]<<8 | buf[4];
      rawAccZ = tmp;
   }
}

short getRawAccX() {
   readADXL345(i2cA);
   return rawAccX;
}

short getRawAccY() {
   readADXL345(i2cA);
   return rawAccY;
}

short getRawAccZ() {
   readADXL345(i2cA);
   return rawAccZ;
}