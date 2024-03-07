#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); // 使用UART1 (或你使用的任何序列端口)
Adafruit_Fingerprint finger(&mySerial);

void setup()  
{
  Serial.begin(115200);
  while (!Serial); // 等待串行端口連接
  Serial.println("\nAdafruit 指紋傳感器測試");

  // 啟動指紋傳感器
  mySerial.begin(57600, SERIAL_8N1, 16, 17); // 請確保這裡的引腳號與你的連接相匹配
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

void loop()                     // 不斷循環等待指紋匹配
{
  getFingerprintIDez();
  delay(50);            // 短暫延遲等待下一次讀取
}

void enrollFingerprint() {
  int id = 1; // 新增指紋的ID，根據需要分配一個唯一的ID
  uint8_t p = -1;

  Serial.println(F("準備新增指紋..."));
  Serial.println(F("請放置手指"));

  // 等待讀取手指
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

  // 轉換圖像
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

  // 轉換圖像並創建模型
  p = finger.image2Tz(2);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("指紋模型創建成功");
  } else {
    Serial.println("錯誤創建指紋模型");
    return;
  }
  
  // 儲存指紋模型
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
