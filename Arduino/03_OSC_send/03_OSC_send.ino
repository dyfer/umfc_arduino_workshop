/*
  Based on UDP send example from the ESP8266 repository
  Expanded from OSC example UDPSendMessage
  created 21 Aug 2010
  by Michael Margolis

  This code is in the public domain.

  adapted from Ethernet library examples
*/

// needs the 8266 board definition from https://learn.adafruit.com/adafruit-io-basics-esp8266-arduino/using-arduino-ide


#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>


#ifndef STASSID
#define STASSID "<network-name>"
#define STAPSK "<password>"
#endif

unsigned int localPort = 8888;  // local port to listen on
// unsigned int destPort; // we'll get that from the message sent at first
unsigned int destPort = 9000; // we'll get that from the message sent at first
IPAddress destIP;

// 
int val = 0;
int randVal;
const char separator = ' ';

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
// char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

WiFiUDP Udp;

void setup() {
  Serial.begin(115200);
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
}

void loop() {
  // if there's data available, read a packet
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

  // if we get the port, then let's start sending
  if(destIP) {
    randVal = random(256);

    OSCMessage msg("/val");
    msg.add((int32_t)val);
    msg.add((int32_t)randVal);
    
    Udp.beginPacket(destIP, destPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    
    // print to serial for comparison
    Serial.print(val);  
    Serial.print(separator);
    Serial.print(random(256));
    Serial.println(""); 
    // Serial.println(destIP.toString().c_str());
    // Serial.println(destPort);

    val++;  // increment the value

    delay(500);
  }
  
  
}

/*
  test (shell/netcat):
  --------------------
    nc -u 192.168.esp.address 8888
*/
