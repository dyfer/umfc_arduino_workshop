// requires LSM6 by Polulu library

// based on the included example
/*
The sensor outputs provided by the library are the raw
16-bit values obtained by concatenating the 8-bit high and
low accelerometer and gyro data registers. They can be
converted to units of g and dps (degrees per second) using
the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM6DS33 gives an accelerometer Z axis reading
of 16276 with its default full scale setting of +/- 2 g. The
LA_So specification in the LSM6DS33 datasheet (page 15)
states a conversion factor of 0.061 mg/LSB (least
significant bit) at this FS setting, so the raw reading of
16276 corresponds to 16276 * 0.061 = 992.8 mg = 0.9928 g.
*/

#include <Wire.h>
#include <LSM6.h>

const char separator = ' ';

LSM6 imu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  if (!imu.init()) {
    Serial.println("Failed to detect and initialize IMU!");
    while (1)
      ;
  }
  imu.enableDefault();
}

void loop() {
  imu.read();
  print_x_y_z();
  Serial.println("");
}

void print_x_y_z() {
  Serial.print(imu.a.x * 0.000061);
  Serial.print(separator);
  Serial.print(imu.a.y * 0.000061);
  Serial.print(separator);
  Serial.print(imu.a.z * 0.000061);
  Serial.print(separator);
}
