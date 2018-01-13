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
} inside, outside, pInside, pOutside;

int target = 70;

void setup() {
//  rf.setChannel(5);
//  rf.setPALevel(RF24_PA_MIN);
//  rf.openWritingPipe(0xc2c2c2c2c2); //what do i put here
//  rf.openReadingPipe(1, 0xe7e7e7e7e7);
//  rf.setCRCLength(RF24_CRC_16);
  
  Serial.begin(9600);
  Serial.println("ss");
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}


void loop() {
  Serial.println("aa");
  //set struct
  inside.temp = CtoF(htu.readTemperature());
  inside.humd = htu.readHumidity();
  
  //recieve from arduino
//  rf.startListening();
//  if (rf.available()) {
//    rf.read(&outside, sizeof(float)*2);  
//  }
  
  Serial.println("Inside--------");
  Serial.print("Temp: "); Serial.print(inside.temp);
  Serial.print("\t\tHum: "); Serial.println(inside.humd);

  Serial.println("Outside--------");
  Serial.print("Temp: "); Serial.print(outside.temp);
  Serial.print("\t\tHum: "); Serial.println(outside.humd);
  delay(3000);
}

float CtoF(float celcius) {
  return celcius*1.8 + 32;  
}

//bool checkTemp(float inside, float outside) {
//  float tempDiff, tempDiffIn, tempDiffOut;
//  tempDiff = outside - inside;
//  tempDiffIn = inside - target; //pos = hotter, neg = cooler
//  tempDiffOut = outside - target; //pos = hotter, neg = cooler
//  //diff bigger than 2 degrees
//  if (tempDiff > 2 || tempDiff < -2) {
//      //if tempDiffIn > 0 && tempDiffOut < 0 --> open
//      //if tempDiffIn > 0 && tempDiffOut > 0
//        //if inside more humid --> open
//        //else --> close
//      //if tempDiffIn < 0 && tempDiffOut < 0 --> close? or open if outside is more humid?
//      //if tempDiffIn < 0 && tempDiffOut > 0 --> open
//  }
//}

//void open --> open window
//void close --> close window
