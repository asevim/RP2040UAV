
#define SAMPLE_RATE 10
#include "SparkFunLSM6DS3.h"
#include <QMC5883LCompass.h>
#include "Wire.h"
#include <MadgwickAHRS.h>

LSM6DS3 lsm6ds3( I2C_MODE, 0x6A );
QMC5883LCompass qmc5883;
Madgwick filter;

unsigned long microsPerReading, microsPrevious;

void setup()
{

  Serial.begin(9600);

  lsm6ds3.begin();
  qmc5883.init();
  filter.begin(SAMPLE_RATE);  

  microsPerReading = 1000000 / SAMPLE_RATE;
  microsPrevious = micros();
}

void loop()
{
  float gx, gy, gz;
  float ax, ay, az;
  float mx, my, mz;
  float roll, pitch, heading;
  unsigned long microsNow;
  qmc5883.read();


  gx = lsm6ds3.readFloatGyroX();
  gy = lsm6ds3.readFloatGyroY();
  gz = lsm6ds3.readFloatGyroZ();
  ax = lsm6ds3.readFloatAccelX();
  ay = lsm6ds3.readFloatAccelY();
  az = lsm6ds3.readFloatAccelZ();
  mx = qmc5883.getX();  
  my = qmc5883.getY();
  mz = qmc5883.getZ();

  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {
    //filter.update(gx,gy,gz,ax,ay,az,mx,my,mz);
    filter.updateIMU(gx,gy,gz,ax,ay,az);
    
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

    microsPrevious = microsPrevious + microsPerReading;
  }
  
  Serial.print("Orientation: ");
  Serial.print(heading);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.println(roll);
  
}
