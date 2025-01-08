// for the BMP 180 sensor
// uses Adafruit BMP085 Library

#include <Adafruit_BMP085.h>

char separator = ' ';

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);

  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}

void loop() {
  Serial.print(bmp.readTemperature());
  Serial.print(separator);
  Serial.print(bmp.readPressure());
  Serial.println("");
  // delay(100);
}
