#include <ArduinoGraphics.h> // Arduino_MKRRGB depends on ArduinoGraphics
#include <Arduino_MKRRGB.h>
#include <Stream.h>

// Example controlling brightness of an LED using PWM
int ledPin = 9; // Example pin (make sure it's PWM-capable)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(11200);
}

void loop() {
  // Vary the brightness using PWM
  for (int i = 0; i <= 255; i++) {
    analogWrite(ledPin, i); // Set the brightness from 0 to 255
    delay(10);  // Small delay for visible change
  }
}

int pwmValue = 128;  // Example PWM value (50% duty cycle)
float supplyVoltage = 3.3;  // Assume a 3.3V supply

float averageVoltage = (pwmValue / 255.0) * supplyVoltage;
Serial.print("Average voltage to LED: ");
Serial.println(averageVoltage);

