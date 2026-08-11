#include <Wire.h>
#include "Adafruit_TCS34725.h"

// TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(
  TCS34725_INTEGRATIONTIME_50MS,
  TCS34725_GAIN_4X
);

void colour_sensor_init()
{
    Serial.println("A: Entering colour_sensor_init()");
    Serial.flush();

    Serial.println("B: Starting I2C");
    Serial.flush();

    Wire.begin(21, 22);
    delay(100);

    Serial.println("C: I2C started");
    Serial.flush();

    Serial.println("D: About to call tcs.begin()");
    Serial.flush();

    bool found = tcs.begin();

    Serial.println("E: tcs.begin() returned");
    Serial.flush();

    if (!found)
    {
        Serial.println("TCS34725 NOT FOUND!");
        return;
    }

    Serial.println("TCS34725 FOUND!");
}

void check_colour() {

  uint16_t r, g, b, c;

  // Read raw RGB + clear
  tcs.getRawData(&r, &g, &b, &c);

  // Avoid division by zero
  if (c == 0) {
    Serial.println("No colour detected");
    delay(200);
    return;
  }

  // Normalize RGB to remove brightness dependency
  float R = (float)r / c;
  float G = (float)g / c;
  float B = (float)b / c;

  Serial.print("R: ");
  Serial.print(R, 3);

  Serial.print("  G: ");
  Serial.print(G, 3);

  Serial.print("  B: ");
  Serial.print(B, 3);

  Serial.print("  C: ");
  Serial.println(c);

  // -------------------------
  // COLOUR DETECTION
  // -------------------------

  if (c < 100) {
    Serial.println("Colour: TOO DARK");
  }

  // BLUE
  else if (B > R * 1.25 && B > G * 1.15) {
    Serial.println("Colour: BLUE");
  }

  // ORANGE
  else if (R > B * 1.5 &&
           R > G * 1.15 &&
           G > B * 1.2) {
    Serial.println("Colour: ORANGE");
  }

  else {
    Serial.println("Colour: UNKNOWN");
  }

  Serial.println("------------------------");

  delay(200);
}