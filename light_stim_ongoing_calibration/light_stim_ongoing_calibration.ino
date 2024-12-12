#include <ArduinoGraphics.h>
#include <Arduino_MKRRGB.h>
#include <Stream.h>

#define let auto const
using namespace std;

const double CALIBRATION_SECONDS = 5.0;
const double STANDARD_DEVIATION_ABOVE_NOISE_MEAN = 3.0;

bool calibration_done = false;
double activation_threshold = 0.0;
double noise_mean = 0.0;
double noise_sse = 0.0;
size_t noise_readout_count = 0;

void setup() {
  MATRIX.begin();
  // set the brightness, supported values are 0 - 255
  MATRIX.brightness(48);
  Serial.begin(115200);  // open the serial port
  Serial1.begin(115201);  // open the serial port
  while (!Serial) {
    ; // wait for serial monitor to open.
  }
  Serial.println("Starting calibration...");
}

void loop() {
    /// incoming serial byte
    let inByte = Serial1.read();
    let input_voltage = readAnalogVoltage(A0);
    let output_voltage = readAnalogVoltage(DAC0);
    Serial.println("output voltage: ");
    Serial.print(output_voltage);
    
    
    // Calibration follows [Welford's online algorithm](https://en.m.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford's_online_algorithm)
    if (millis() < CALIBRATION_SECONDS * 1000.0) {
      noise_readout_count++;
      let delta = input_voltage - noise_mean;
      noise_mean += delta / noise_readout_count;
      let delta2 = input_voltage - noise_mean;
      noise_sse += delta * delta2;
      return;
    }
    if (!calibration_done) {
        let noise_sd = sqrt(noise_sse / (noise_readout_count - 1));
        activation_threshold = noise_mean + STANDARD_DEVIATION_ABOVE_NOISE_MEAN * noise_sd;
        activation_threshold = 0;
        
        Serial.println("Calibration done!");
        Serial.print("Mean noise: ");
        Serial.print(noise_mean);
        Serial.println( "V");
        Serial.print("Noise standard deviation: ");
        Serial.print(noise_sd);
        Serial.println(" V");
        Serial.print("Using ");
        Serial.print(activation_threshold);
        Serial.print(" V as a threshold. ( = ");
        Serial.print(noise_mean);
        Serial.print(" V + ");
        Serial.print(STANDARD_DEVIATION_ABOVE_NOISE_MEAN);
        Serial.print(" * ");
        Serial.print(noise_sd);
        Serial.println(" V)");
        Serial.println("(5.0 V is the max allowed input voltage)");
        Serial.print("noise readout count:");
        Serial.println(noise_readout_count);
        calibration_done = true;
    }

    if (input_voltage > activation_threshold){
      MATRIX.beginDraw();
      MATRIX.fill(22, 22, 22);
      MATRIX.rect(0, 0, MATRIX.width(), MATRIX.height());
      MATRIX.endDraw();
      delay(10);
    }
    else {
      MATRIX.beginDraw();
      MATRIX.fill(0, 0, 0);
      MATRIX.rect(0, 0, MATRIX.width(), MATRIX.height());
      MATRIX.endDraw();
      delay(10);
    }
}

float readAnalogVoltage(uint8_t pin) {
    return analogRead(A0) * 5.0 / 1023.0;
}


