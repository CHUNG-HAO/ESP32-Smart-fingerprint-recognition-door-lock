#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// 0x27是LCD模組的地址，16表示有十六欄位，2表示總共有2欄
LiquidCrystal_I2C lcd(0x27,20,4);
void setup()
{
//LCD初始化
lcd.init();
lcd.init();
//開啟背光
lcd.backlight();
//清除先前畫面
lcd.clear();
//設定第一個字顯示座標
lcd.setCursor(0,0);
//顯示字串

int i =10;
}
void loop()
{
  int i =10;
  for(int n=0; n < i ;i++ ){
    lcd.print("Hello, Hao!");
  }
}