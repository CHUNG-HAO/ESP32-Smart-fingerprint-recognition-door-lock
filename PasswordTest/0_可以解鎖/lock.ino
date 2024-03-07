#include "Adafruit_Keypad.h"

const byte ROWS = 4; // 列數(橫的)
const byte COLS = 4; // 行數(直的)
//鍵盤上每一個按鍵的名稱
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; //定義列的腳位
byte colPins[COLS] = {27, 14, 12, 13}; //定義行的腳位

//初始化鍵盤
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String PW = "0000";
String K = "";
String Num = "";

void setup() {
  Serial.begin(9600);
  customKeypad.begin();
  Serial.println("KeyPad Test...");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // 開始偵測使用者的按鍵狀態
  customKeypad.tick();

  //判斷按了哪一個鍵
  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    //Serial.print((char)e.bit.KEY);
    Num = (char)e.bit.KEY;
    //if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed"); 
    if(e.bit.EVENT == KEY_JUST_PRESSED) K = K + Num; 
    //按下的狀態是KEY_JUST_PRESSED
    //else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");  
    //放開的狀態是KEY_JUST_RELEASED
    
    if(K == PW){
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(10);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);
    }

    if(Num == "D") K = "";
    Serial.println(K);
  }

  delay(10);
}