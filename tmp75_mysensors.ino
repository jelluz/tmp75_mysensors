
#define CHILD_ID_TEMP 0
#define MY_RADIO_NRF24
#define MY_NODE_ID AUTO
#define MY_DEBUG

#include <SPI.h>
#include <MySensors.h>  
#include <Wire.h>        // Wire header file for I2C and 2 wire
#include <String.h>
#include <stdio.h>
unsigned long SLEEP_TIME = 1000; // Sleep time between reads (in milliseconds)

float lastTemp;
boolean metric = true; 
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);

int TMP75_Address = 0x48; // Apple iMac Temp Sensor Circular PCB
byte configReg = 0x01;  // Address of Configuration Register
byte bitConv = B01100000;   // Set to 12 bit conversion
byte rdWr = 0x01;       // Set to read write
byte rdOnly = 0x00;    // Set to Read
int decPlaces = 1;
int numOfBytes = 2;

void setup()  
{ 

  Wire.begin();                      // Join the I2C bus as a master
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(configReg);                       // Address the Configuration register
  Wire.write(bitConv);                         // Set the temperature resolution
  Wire.endTransmission();                      // Stop transmitting
  Wire.beginTransmission(TMP75_Address);       // Address the TMP75 sensor
  Wire.write(rdOnly);                          // Address the Temperature register
  Wire.endTransmission();                      // Stop transmitting  
//  begin(NULL,5);
}

void presentation()
{ 
  sendSketchInfo("Temperature", "1.0");
  present(CHILD_ID_TEMP, S_TEMP);
}


void loop()      
{  
  

  float temperature = readTemp();
    //  if (isnan(temperature)) {
    //      Serial.println("Failed reading temperature from DHT");
    //  } else if (temperature != lastTemp) {
    //    lastTemp = temperature;
    //    if (!metric) {
    //      temperature = dht.toFahrenheit(temperature);
    //    }
    //    gw.send(msgTemp.set(temperature, 1));
    //    Serial.print("T: ");
    //    Serial.println(temperature);
    //  }
    send(msgTemp.set(temperature, 1));
    Serial.print("T: ");
    Serial.println(temperature);
  //delay(dht.getMinimumSamplingPeriod());

  sleep(SLEEP_TIME); //sleep a bit
}

  // Begin the reading the TMP75 Sensor
float readTemp(){
  // Now take a Temerature Reading
  Wire.requestFrom(TMP75_Address,numOfBytes);  // Address the TMP75 and set number of bytes to receive
  byte MostSigByte = Wire.read();              // Read the first byte this is the MSB
  byte LeastSigByte = Wire.read();             // Now Read the second byte this is the LSB

  // Being a 12 bit integer use 2's compliment for negative temperature values
  int TempSum = (((MostSigByte << 8) | LeastSigByte) >> 4);
  // From Datasheet the TMP75 has a quantisation value of 0.0625 degreesC per bit
  float temp = (TempSum*0.0625);
  //Serial.println(MostSigByte, BIN);   // Uncomment for debug of binary data from Sensor
  //Serial.println(LeastSigByte, BIN);  // Uncomment for debug  of Binary data from Sensor
  return temp;                           // Return the temperature value
}
  



