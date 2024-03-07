void LED_COLOR(int mode){  //設定每個燈珠的顏色
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

void LOCK_IT(){   //上鎖
  LED_COLOR(2);
  Lock_State = 0;
  K = "";
  myservo.write(0);  //旋轉到0度，就是一般所說的歸零
}

void BE_BE(int times){  //蜂鳴器叫0.5s
  for(int i = 0; i < times; i++){
    digitalWrite(15, LOW);
    delay(500);
    digitalWrite(15,HIGH);
    delay(500);
  }
}

void State_Check(){ //解鎖->鎖定/改密碼；改密碼
  if(Lock_State == 1){
    LED_COLOR(1);
    myservo.write(SERVO_ANGLE);   //旋轉到指定角度
    K = "";
    failCount = 0;
    if(Num == "C")  LOCK_IT();
    if(Num == "D")  Lock_State = 3;
    if(Num == "B")  Lock_State = 4;
  }
  else if(Lock_State == 3){
    LED_COLOR(3);
    if(Num == "#"){
      PW = K;
      K = "";
      LOCK_IT();
    }
  }
  else if(Lock_State == 4){
    LED_COLOR(3);
    Serial.println(F("等待有效指紋..."));
    enrollFingerprint();
    LOCK_IT();
  }
}

void BLINK(){ //LED閃紅燈
  while(true){
    LED_COLOR(2);
    delay(100);
    LED_COLOR(4);
    delay(100);
    }
}

//指紋辨識用
void Reset_FingerprintTemporaryParameters(){  //清除暫存
  attemptThisInterval = false;
  successThisInterval = false;
  failedThisInterval = false;
  lastCheckTime = currentMillis;
}

void enrollFingerprint() {  //增加指紋
  id++; //id+1
  if(id > Fingerprint_Limit)  id = 1; //如果收錄的指紋大於上限，從頭開始使用編號
  
  if (enroll(id)) {
    Serial.println("指紋儲存成功");
  } else {
    Serial.println("錯誤儲存指紋");
  }
}

bool enroll(int id) {
  if (takeImage() && convertImage(1) && takeImage() && convertImage(2)) {
    if (finger.createModel() == FINGERPRINT_OK) {
      return finger.storeModel(id) == FINGERPRINT_OK;
    }
  }
  return false;
}

bool takeImage() {
  while (finger.getImage() != FINGERPRINT_OK);
  return true; // 仅当getImage返回FINGERPRINT_OK时
}

bool convertImage(int slot) {
  return finger.image2Tz(slot) == FINGERPRINT_OK;
}

int getFingerprintIDez() {  //檢測是否成功（正確指紋）
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK) {
    // 检测到手指
    attemptThisInterval = true; // 确认有指纹尝试
    if (finger.image2Tz() == FINGERPRINT_OK && finger.fingerFastSearch() == FINGERPRINT_OK) {
      Serial.print("找到指纹 ID #"); Serial.print(finger.fingerID);
      Serial.print(" 与置信度 "); Serial.println(finger.confidence);
      successThisInterval = true; // 标记成功
      return finger.fingerID;
    } else {
      failedThisInterval = true; // 后续流程失败，标记失败
      return -1;
    }
  } else if (p == FINGERPRINT_NOFINGER) {
    // 没有检测到手指，不更新失败或尝试标志
    return -1;
  } else {
    // 检测到手指但获取图像失败，视为尝试
    attemptThisInterval = true;
    failedThisInterval = true;
    return -1;
  }
  return -1;
}

uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}