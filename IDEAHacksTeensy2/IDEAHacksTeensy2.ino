#include <Wire.h>
#include <Servo.h>
#include "Adafruit_HTU21DF.h"
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Keypad.h"
#include "artwork.h"

#define CSN 10
#define CE 9

// If using software SPI (the default case):
#define OLED_MOSI   7
#define OLED_CLK   6
#define OLED_DC    0
#define OLED_CS    1
#define OLED_RESET 20
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

//button matrix
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {23, 22, 21}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define Button 8

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); //temp sensor
RF24 rf(CE, CSN); //radio module
Servo myservo; //servo

struct tempHumd {
  float temp;
  float humd;
  int hi;
} inside, outside, pInside, pOutside;

int target = 70;
int opened = 0;
int targethumd = 20;
int pPressed = 1, auto_mode = 1;

void setup() {

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  
  rf.begin();
  rf.setChannel(5);
  rf.setPALevel(RF24_PA_MIN);
  rf.openReadingPipe(1, 0xe7e7e7e7e7);
  rf.setCRCLength(RF24_CRC_16);
  
  pinMode(Button, INPUT);
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
    inside.temp = heatIndex(&inside);
    outside.temp = heatIndex(&outside);
    
    Serial.println("Inside--------");
    Serial.print("Temp: "); Serial.print(inside.temp);
    Serial.print("\t\tHum: "); Serial.println(inside.humd);
  
    Serial.println("Outside--------");
    Serial.print("Temp: "); Serial.print(outside.temp);
    Serial.print("\t\tHum: "); Serial.println(outside.humd);
    if(auto_mode)
      checkTemp();
    
    showReadings();
  }
  char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    switch (key)
    {
      case '*':
        changeVal(0);
        break;
      case '#':
        changeVal(1);
        break;
      default:
        break;
    }
  }
  delay(100);
}

float CtoF(float celcius) {
  return celcius*1.8 + 32;  
}

void checkTemp() {
  float tempDiff, tempDiffIn, tempDiffOut;
  float humdDiff, humdDiffIn, humdDiffOut;
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

// Approximation Formula: https://en.wikipedia.org/wiki/Heat_index
float heatIndex(struct tempHumd *th){
  float temp = th->temp;
  float humd = th->humd;
  if (temp > 80 && humd > 40) {
    th->hi = 1;
    return -42.379 + 2.04901523*temp + 10.14333127*humd + -0.22477541*temp*humd
    + -0.00683783*temp*temp + -0.05481717*humd*humd + 0.00122874*temp*temp*humd
    + 0.00085282*temp*humd*humd + -0.00000199*temp*temp*humd*humd;
  } else {
    th->hi = 0;
    return temp;
  }
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

//prints a line of values "TTT°F HHH%"
void printValues(struct tempHumd *th){
  String line = String((int)th->temp);
  while(line.length()<3)
    line = " "+line;
  line += (char)247;
  line += "F";
  if(th->hi){
    line += "*";
  }else{
    line += " ";
  }
  String h = String((int) th->humd);
  while(h.length()<3)
    h = " "+h;
  line += h;
  line += "%\n";
  
  display.print(line);
}

void showReadings(){
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  
  display.println("Inside:");
  printValues(&inside);
  display.println("Outside:");
  printValues(&outside);
  
  display.display();
}

void checkButton() {
    if (digitalRead(Button) == HIGH && pPressed == 0) {
    Serial.println("BUTTON");
    pPressed = 1;
    auto_mode = !auto_mode;
  } else if (digitalRead(Button) == LOW) {
    pPressed = 0;  
  }
}

//i = 0 for temp, i = 1 for humd
void changeVal(int id){
  int loop = 1;
  int count = 0;
  String newval = "";
  while(loop){
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    if(id == 0)
      display.println("New Temp:");
    else
      display.println("New Humd:");
    display.println(newval);
    display.display();
    char key = kpd.getKey();
    if(key)  // Check for a valid key.
    {
      switch (key)
      {
        case '*':
          loop = 0;
          break;
        case '#':
          count--;
          newval.remove(count);
          break;
        default:
          if(count<3){
            newval += key;
            count++;
          }
          break;
      }
    }
    delay(100);
  }
  int conv = newval.toInt();
  if(count != 0){
    if(id == 0)
      target = conv;
    else if(conv <= 100)
      targethumd = conv;
    else
      showArt();
  }
}

void showArt(){
  display.clearDisplay();
  display.drawBitmap(0, 0, bestArtEver, 128, 64, WHITE);
  display.display();
  delay(500);
}

