#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
void setup()
{

lcd.init();
lcd.init();

lcd.backlight();

lcd.clear();

lcd.setCursor(0,0);


int i =10;
}
void loop()
{
  int i =10;
  for(int n=0; n < i ;i++ ){
    lcd.print("Hello, Hao!");
  }
}