#include <ArduinoGraphics.h>
#include <Arduino_MKRRGB.h>
#include <Stream.h>

#define let auto const
using namespace std;

int pwmValue = 48;   // Global brightness (0 - 255)
int r = 22;          // Red component
int g = 22;          // Green component
int b = 22;          // Blue component

void setup() {
  MATRIX.begin();
  MATRIX.brightness(pwmValue);  // Set global brightness
  Serial.begin(115200);         // Open the serial port
}

void loop() {
  MATRIX.beginDraw();
  MATRIX.fill(r, g, b); // Set color intensity
  MATRIX.rect(0, 0, MATRIX.width(), MATRIX.height());
  MATRIX.endDraw();

  float supplyVoltage = 3.3;  // Assume a 3.3V supply

  // Calculate effective brightness for each color channel
  float effectiveRed = supplyVoltage * (pwmValue / 255.0) * (r / 255.0);
  float effectiveGreen = supplyVoltage * (pwmValue / 255.0) * (g / 255.0);
  float effectiveBlue = supplyVoltage * (pwmValue / 255.0) * (b / 255.0);

  Serial.print("Effective Voltage (R, G, B): ");
  Serial.print(effectiveRed); Serial.print(" V, ");
  Serial.print(effectiveGreen); Serial.print(" V, ");
  Serial.print(effectiveBlue); Serial.println(" V");

  delay(1000);  // Wait for a second
}
