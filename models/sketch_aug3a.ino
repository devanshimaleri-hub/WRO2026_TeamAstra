
#define base_speed 150

void setup()
{
    Serial.begin(115200);
    delay(1000);

    motor_init();

    servo_init();

    turn_center();
}

void loop()
{
  if (Serial.available())
  {
   
    char cmd = Serial.read();

    switch (cmd)
    {
      case 'l':
    
        turn_left();
        break;

      case 'r':
      
        turn_right();
        break;

      case 'f':
      turn_center();
      move_forward(base_speed);
        break;

      case 's':
      stop_motor();
      turn_center();
        break;

      case 'z':
      stop_motor();
      turn_center();
        break;

      case 'x':
      stop_motor();
      turn_center();
        break;

      case 'b':
      turn_center();
      move_backward(base_speed);
        break;
    }
  }

  
}

