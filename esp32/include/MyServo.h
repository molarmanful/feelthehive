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

  void setPos(int n) {
    pos = max(min(n, 180), 0);
    servo.write(pos);
  }

  void inc(int n = 1) { setPos(pos + n); }

  void dec(int n = 1) { setPos(pos - n); }
};