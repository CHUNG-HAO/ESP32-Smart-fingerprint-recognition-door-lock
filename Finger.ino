void enrollFingerprint() {
  int id = 1; // 新增指紋的ID
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
  return true; 
}

bool convertImage(int slot) {
  return finger.image2Tz(slot) == FINGERPRINT_OK;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_OK) {
  
    attemptThisInterval = true; 
    if (finger.image2Tz() == FINGERPRINT_OK && finger.fingerFastSearch() == FINGERPRINT_OK) {
      Serial.print("找到指纹 ID #"); Serial.print(finger.fingerID);
      Serial.print(" 与置信度 "); Serial.println(finger.confidence);
      successThisInterval = true; 
      return finger.fingerID;
    } else {
      failedThisInterval = true;
      return -1;
    }
  } else if (p == FINGERPRINT_NOFINGER) {
    
    return -1;
  } else {
  
    attemptThisInterval = true;
    failedThisInterval = true;
    return -1;
  }
  return -1;
}