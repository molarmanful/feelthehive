#include "deps.h"

class MyServo {
 public:
  Servo servo;
  int pin;
  int pos = 0;

  MyServo(int p) { pin = p; }

  void init() {
    servo.setPeriodHertz(50);
    servo.attach(pin);
    servo.write(0);
  }

  void inc(int n = 1) {
    pos = min(pos + n, 180);
    servo.write(pos);
  }

  void dec(int n = 1) {
    pos = max(pos - n, 0);
    servo.write(pos);
  }
};