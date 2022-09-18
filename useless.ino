#include <Servo.h>


Servo test_servo;


void setup()
{
  Serial.begin(115200);
  Serial.print("Servo test started");
  test_servo.attach(9);           //set door servo on Pin 9 pwm
  // test_servo.writeMicroseconds(1600);
  test_servo.write(0);

}


void loop()
{
  static uint32_t last_time = 0;
  uint32_t now = millis();
  if (now > last_time + 100)
  {
    last_time = now;
    static int dir = 0;
    dir = (dir + 1) % 2;
    Serial.print("dir=");
    Serial.println(dir);
    test_servo.write(dir ? 180 : 0);

  }
}
