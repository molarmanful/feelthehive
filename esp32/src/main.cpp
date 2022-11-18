#include "MyServo.h"

MyServo servos[] = {12, 13, 14, 15, 18, 19};

void setup() {
  Serial.begin(115200);
  WiFiManager wm;
  bool res = wm.autoConnect("FeelTheHive", "feelthehive");
  if (!res) {
    Serial.println("wifi connect failed");
    while (1)
      ;
  }
  for (auto& servo : servos) {
    servo.init();
    Serial.print("servo init: ");
    Serial.println(servo.pin);
  }
}

void loop() {
  // for (auto& servo : servos) servo.servo.write(0);
  while (servos[0].pos < 135) {
    for (auto& servo : servos) servo.inc();
    delay(10);
  }
  while (servos[0].pos > 45) {
    for (auto& servo : servos) servo.dec();
    delay(10);
  }
}