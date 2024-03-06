#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); 
Adafruit_Fingerprint finger(&mySerial);

unsigned long lastCheckTime = 0;
const long interval = 1500; 
bool failedThisInterval = false; 
bool successThisInterval = false; 
bool attemptThisInterval = false; 
int failCount = 0; 

void setup() {
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

void loop() {
  unsigned long currentMillis = millis();


  if (currentMillis - lastCheckTime >= interval) {
    if (attemptThisInterval) {
      if (!successThisInterval && failedThisInterval) {
        Serial.println("过去1.5秒内至少一次尝试失败。");
        failCount++; 
      }
    } else {
      Serial.println("过去1.5秒内没有指纹尝试。");
    }

    Serial.print("当前失败次数：");
    Serial.println(failCount);
    

    attemptThisInterval = false;
    successThisInterval = false;
    failedThisInterval = false;
    lastCheckTime = currentMillis;
  }


  getFingerprintIDez();

  delay(50); 
