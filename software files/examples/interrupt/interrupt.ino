#include "caliPile.h"

#define interruptPin 16
uint8_t  intStatus = 0;
bool presSign = false, motSign = false;
caliPile sensor(interruptPin);

void inthandler() {
  newInt = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(interruptPin, INPUT);
  Wire.begin();

  sensor.activateSensor();

  sensor.initMotion(LP_8s, LP_1s, src_TPOBJLP1_TPOBJLP2, ms30);

  sensor.TempCalculations();
  sensor.initTPotThreshHold(0x83); //0x83

  sensor.initTpMotionThreshHold(2);

  sensor.initTpPresenceThreshHold(2);
  attachInterrupt(digitalPinToInterrupt(interruptPin), inthandler, FALLING);  // define interrupt for INT pin output of CaliPile
}

void loop() {

  uint8_t presence = sensor.readRegister(SENSOR_ADDRESS, TPPRESENCE);
  uint8_t motion = sensor.readRegister(SENSOR_ADDRESS, TPMOTION);
  uint8_t ambshock = sensor.readRegister(SENSOR_ADDRESS, TPAMB_SHOCK);

  /*if(interruptPin == 0) {
    Serial.println(sensor.readRegister(SENSOR_ADDRESS, INTERRUPT_STATUS));
  } */

  Serial.print(presence);
  Serial.print("  ");
  Serial.print(motion);
  Serial.print("  ");
  Serial.println(ambshock);

  /*Serial.print(sensor.calcAmbientTemp(sensor.getAmbientTemp()));
  Serial.print("  ");
  Serial.print(sensor.calcObjectTemp(sensor.getObjectTemp(), sensor.calcAmbientTemp(sensor.getAmbientTemp())));
  Serial.println("  ");*/
  delay(100);
}