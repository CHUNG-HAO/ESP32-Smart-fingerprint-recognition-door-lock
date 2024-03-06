#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); 
Adafruit_Fingerprint finger(&mySerial);

void setup()  
{
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("\nAdafruit 指紋傳感器測試");


  mySerial.begin(57600, SERIAL_8N1, 16, 17); 
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("指紋傳感器找到並已連接");
  } else {
    Serial.println("指紋傳感器未找到，請檢查連接");
    while (1) { delay(1); }
  }

  Serial.println(F("等待有效指紋..."));
  enrollFingerprint();
}

void loop()                    
  getFingerprintIDez();
  delay(50);          
}

void enrollFingerprint() {
  int id = 1; 
  uint8_t p = -1;

  Serial.println(F("準備新增指紋..."));
  Serial.println(F("請放置手指"));

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("圖像讀取成功");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("通訊錯誤");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("圖像讀取失敗");
        break;
      default:
        Serial.println("未知錯誤");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("圖像轉換成功");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("圖像太亂");
      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("通訊錯誤");
      return;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("無法識別指紋特徵");
      return;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("指紋圖像無效");
      return;
    default:
      Serial.println("未知錯誤");
      return;
  }
  
  Serial.println("請再次放置同一手指");
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p == FINGERPRINT_OK) {
      Serial.println("圖像讀取成功");
    } else if (p == FINGERPRINT_NOFINGER) {
      Serial.print(".");
    }
  }


  p = finger.image2Tz(2);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("指紋模型創建成功");
  } else {
    Serial.println("錯誤創建指紋模型");
    return;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("指紋儲存成功");
  } else {
    Serial.println("錯誤儲存指紋");
  }
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // 找到匹配
  Serial.print("找到指紋 ID #"); Serial.print(finger.fingerID); 
  Serial.print(" 與置信度 "); Serial.println(finger.confidence);
  return finger.fingerID;
}
