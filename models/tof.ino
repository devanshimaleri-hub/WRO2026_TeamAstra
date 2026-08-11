// #include <Wire.h>
// #include <VL53L1X.h>

// // -------- CHANGE THESE TO YOUR GPIO PINS --------
// #define SDA_PIN       21    
// #define SCL_PIN       22    

// #define XSHUT_LEFT    13
// #define XSHUT_FRONT   14
// #define XSHUT_RIGHT   16
// #define XSHUT_BACK   17
// // ------------------------------------------------

// VL53L1X leftSensor;
// VL53L1X frontSensor;
// VL53L1X rightSensor;
// VL53L1X backSensor;

// uint16_t left_distance = 8190;
// uint16_t front_distance = 8190;
// uint16_t right_distance = 8190;
// uint16_t back_distance = 8190;

// void tof_init()
// {
//   Serial.begin(115200);

//   // Pico (Earle Philhower core)
//   Wire.setSDA(SDA_PIN);
//   Wire.setSCL(SCL_PIN);
//   Wire.begin();

//   pinMode(XSHUT_LEFT, OUTPUT);
//   pinMode(XSHUT_FRONT, OUTPUT);
//   pinMode(XSHUT_RIGHT, OUTPUT);
//   pinMode(XSHUT_BACK, OUTPUT);

//   // Turn off all sensors
//   digitalWrite(XSHUT_LEFT, LOW);
//   digitalWrite(XSHUT_FRONT, LOW);
//   digitalWrite(XSHUT_RIGHT, LOW);
//   digitalWrite(XSHUT_BACK, LOW);
//   delay(50);

//   // ---------------- LEFT ----------------
//   digitalWrite(XSHUT_LEFT, HIGH);
//   delay(50);

//   if (!leftSensor.init())
//   {
//     Serial.println("Left sensor failed");
//     while (1);
//   }

//   leftSensor.setAddress(0x30);
//   leftSensor.setTimeout(50);
//   leftSensor.setDistanceMode(VL53L1X::Short);
//   leftSensor.setMeasurementTimingBudget(20000);
//   leftSensor.startContinuous(20);

//   // ---------------- FRONT ----------------
//   digitalWrite(XSHUT_FRONT, HIGH);
//   delay(50);

//   if (!frontSensor.init())
//   {
//     Serial.println("Front sensor failed");
//     while (1);
//   }

//   frontSensor.setAddress(0x31);
//   frontSensor.setTimeout(50);
//   frontSensor.setDistanceMode(VL53L1X::Short);
//   frontSensor.setMeasurementTimingBudget(20000);
//   frontSensor.startContinuous(20);

//   // ---------------- RIGHT ----------------
//   digitalWrite(XSHUT_RIGHT, HIGH);
//   delay(50);

//   if (!rightSensor.init())
//   {
//     Serial.println("Right sensor failed");
//     while (1);
//   }

//   rightSensor.setAddress(0x32);
//   rightSensor.setTimeout(50);
//   rightSensor.setDistanceMode(VL53L1X::Short);
//   rightSensor.setMeasurementTimingBudget(20000);
//   rightSensor.startContinuous(20);

//     // ---------------- BACK ----------------
//   digitalWrite(XSHUT_BACK, HIGH);
//   delay(50);

//   if (!backSensor.init())
//   {
//     Serial.println("Back sensor failed");
//     while (1);
//   }

//   frontSensor.setAddress(0x33);
//   frontSensor.setTimeout(50);
//   frontSensor.setDistanceMode(VL53LoX::Short);
//   frontSensor.setMeasurementTimingBudget(20000);
//   frontSensor.startContinuous(20);

//   Serial.println("All VL53LoX sensors initialized!");
// }

// void check_distances()
// {
//   left_distance = leftSensor.read();
//   front_distance = frontSensor.read();
//   right_distance = rightSensor.read();
//   back_distance = backSensor.read();

//   if (leftSensor.timeoutOccurred() ||
//       leftSensor.ranging_data.range_status != VL53L1X::RangeValid)
//     left_distance = 8190;

//   if (frontSensor.timeoutOccurred() ||
//       frontSensor.ranging_data.range_status != VL53L1X::RangeValid)
//     front_distance = 8190;

//   if (rightSensor.timeoutOccurred() ||
//       rightSensor.ranging_data.range_status != VL53L1X::RangeValid)
//     right_distance = 8190;

//       if (backSensor.timeoutOccurred() ||
//       rightSensor.ranging_data.range_status != VL53LoX::RangeValid)
//     right_distance = 8190;
// }


