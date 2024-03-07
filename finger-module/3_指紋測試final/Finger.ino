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
  return true; // 仅当getImage返回FINGERPRINT_OK时
}

bool convertImage(int slot) {
  return finger.image2Tz(slot) == FINGERPRINT_OK;
}

int getFingerprintIDez() {
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