//LIB
#include "Adafruit_Keypad.h" //Adafruit_Keypad
#include <FastLED.h>  //FastLED
#include <Servo.h>  //ServoESP32

//視需求更改的變數
String PW = "0000";       //密碼（預設）
#define SERVO_ANGLE 90    //馬達打開角度（0~90）
#define PW_ERROR_TIMES  3 //可以輸錯密碼的次數

//腳位大集合
#define DATA_PIN 2                      //LED腳位（DIN）
#define SERVO_PIN 5                     //Servo腳位（S）
#define BUZZER_PIN 15                   //蜂鳴器控制腳位
//鍵盤腳位請於下面更改

//以上為可作更改的變數，下面的變數請不要更動------------------------------------------------

//鍵盤設定
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26};  //鍵盤列的腳位，只有這裡可以更改！
byte colPins[COLS] = {27, 14, 12, 13};  //鍵盤行的腳位，只有這裡可以更改！
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//密碼（暫存）
String N_PW = "";

int Lock_State = 0; //鎖定的狀態  LOCK:0; UNLOCK:1; CHANGE_PW:3
int ERROR_TIMES = 0;      //密碼錯誤的次數（預設0）
String K = "";      //鍵盤輸入值
String Num = "";    //當下按下什麼按鍵

#define NUM_LEDS 8  //LED數量
CRGB leds[NUM_LEDS];

Servo myservo;  //SERVO

//--------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println("Program Testing...");

  //鍵盤設定
  customKeypad.begin();

  //LED模組設定
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH); //蜂鳴器控制腳HIGH，聲音關閉
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  LED_MODE(2);  //設定LED為紅色

  //SERVO
  myservo.attach(SERVO_PIN);
  myservo.write(0);
}

void loop() {
  customKeypad.tick();  //開始偵測使用者的按鍵狀態

  if(ERROR_TIMES < PW_ERROR_TIMES){ //如果密碼錯誤的次數少於設定的次數
    //指紋組程式



    //鍵盤組程式-------------------------------------------------------------------------  
    keypadEvent e = customKeypad.read();
    Num = (char)e.bit.KEY;  //暫存現在按下的按鍵
    if(e.bit.EVENT == KEY_JUST_PRESSED){
      if(Num != "*")  K = K + Num;  //如果按下的按鍵不是"*"，就存入字串中
      else if(Num == "*"){  //密碼檢查
        if(K == PW) Lock_State = 1; //正確亮綠色0.5s並清空
        else if(K != PW){ //如果錯誤
          ERROR_TIMES += 1; //錯誤次數+1
          K = ""; //清空字串
          BE_BE(ERROR_TIMES); //蜂鳴器叫錯誤的次數
        }
      }
      State_Check();  //解鎖＆切換至設定密碼模式
    } 
    LED_STATE();            //LED根據鎖定狀態顯示不同顏色
    if(Num == "C") K = "";  //清空鍵盤
  }
  else  BLINK();  //超過設定的密碼錯誤次數會關閉，並且開始閃爍
  delay(10);
}