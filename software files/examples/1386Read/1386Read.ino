#include "caliPile.h"

const int interruptPin = 4;
uint8_t  intStatus = 0;
bool presSign = false, motSign = false;
caliPile sensor(interruptPin);

void inthandler() {
  newInt = true;
}

void setup() {
  Serial.begin(115200);

  Wire.begin();

  sensor.activateSensor();

  sensor.initMotion(LP_8s, LP_1s, src_TPOBJLP1_TPOBJLP2, ms30);

  sensor.TempCalculations();
  sensor.initTPotThreshHold(131); //0x83

  sensor.initTpMotionThreshHold(10);

  sensor.initTpPresenceThreshHold(30);
  attachInterrupt(digitalPinToInterrupt(interruptPin), inthandler, FALLING);  // define interrupt for INT pin output of CaliPile
}

void loop() {

  Serial.print(sensor.calcAmbientTemp(sensor.getAmbientTemp()));
  Serial.print("  ");
  Serial.print(sensor.calcObjectTemp(sensor.getObjectTemp(), sensor.calcAmbientTemp(sensor.getAmbientTemp())));
  Serial.println("  ");
  delay(100);
}