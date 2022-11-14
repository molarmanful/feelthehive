#include "MyServo.h"

MyServo servos[] = {12, 13, 14, 15};

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  for (auto& servo : servos) {
    servo.init();
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