int val = 0;

const char separator = ' ';

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200); // note the speed
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
  // print one number as ASCII charactors
  Serial.print(val);

  // add another value
  // Serial.print(separator);
  // Serial.print(val+20);

  // add a random value
  Serial.print(separator);
  Serial.print(random(256));
  
  // add a newline at the end
  Serial.println(""); 
  delay(500);
  val++;
}
