#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); 
Adafruit_Fingerprint finger(&mySerial);

unsigned long lastCheckTime = 0; // 用来记录上一次检查时间的变量
const long interval = 1500; // 时间间隔设置为1.5秒
bool failedThisInterval = false; // 标记这个时间段内是否有失败
bool successThisInterval = false; // 标记这个时间段内是否有成功
bool attemptThisInterval = false; // 标记这个时间段内是否有指纹尝试
int failCount = 0; // 记录失败次数的变量

void setup() {
  Serial.begin(115200);
  while (!Serial); // 等待串行端口连接
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

  // 首先检查是否达到1.5秒时间间隔
  if (currentMillis - lastCheckTime >= interval) {
    if (attemptThisInterval) {
      if (!successThisInterval && failedThisInterval) {
        Serial.println("过去1.5秒内至少一次尝试失败。");
        failCount++; // 仅在有尝试并失败时，才增加失败计数
      }
    } else {
      Serial.println("过去1.5秒内没有指纹尝试。");
    }

    Serial.print("当前失败次数：");
    Serial.println(failCount);
    
    // 重置标志位
    attemptThisInterval = false;
    successThisInterval = false;
    failedThisInterval = false;
    lastCheckTime = currentMillis;
  }

  // 尝试进行指纹识别
  getFingerprintIDez();

  delay(50); // 简单延迟以避免过于频繁的处理
}


