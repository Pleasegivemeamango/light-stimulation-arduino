#include <ArduinoGraphics.h>
#include <Arduino_MKRRGB.h>
#include <Stream.h>

#define let auto const
using namespace std;

const int NOISE_READOUTS_LEN = 1000;
bool calibration_done = false;
float activation_threshold = 0.0;
float noise_readouts[NOISE_READOUTS_LEN];
int last_noise_index = 0;

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
    
    if (last_noise_index < NOISE_READOUTS_LEN) {
      noise_readouts[last_noise_index] = input_voltage;
      last_noise_index++;
      return;
    }
    if (!calibration_done) {
        let mean_noise = mean(noise_readouts, NOISE_READOUTS_LEN);
        let noise_sd = sd(noise_readouts, NOISE_READOUTS_LEN);
        activation_threshold = mean_noise + 3.0 * noise_sd;

        Serial.println("Calibration done!");
        Serial.print("Mean noise: ");
        Serial.print(mean_noise);
        Serial.println( "V");
        Serial.print("Noise standard deviation: ");
        Serial.print(noise_sd);
        Serial.println(" V");
        Serial.print("Using ");
        Serial.print(activation_threshold);
        Serial.println(" V as a threshold. (5.0 V is the max allowed input)");
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

float mean(float* array, int len) {
  float sum = 0.0;
  for (int i = 0; i < len; i++) {
    sum += array[i];
  }
  return sum / len;
}

float sd(float* array, int len) {
  let u = mean(array, len);
  float sse = 0.0;
  for (int i = 0; i < len; i++) {
    let diff = array[i] - u;
    sse += diff * diff;
  }
  return sqrt(sse / len);
}


  // we echo on the serial port the value read on the analog pin



