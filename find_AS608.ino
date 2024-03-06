#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(1); 
Adafruit_Fingerprint finger(&mySerial);

void setup()  
{
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("\n\nAdafruit 指紋傳感器測試");


  mySerial.begin(57600, SERIAL_8N1, 16, 17);
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("找到指紋傳感器！");
  } else {
    Serial.println("未找到指紋傳感器。請檢查連接.");
  }
}

void loop() {

}
