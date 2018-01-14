#include <Wire.h>
#include <Servo.h>
#include "Adafruit_HTU21DF.h"
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define CSN 10
#define CE 9

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); //temp sensor
RF24 rf(CE, CSN); //radio module
Servo myservo; //servo

struct tempHumd {
  float temp;
  float humd;  
} inside, outside, pInside, pOutside;

int target = 70;
int opened = 0;
int targethumd = 20;

void setup() {
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openWritingPipe(0xc2c2c2c2c2); //what do i put here
  rf.openReadingPipe(1, 0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  
  Serial.begin(9600);
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
  myservo.attach(16);
}


void loop() {
  //set struct
  inside.temp = CtoF(htu.readTemperature());
  inside.humd = htu.readHumidity();
  
  //recieve from arduino
  rf.startListening();
  if (rf.available()) {
    rf.read(&outside, sizeof(float)*2);  
  }
  
  Serial.println("Inside--------");
  Serial.print("Temp: "); Serial.print(inside.temp);
  Serial.print("\t\tHum: "); Serial.println(inside.humd);

  Serial.println("Outside--------");
  Serial.print("Temp: "); Serial.print(outside.temp);
  Serial.print("\t\tHum: "); Serial.println(outside.humd);
  checkTemp();
  delay(1000);
}

float CtoF(float celcius) {
  return celcius*1.8 + 32;  
}

void checkTemp() {
  float tempDiff, tempDiffIn, tempDiffOut;
  tempDiff = outside.temp - inside.temp;
  tempDiffIn = inside.temp - target; //pos = hotter, neg = cooler
  tempDiffOut = outside.temp - target; //pos = hotter, neg = cooler
  humdDiff = outside.humd - inside.humd;
  humdDiffIn = inside.humd - targethumd; 
  humdDiffOut = outside.humd - targethumd;

//  diffSumIn = tempDiffIn+0.1*humdDiffIn;
//  diffSumOut = tempDiffOut+0.1*humdDiffOut;
  //diff bigger than 2 degrees
  if (abs(tempDiff) > 2) {
      if(abs(tempDiffIn) > abs(tempDiffOut)){
        openwindow();
      }
      else{
        closewindow();
      }
  } else if(abs(humdDiff) > 20){
    if(abs(humdDiffIn) > abs(humdDiffOut)){
        openwindow();
    }else{
        closewindow();
    }
  }
  // maybe weight both temp and humd to make a decision?
}

void test(){
  
  total_err = humd_err + temp_err;
}

float heatIndex(float temp, float humd){
  return -42.379 + 2.04901523*temp + 10.14333127*humd + -0.22477541*temp*humd
  + -0.00683783*temp*temp + -0.05481717*humd*humd;
}

void openwindow(){
  if(!opened){
    myservo.write(70);
    delay(500);
    myservo.write(90);
  }
  Serial.println("opened");
  opened = 1;
}
void closewindow(){
  if(opened){
    myservo.write(110);
    delay(500);
    myservo.write(90);
  }
  Serial.println("closed");
  opened = 0;
}
