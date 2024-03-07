#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); // 假設你使用GPIO16作為RX, GPIO17作為TX
Adafruit_Fingerprint finger(&mySerial);

void setup()  
{
  Serial.begin(115200);
  while (!Serial); // 等待串行控制台開啟
  Serial.println("\n\nAdafruit 指紋傳感器測試");

  // 啟動指紋傳感器
  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("找到指紋傳感器！");
  } else {
    Serial.println("未找到指紋傳感器。請檢查連接.");
  }
}

void loop() {
  // 這裡不需要重複執行任何代碼
}
