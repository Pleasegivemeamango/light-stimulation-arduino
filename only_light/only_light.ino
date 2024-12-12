#include <ArduinoGraphics.h>  // Arduino_MKRRGB depends on ArduinoGraphics
#include <Arduino_MKRRGB.h>
#include <Stream.h>

#define let auto const
using namespace std;

int pwmValue = 16;

void setup() {
  MATRIX.begin();
  MATRIX.brightness(pwmValue);  // set the brightness, supported values are 0 - 255
  Serial.begin(115200);         // open the serial port
}

void loop() {
  let inByte = Serial.read();
  let input_voltage = readAnalogVoltage(A1);
  Serial.println("input voltage: ");
  Serial.print(input_voltage);


  MATRIX.beginDraw();
  MATRIX.fill(72, 72, 72);
  MATRIX.rect(0, 0, MATRIX.width(), MATRIX.height());
  MATRIX.endDraw();
  delay(10);


  float supplyVoltage = 3.3;  // Assume a 3.3V supply

  float averageVoltage = (pwmValue / 255.0) * supplyVoltage;
  //Serial.print("Average voltage to LED: ");
  //Serial.println(averageVoltage);
  delay(1000);
}

float readAnalogVoltage(uint8_t pin) {
    return analogRead(A1) * 5 / 1023.0;
}

