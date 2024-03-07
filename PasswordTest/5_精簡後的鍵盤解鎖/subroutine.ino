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
  myservo.write(0);  //旋轉到0度，就是一般所說的歸零
  delay(10);
}

void BE_BE(int times){
  for(int i = 0; i < times; i++){
    digitalWrite(15, LOW);
    delay(500);
    digitalWrite(15,HIGH);
    delay(500);
  }
}

void State_Check(){
  if(Lock_State == 1){
    myservo.write(SERVO_ANGLE);   //旋轉到指定角度
    delay(10);
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

void BLINK(){
  while(true){
    LED_MODE(2);
    delay(100);
    LED_MODE(4);
    delay(100);
    }
}

void LED_STATE(){
  if(Lock_State == 1) LED_MODE(1);
  else if(Lock_State == 0)  LED_MODE(2);  //常亮紅色
  else if(Lock_State == 3)  LED_MODE(3);
}