#include "MyServo.h"

MyServo servo(13);

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  servo.init();
}

void loop() {
  while (servo.pos < 180) {
    servo.inc();
    Serial.println(servo.pos);
    delay(15);
  }
  while (servo.pos > 0) {
    servo.dec();
    Serial.println(servo.pos);
    delay(15);
  }
}