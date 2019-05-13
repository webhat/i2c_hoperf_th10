/*
 * I2C HopeRF TH10 measurement reader
 * 
 * Uses code from:
 * I2C SRF10 or SRF08 Devantech Ultrasonic Ranger Finder 
 * by Nicholas Zambetti <http://www.zambetti.com> 
 * and James Tichenor <http://www.jamestichenor.net>
 * 
 * This example code is in the public domain.
 */

#include <Wire.h>

void setup() {
  Wire.begin();                // join i2c bus (address optional for master)

  
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  
  Serial.begin(9600);          // start serial communication at 9600bps
  Serial.write("i2c test TH10 v1.0\n");
}

long reading = 0;

void loop() {
  Serial.println("Start Loop");

  int address = 0x44;  // transmit to device (0x00 or 0x88)
  // step 1: instruct sensor to start measurement
  Wire.beginTransmission(address);
  Wire.write(byte(0x24));      // sets register pointer to the command register (0x00)
  Wire.write(byte(0x16));
  Wire.endTransmission();      // stop transmitting
  Serial.println("Transmission Sent");

  // step 2: wait for readings to happen
  delay(1000);                   // datasheet suggests at least 65 milliseconds

  int bytes_receive = 6;

  // step 3: request reading from sensor
  Wire.requestFrom(0x44, bytes_receive);    // request 6 bytes from device
  Serial.println("Read data");
  int bytes_received = Wire.available();

  /*
  Serial.print("b: ");
  Serial.print(bytes_received);
  Serial.print("/");
  Serial.print(bytes_receive);
  Serial.println();
  */

  // step 5: receive reading from sensor
  if (bytes_receive <= bytes_received) { // if two bytes were received
    // Temperature
    reading = Wire.read();  // receive high byte (overwrites previous )
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read(); // receive low byte as lower 8 bits
    
    Serial.println(-45+(127*(reading/((2^16)-1))));   // print the reading

    // Swallow CRC
    reading = Wire.read();

    // Humidity
    reading = Wire.read();  // receive high byte (overwrites previous )
    reading = reading << 8;    // shift high byte to be high 8 bits
    reading |= Wire.read(); // receive low byte as lower 8 bits
    
    Serial.println(100*(reading/((2^16)-1)));   // print the reading    
  }
  Serial.println("End Read");

  delay(250);                  // wait a bit since people have to read the output :)
}
