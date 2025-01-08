// uses the "MPU9250" library by hideakitai

#include "MPU9250.h"

const char separator = ' ';

MPU9250 mpu;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
}

void loop() {
    if (mpu.update()) {
      // we'll update as fast as we can...
        // static uint32_t prev_ms = millis();
        // if (millis() > prev_ms + 25) {
            // print_roll_pitch_yaw();
            // prev_ms = millis();
        // }
        print_x_y_z();
        print_roll_pitch_yaw();
        Serial.println("");
    }
}

void print_roll_pitch_yaw() {
    // Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw());
    Serial.print(separator);
    Serial.print(mpu.getPitch());
    Serial.print(separator);
    Serial.print(mpu.getRoll());
    Serial.print(separator);
}

void print_x_y_z() {
    // Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getAccX());
    Serial.print(separator);
    Serial.print(mpu.getAccY());
    Serial.print(separator);
    Serial.print(mpu.getAccZ());
    Serial.print(separator);
}



