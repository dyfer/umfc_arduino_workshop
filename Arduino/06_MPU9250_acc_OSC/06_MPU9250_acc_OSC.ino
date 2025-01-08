// requires MPU9250 library

// for the ESP8266 board!

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include "MPU9250.h"

MPU9250 mpu;
WiFiUDP Udp;

#ifndef STASSID
#define STASSID "<network-name>"
#define STAPSK "<password>"
#endif

unsigned int localPort = 8888;  // local port to listen on
// unsigned int destPort; // we'll get that from the message sent at first
unsigned int destPort = 9000;  // we'll get that from the message sent at first
IPAddress destIP;

const char separator = ' ';

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
// char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // for ESP8266 we define SDA/SCL pins

  delay(200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.println("");
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);

  if (!mpu.setup(0x68)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort(), ESP.getFreeHeap());

    destIP = Udp.remoteIP();
    // destPort = Udp.remotePort(); // this doesn't work with Max, since Max sends from a random port

    // we don't care about further processing

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // // send a reply, to the IP address and port that sent us the packet we received
    // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    // Udp.write(ReplyBuffer);
    // Udp.endPacket();
  }


  if (mpu.update()) {
    // we'll update as fast as we can...
    // static uint32_t prev_ms = millis();
    // if (millis() > prev_ms + 25) {
    // print_yaw_pitch_roll();
    // prev_ms = millis();
    // }
    // print_yaw_pitch_roll();
    print_x_y_z();
    Serial.println("");
    // if we get the port, then let's start sending
    if (destIP) {
      send_yaw_pitch_roll();
      send_x_y_z();
    }
  }
}

void print_yaw_pitch_roll() {
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

void send_yaw_pitch_roll() {

  OSCMessage msg("/ypr");
  msg.add((float)mpu.getYaw());
  msg.add((float)mpu.getPitch());
  msg.add((float)mpu.getRoll());

  Udp.beginPacket(destIP, destPort);
  msg.send(Udp);    // send the bytes to the SLIP stream
  Udp.endPacket();  // mark the end of the OSC Packet
  msg.empty();      // free space occupied by message

}

void send_x_y_z() {
  OSCMessage msg("/acc");
  msg.add((float)mpu.getAccX());
  msg.add((float)mpu.getAccY());
  msg.add((float)mpu.getAccZ());

  Udp.beginPacket(destIP, destPort);
  msg.send(Udp);    // send the bytes to the SLIP stream
  Udp.endPacket();  // mark the end of the OSC Packet
  msg.empty();      // free space occupied by message
}
