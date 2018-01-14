#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9
#define Button 8

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); //temp sensor
RF24 rf(CE, CSN); //radio module

struct tempHumd {
  float temp;
  float humd;  
} outside;

void setup() {
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  Serial.begin(9600);
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }

  pinMode(Button, INPUT);
}

int pPressed = 0;
bool mode = true; //1 on, off 

void loop() {
  //set struct
  outside.temp = CtoF(htu.readTemperature());
  outside.humd = htu.readHumidity();
  
//  //transmit to arduino
  rf.stopListening();
  int status = rf.write(&outside, 2 * sizeof(float));
  checkButton();
  
  printDetails(status, &outside);
  delay(1000);
}

float CtoF(float celcius) {
  return celcius*1.8 + 32;  
}

void printDetails(int status, struct tempHumd *th) {
  Serial.print("Radio status: "); Serial.println(status);
  Serial.print("Mode: "); Serial.println(mode);
  Serial.println("Outside------------------------");
  Serial.print("Temp: "); Serial.print(th->temp);
  Serial.print("\t\tHum: "); Serial.println(th->humd);
}

void checkButton() {
    if (digitalRead(Button) == HIGH && pPressed == 0) {
    Serial.println("BUTTON");
    pPressed = 1;
    mode = !mode;
  } else if (digitalRead(Button) == LOW) {
    pPressed = 0;  
  }  
}
