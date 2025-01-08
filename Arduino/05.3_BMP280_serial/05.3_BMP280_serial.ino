// for BMP 280 sensor
// uses BMP280 library by dvarrel

#include <BMP280.h>

char separator = ' ';

BMP280 bmp280;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.
  bmp280.begin();
}

void loop() {
  Serial.print(bmp280.getTemperature());
  Serial.print(separator);
  Serial.print(bmp280.getPressure());
  Serial.println("");
  // delay(100);
}
