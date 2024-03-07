//LIB
#include "Adafruit_Keypad.h"
#include <FastLED.h>

//密碼
String PW = "0000";
String N_PW = "";

//鍵盤設定
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; //定義列的腳位
byte colPins[COLS] = {27, 14, 12, 13}; //定義行的腳位
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//狀態
int Lock_State = 0; //LOCK:0; UNLOCK:1; CHANGE_PW:3
int ERROR = 0;
String K = "";
String Num = "";

//LED
#define NUM_LEDS 8  //LED數量
#define DATA_PIN 2
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  Serial.println("Program Testing...");

  //鍵盤設定
  customKeypad.begin();

  //LED模組設定
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  customKeypad.tick();  // 開始偵測使用者的按鍵狀態

  if(ERROR < 3){
    while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    Num = (char)e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED){
      if(Num != "*"){
        K = K + Num;
      }
      else if(Num == "*"){  //密碼檢查
        if(K == PW){     //正確亮綠色0.5s並清空
          Lock_State = 1;
        }
        else if(K != PW){
          ERROR += 1;
          K = "";
          if(ERROR == 1 || ERROR == 2 || ERROR == 3) BE_BE(ERROR);
        }      
      }

      if(Lock_State == 1){
        K = "";
        if(Num == "D")  Lock_State = 3;
        if(Num == "A")  LOCK_IT();
      }
      else if(Lock_State == 3){
        if(Num != "#"){
          N_PW = N_PW + Num;
        }
        else if(Num == "#"){
          PW = N_PW;
          N_PW = "";
          LOCK_IT();
        }
      }
    } 
    
    if(Lock_State == 1) LED_MODE(1);
    else if(Lock_State == 0)  LED_MODE(2);  //常亮紅色
    else if(Lock_State == 3)  LED_MODE(3);

    if(Num == "C") K = "";  //清空鍵盤

    Serial.println(Num);
    Serial.println(K);
    Serial.println(ERROR);
    }
  }
  else{
    while(true){
      LED_MODE(2);
      delay(100);
      LED_MODE(4);
      delay(100);
    }
  }
  
  delay(10);
}

void LED_MODE(int mode){
  if(mode == 1){
    for(int i=0;i<8;i++){
    leds[i] = CRGB::Green;
    FastLED.show();
    }
  }
  else if(mode == 2){
    for(int i=0;i<8;i++){
    leds[i] = CRGB::Red;
    FastLED.show();
    }
  }
  else if(mode == 3){
    for(int i=0;i<8;i++){
    leds[i] = CRGB::Blue;
    FastLED.show();
    }
  }
  else if(mode == 4){
    for(int i=0;i<8;i++){
    leds[i] = CRGB::Black;
    FastLED.show();
    }
  }
}

void LOCK_IT(){
  Lock_State = 0;
  K = "";
}

void BE_BE(int times){
  for(int i = 0; i < times; i++){
    digitalWrite(15, LOW);
    delay(500);
    digitalWrite(15,HIGH);
    delay(500);
  }
}