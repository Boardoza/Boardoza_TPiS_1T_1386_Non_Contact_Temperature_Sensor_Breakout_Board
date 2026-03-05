#ifndef caliPile_h
#define caliPile_h

#include "Arduino.h"
#include "Wire.h"
// Sensor registers
#define SENSOR_ADDRESS 0x0C
#define TPOBJECT 1
#define TPAMBIENT 3
#define TPOBJLP1 5
#define TPOBJLP2 7
#define TPAMBLP3 10
#define TPOBJLP2_FRZN 12
#define TPPRESENCE 15
#define TPMOTION 16
#define TPAMB_SHOCK 17
#define INTERRUPT_STATUS 18
#define CHIP_STATUS 19
#define SLP12 20
#define SLP3 21
#define TP_PRES_THLD 22
#define TP_MOT_THLD 23
#define TP_AMB_SHOCK_THLD 24
#define INT_MASK 25
#define SRC_SELECT 26
#define TMR_INT 27
#define TPOT_THR 28
// EEPROM addresses
#define EEPROM_CONTROL 31
#define EEPROM_PROTOCOL 32
#define EEPROM_CHECKSUM 33
#define EEPROM_LOOKUPNUM 41
#define EEPROM_PTAT25 42
#define EEPROM_M 44
#define EEPROM_U0 46
#define EEPROM_UOUT1 48
#define EEPROM_TOBJ1 50
#define SLAVE_ADDRESS 63
// Low-pass time
#define LP_512s 0x00
#define LP_256s 0x01
#define LP_128s 0x02
#define LP_64s 0x03
#define LP_32s 0x04
#define LP_16s 0x05
#define LP_8s 0x08
#define LP_4s 0x09
#define LP_2s 0x0A
#define LP_1s 0x0B
#define LP_0_50s 0x0C
#define LP_0_25s 0x0D
// Source select
#define src_TPOBJ_TPOBJLP2 0x00
#define src_TPOBJLP1_TPOBJLP2 0x01
#define src_TPOBJ_TPOBJLP2FRZN 0x02
#define src_TPOBJLP1_TPOBJLP2FRZN 0x03
// Cycle time
#define ms30 0x00
#define ms60 0x01
#define ms120 0x02
#define ms240 0x03

extern bool newInt;

class caliPile {
public:
    caliPile(uint8_t pin);
    //void myinthandler();
    void activateSensor();
    uint8_t interruptStatus();
    void readMemory();
    void initMotion(uint8_t LPTime1, uint8_t LPTime2, uint8_t tempSource, uint8_t cycleTime);
    void TempCalculations();
    void initTPotThreshHold(uint16_t Tcounts);
    void initTpMotionThreshHold(uint16_t Tcounts);
    void initTpPresenceThreshHold(uint16_t Tcounts);
    float getAmbientTemp();
    uint32_t getObjectTemp();
    uint32_t getObjectTempLP1();
    uint32_t getObjectTempLP2();
    uint16_t getAmbientTempLP3();
    uint32_t getObjectTempLP2Frozen();
    uint8_t getPresenceStat();
    uint8_t getMotionStat();
    uint8_t getAmbientShockStat();
    float convertToCelcius(float temp_val);
    float calcAmbientTemp(uint16_t ambientTemp);
    float calcObjectTemp(uint32_t objectTemp, float ambientTemp);
    void writeRegister(uint8_t address, uint8_t altAddress, uint8_t data);
    uint8_t readRegister(uint8_t address, uint8_t altAddress);
    void readRegisters(uint8_t address, uint8_t altAddress, uint8_t count, uint8_t *target);
    uint8_t tempData[3] = {0, 0, 0};

    uint16_t PTAT25, M, U0, CHECKSUM;
    uint32_t UOUT1;
    uint8_t TOBJ1, lookup;
    float k, AmbientT, ObjectT;
    uint8_t interruptPin;
    
private:

};

#endif