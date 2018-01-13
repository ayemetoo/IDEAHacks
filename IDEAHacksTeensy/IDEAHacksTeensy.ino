#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); //temp sensor
//RF24 rf(CE, CSN); //radio module

struct tempHumd {
  float temp;
  float humd;  
} outside;

void setup() {
//  rf.begin();
//  rf.setChannel(5);
//  rf.setPALevel(RF24_PA_MIN);
//  rf.openWritingPipe(0xe7e7e7e7e7);
//  rf.openReadingPipe(1, 0xc2c2c2c2c2);
//  rf.setCRCLength(RF24_CRC_16);
  
  Serial.begin(9600);
  Serial.println("HTU21D-F test");
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}


void loop() {
  //set struct
  outside.temp = CtoF(htu.readTemperature());
  outside.humd = htu.readHumidity();
  
//  //transmit to arduino
//  rf.stopListening();
//  int status = rf.write(&outside, 2 * sizeof(float));
//  Serial.println(status);
  
  Serial.println("Outside--------");
  Serial.print("Temp: "); Serial.print(outside.temp);
  Serial.print("\t\tHum: "); Serial.println(outside.humd);
  delay(3000);
}

float CtoF(float celcius) {
  return celcius*1.8 + 32;  
}
