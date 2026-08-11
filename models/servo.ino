#include <ESP32Servo.h>

Servo steering;

const int servo_pin = 26;

const int center = 87;      
const int max_left = 57;   

const int max_right = 110;  


void servo_init()
{
  steering.attach(servo_pin);

  steering.write(center);
  delay(100);
}


void turn_left()
{
  steering.write(max_left);
}

void turn_right()
{
  steering.write(max_right);
}

void turn_center()
{
  steering.write(center);
}