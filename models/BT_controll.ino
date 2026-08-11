#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;


void BT_controller_init() {

  Serial.begin(115200);

  turn_center();


  SerialBT.begin("ESP32_CAR");

  Serial.println();
  Serial.println("==============================");
  Serial.println("BLUETOOTH CAR READY");
  Serial.println("==============================");

  Serial.println("Bluetooth name: ESP32_CAR");

  Serial.println();
  Serial.println("Commands:");
  Serial.println("F = Forward");
  Serial.println("B = Backward");
  Serial.println("L = Left");
  Serial.println("R = Right");
  Serial.println("S = Stop");
  Serial.println("C = Center steering");
}


void BT_controll() {

  if (SerialBT.available()) {

    char command = SerialBT.read();

    Serial.print("Command: ");
    Serial.println(command);

    switch (command) {

      case 'F':
      case 'f':

        move_forward(base_speed);

        break;

      case 'B':
      case 'b':

        move_backward(base_speed);

        break;

      case 'L':
      case 'l':

        turn_left();

        break;

      case 'R':
      case 'r':

        turn_right();

        break;

      case 'S':
      case 's':

        stop_motor();

        break;

      case 'C':
      case 'c':

        turn_center();

        break;


      default:

        break;
    }
  }
}