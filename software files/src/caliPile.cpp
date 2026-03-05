#include "caliPile.h"

/**
 * @brief Constructor for the caliPile class.
 * 
 * This constructor initializes an instance of the caliPile class.
 * 
 * @param intPin Pin number to be used as the interrupt pin. 
 *               This pin will be configured as an input pin.
 */
//#define TPIS_1S
#define TPIS_1T

#ifdef TPIS_1S
float lookUpNumber = (float) 3.8f; 
#endif
#ifdef TPIS_1T
float lookUpNumber = (float) 4.2f;
#endif

bool newInt = false;

/**
 * @brief Constructor for the caliPile class.
 * 
 * Initializes the caliPile object with the specified interrupt pin.
 * 
 * @param intPin The interrupt pin to be used for the caliPile object.
 */
caliPile::caliPile(uint8_t intPin) {
    pinMode(intPin, INPUT);
    interruptPin = intPin;
}

/**
 * @brief Activates the sensor by sending the call and reload command.
 * 
 * This function activates the sensor by sending the call and reload command.
 * It writes the appropriate values to the registers and introduces a delay
 * to allow the sensor to initialize.
 * 
 * @note This function assumes that the necessary registers have been properly configured before calling it.
 *       It is important to ensure that the appropriate register settings are applied before activating the sensor.
 */
void caliPile::activateSensor() {
    writeRegister(0x00, 0x04, 0x00); // Call and reload command
    delay(10);
}

uint8_t caliPile::interruptStatus() {
    uint8_t tempValue = readRegister(SENSOR_ADDRESS, INTERRUPT_STATUS);
    return tempValue;
}

/**
 * @brief Initializes the motion detection settings of the sensor.
 * 
 * This function initializes the motion detection settings of the sensor by
 * configuring the necessary registers with the provided parameters.
 * It sets the LPTime1 and LPTime2 values, selects the temperature source,
 * configures the cycle time, and sets the presence and motion thresholds.
 * 
 * @param LPTime1 The value for LPTime1 register. Determines the low power time duration.
 * @param LPTime2 The value for LPTime2 register. Determines the low power time duration.
 * @param tempSource The temperature source selection value for the SRC_SELECT register.
 * @param cycleTime The cycle time value for the SRC_SELECT register. Determines the measurement cycle time.
 */
void caliPile::initMotion(uint8_t LPTime1, uint8_t LPTime2, uint8_t tempSource, uint8_t cycleTime) {
    writeRegister(SENSOR_ADDRESS, INT_MASK, 0x1C);
    writeRegister(SENSOR_ADDRESS, SLP12, LPTime2 << 4 | LPTime1);
    uint8_t temp = readRegister(SENSOR_ADDRESS, SRC_SELECT); 
    writeRegister(SENSOR_ADDRESS, SRC_SELECT, temp | tempSource << 2 | cycleTime);
    writeRegister(SENSOR_ADDRESS, TP_PRES_THLD, 0x22); // presence threshold
    writeRegister(SENSOR_ADDRESS, TP_MOT_THLD, 0x0A); // motion threshold
}

/**
 * @brief Perform temperature calculations based on sensor EEPROM data.
 * 
 * This function reads EEPROM data from the sensor, performs temperature calculations using the data,
 * and calculates the value of 'k' based on the formula provided.
 */
void caliPile::TempCalculations() {
    writeRegister(SENSOR_ADDRESS, EEPROM_CONTROL, 0x80);
    readRegisters(SENSOR_ADDRESS, EEPROM_PTAT25, 2, &tempData[0]);
    PTAT25 = ((uint16_t) tempData[0] << 8) | tempData[1];
    readRegisters(SENSOR_ADDRESS, EEPROM_M, 2, &tempData[0]);
    M = ((uint16_t) tempData[0] << 8) | tempData[1];
    M /= 100;
    readRegisters(SENSOR_ADDRESS, EEPROM_U0, 2, &tempData[0]);
    U0 = ((uint16_t) tempData[0] << 8) | tempData[1];
    U0 += 32768;
    readRegisters(SENSOR_ADDRESS, EEPROM_UOUT1, 2, &tempData[0]);
    UOUT1 = ((uint16_t) tempData[0] << 8) | tempData[1];
    UOUT1 *= 2;
    TOBJ1 = readRegister(SENSOR_ADDRESS, EEPROM_TOBJ1);
    readRegisters(SENSOR_ADDRESS, EEPROM_CHECKSUM, 2, &tempData[0]);
    CHECKSUM = ((uint16_t) tempData[0] << 8) | tempData[1];
    writeRegister(SENSOR_ADDRESS, EEPROM_CONTROL, 0x00);

    k = ( (float) (UOUT1 - U0) )/(powf((float)(TOBJ1 + 273.15f), lookUpNumber) - powf(25.0f + 273.15f, lookUpNumber) );

}

/**
 * @brief Initializes the temperature threshold settings of the sensor.
 * 
 * This function initializes the temperature threshold settings of the sensor
 * by configuring the necessary registers with the provided threshold value.
 * It writes the threshold value to the TPOT_THR register, sets the higher byte
 * to 0x00, sets the temperature source selection bit, and reads the threshold 
 * value back from the sensor registers.
 * 
 * @param Tcounts The threshold value to be set in the TPOT_THR register.
 */
void caliPile::initTPotThreshHold(uint16_t Tcounts) {
    uint8_t rawData[2] = {0, 0};
    writeRegister(SENSOR_ADDRESS, TPOT_THR, Tcounts);
    writeRegister(SENSOR_ADDRESS, (TPOT_THR + 1), 0x00);
    uint8_t temp = readRegister(SENSOR_ADDRESS, SRC_SELECT);
    writeRegister(SENSOR_ADDRESS, SRC_SELECT, temp | 0x10);
    readRegisters(SENSOR_ADDRESS, TPOT_THR, 2, &rawData[0]);
    uint16_t TPOTTHR = ((uint16_t) rawData[0] << 8) | rawData[1];
}

/**
 * @brief Initializes the temperature motion threshold value in the sensor.
 * 
 * This function sets the temperature motion threshold value in the sensor to the specified Tcounts.
 * 
 * @param Tcounts The temperature motion threshold value to be set in the sensor.
 */
void caliPile::initTpMotionThreshHold(uint16_t Tcounts) {
    writeRegister(SENSOR_ADDRESS, TP_MOT_THLD, Tcounts);
}

/**
 * @brief Initializes the temperaturepresence threshold value in the sensor.
 * 
 * This function sets the temperature presence threshold value in the sensor to the specified Tcounts.
 * 
 * @param Tcounts The temperature presence threshold value to be set in the sensor.
 */
void caliPile::initTpPresenceThreshHold(uint16_t Tcounts) {
    writeRegister(SENSOR_ADDRESS, TP_PRES_THLD, Tcounts);
}

/**
 * @brief Get the ambient temperature from the sensor.
 * 
 * This function reads the ambient temperature data from the sensor, processes it, and returns the ambient temperature value.
 * 
 * @return The ambient temperature value obtained from the sensor in degrees Celsius.
 * 
 * @details
 * The ambient temperature value is read from the sensor register TPAMBIENT and processed to obtain the actual temperature value.
 * The temperature value is returned as a floating-point value in degrees Celsius.
 */
float caliPile::getAmbientTemp() {
    uint8_t rawData[2] = {0, 0};
    readRegisters(SENSOR_ADDRESS, TPAMBIENT, 2, &rawData[0]);
    uint16_t TPAmbient = ((uint16_t)(rawData[0] & 0x7F) << 8) | rawData[1];
 
    return TPAmbient;
}

/**
 * @brief Retrieves the object temperature reading from the sensor.
 * 
 * This function retrieves the object temperature reading from the sensor
 * by reading the registers where the temperature data is stored.
 * It reads the temperature data from the TPOBJECT register and combines
 * the three bytes to form a 32-bit value. The MSB is masked to ensure the sign
 * bit is not considered, and the temperature value is shifted to the right by 15 bits.
 * The resulting 32-bit value is returned as the object temperature reading.
 * 
 * @return The object temperature reading from the sensor.
 */
uint32_t caliPile::getObjectTemp() {
    
    uint8_t rawData[3] = {0, 0, 0};
    readRegisters(SENSOR_ADDRESS, TPOBJECT, 3, &rawData[0]);
    uint32_t TPObject = ((uint32_t) ( (uint32_t)rawData[0] << 24) | ( (uint32_t)rawData[1] << 16) | ( (uint32_t)rawData[2] & 0x80) << 8) >> 15;

    return TPObject;
}

float caliPile::convertToCelcius(float temp_val ) {
    return (temp_val - 273.15);
}

/**
 * @brief Retrieves the object temperature reading in low power mode 1 from the sensor.
 * 
 * This function retrieves the object temperature reading in low power mode 1 from the sensor
 * by reading the registers where the temperature data is stored.
 * It reads the temperature data from the TPOBJLP1 register and combines
 * the three bytes to form a 32-bit value. The temperature value is then shifted to the right
 * by 4 bits to remove the lower 4 bits. The resulting 32-bit value is divided by 8 to convert
 * it to the correct temperature scale, and the object temperature reading is returned.
 * 
 * @return The object temperature reading in low power mode 1 from the sensor.
 */
uint32_t caliPile::getObjectTempLP1() {
    uint8_t rawData[3] = {0, 0, 0};
    readRegisters(SENSOR_ADDRESS, TPOBJLP1, 3, &rawData[0]);
    uint32_t temp = (((uint32_t) rawData[0] << 16) | ((uint32_t) rawData[1] << 8) | ( (uint32_t)rawData[2] & 0xF0) ) >> 4;
    temp /= 8;
    return temp;
}

/**
 * @brief Retrieves the object temperature reading in low power mode 2 from the sensor.
 * 
 * This function retrieves the object temperature reading in low power mode 2 from the sensor
 * by reading the registers where the temperature data is stored.
 * It reads the temperature data from the TPOBJLP2 register and combines
 * the three bytes to form a 32-bit value. The temperature value is divided by 8 to convert
 * it to the correct temperature scale, and the object temperature reading is returned.
 * 
 * @return The object temperature reading in low power mode 2 from the sensor.
 */
uint32_t caliPile::getObjectTempLP2() {
    uint8_t rawData[3] = {0, 0, 0};
    readRegisters(SENSOR_ADDRESS, TPOBJLP2, 3, &rawData[0]);
    uint32_t temp = ((uint32_t) (rawData[0] & 0x0F) << 16) | ((uint32_t) rawData[1] << 8) | rawData[2];
    temp /= 8;
    return temp;
}

/**
 * @brief Retrieves the ambient temperature reading in low power mode 3 from the sensor.
 * 
 * This function retrieves the ambient temperature reading in low power mode 3 from the sensor
 * by reading the registers where the temperature data is stored.
 * It reads the temperature data from the TPAMBLP3 register and combines
 * the two bytes to form a 16-bit value. The temperature value is divided by 2 to convert
 * it to the correct temperature scale, and the ambient temperature reading is returned.
 * 
 * @return The ambient temperature reading in low power mode 3 from the sensor.
 */
uint16_t caliPile::getAmbientTempLP3() {
    uint8_t rawData[2] = {0, 0};
    readRegisters(SENSOR_ADDRESS, TPAMBLP3, 3, &rawData[0]);
    uint16_t temp = ((uint16_t) rawData[0] << 8) | rawData[1];
    temp /= 2;
    return temp;
}

/**
 * @brief Retrieves the frozen object temperature reading in low power mode 2 from the sensor.
 * 
 * This function retrieves the frozen object temperature reading in low power mode 2 from the sensor
 * by reading the registers where the temperature data is stored.
 * It reads the temperature data from the TPOBJLP2_FRZN register and combines
 * the three bytes to form a 32-bit value. The temperature value is divided by 128 to convert
 * it to the correct temperature scale, and the frozen object temperature reading is returned.
 * 
 * @return The frozen object temperature reading in low power mode 2 from the sensor.
 */
uint32_t caliPile::getObjectTempLP2Frozen() {
    uint8_t rawData[3] = {0, 0, 0};
    readRegisters(SENSOR_ADDRESS, TPOBJLP2_FRZN, 3, &rawData[0]);
    uint32_t temp = ((uint32_t) rawData[0] << 16) | ((uint32_t) rawData[1] << 8) | rawData[2];
    temp /= 128;
    return temp;
}

/**
 * @brief Retrieves the presence status from the sensor.
 * 
 * This function retrieves the presence status from the sensor
 * by reading the register where the presence status data is stored.
 * It reads the presence status data from the TPPRESENCE register and returns the value.
 * 
 * @return The presence status from the sensor.
 */
uint8_t caliPile::getPresenceStat() {
    uint8_t temp = readRegister(SENSOR_ADDRESS, TPPRESENCE);
    return temp;
}

/**
 * @brief Retrieves the motion status from the sensor.
 * 
 * This function retrieves the motion status from the sensor
 * by reading the register where the motion status data is stored.
 * It reads the motion status data from the TPMOTION register and returns the value.
 * 
 * @return The motion status from the sensor.
 */
uint8_t caliPile::getMotionStat() {
    uint8_t temp = readRegister(SENSOR_ADDRESS, TPMOTION);
    return temp;
}

/**
 * @brief Retrieves the ambient shock status from the sensor.
 * 
 * This function retrieves the ambient shock status from the sensor
 * by reading the register where the ambient shock status data is stored.
 * It reads the ambient shock status data from the TPAMB_SHOCK register and returns the value.
 * 
 * @return The ambient shock status from the sensor.
 */
uint8_t caliPile::getAmbientShockStat() {
    uint8_t temp = readRegister(SENSOR_ADDRESS, TPAMB_SHOCK);
    return temp;
}

/**
 * @brief Calculates the ambient temperature from the raw ambient temperature reading.
 * 
 * This function calculates the ambient temperature from the raw ambient temperature reading
 * by using the PTAT25 and M constants. It subtracts the PTAT25 value from the ambient temperature reading,
 * multiplies it by the reciprocal of the M constant, and adds it to the reference temperature of 298.15 degrees Kelvin.
 * The resulting temperature is returned as a floating-point value.
 * 
 * @param ambientTemp The raw ambient temperature reading.
 * @return The calculated ambient temperature in degrees Kelvin.
 */
float caliPile::calcAmbientTemp(uint16_t ambientTemp) {
    float temp = 298.15f + ((float)ambientTemp - (float)PTAT25) * (1.0f / (float)M);
    return temp;
}

/**
 * @brief Calculates the object temperature from the raw object temperature reading and ambient temperature.
 * 
 * This function calculates the object temperature from the raw object temperature reading and ambient temperature.
 * It uses the U0 and k constants, as well as mathematical formulas, to perform the calculation.
 * It first calculates `temp0` by raising the ambient temperature to the power of 3.8.
 * Then, it calculates `temp1` by subtracting the U0 constant from the raw object temperature and dividing by the k constant.
 * Next, it calculates `temp2` by adding `temp0` and `temp1`, and raising the result to the power of 0.2631578947.
 * The resulting temperature is returned as a floating-point value.
 * 
 * @param objectTemp The raw object temperature reading.
 * @param ambientTemp The calculated ambient temperature in degrees Kelvin.
 * @return The calculated object temperature.
 */
float caliPile::calcObjectTemp(uint32_t objectTemp, float ambientTemp) {
    float temp0 = powf(ambientTemp, lookUpNumber);
    float temp1 = (((float) objectTemp) - ((float) U0)) / k ;
    float result_temp = powf((temp0 + temp1), 1.0 / lookUpNumber);
    return result_temp;
}

/**
 * @brief Writes data to a register of the sensor.
 * 
 * This function writes data to a register of the sensor specified by the address parameter.
 * It uses the altAddress parameter to specify the address of the register within the device.
 * The data parameter represents the value to be written to the register.
 * 
 * @param address The address of the sensor.
 * @param altAddress The address of the register within the sensor.
 * @param data The data value to be written to the register.
 */
void caliPile::writeRegister(uint8_t address, uint8_t altAddress, uint8_t data) {
    uint8_t temp[2];
    temp[0] = altAddress;
    temp[1] = data;
    Wire.beginTransmission(address);
    Wire.write(&temp[0], 2);
    Wire.endTransmission();
}

/**
 * @brief Reads data from a register of the sensor.
 * 
 * This function reads data from a register of the sensor specified by the address parameter.
 * It uses the altAddress parameter to specify the address of the register within the device.
 * The function initiates a transmission to the sensor using Wire.beginTransmission(),
 * writes the altAddress to request the data from the specified register using Wire.write(),
 * and ends the transmission using Wire.endTransmission().
 * It then requests one byte of data from the sensor using Wire.requestFrom(),
 * and if data is available, it reads the byte using Wire.read() and stores it in the temp array.
 * Finally, it returns the read data.
 *
 * @param address The address of the sensor.
 * @param altAddress The address of the register within the sensor.
 * @return The data read from the specified register.
 */
uint8_t caliPile::readRegister(uint8_t address, uint8_t altAddress) {
    uint8_t temp[1];
    Wire.beginTransmission(address);
    Wire.write(altAddress);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);
    if (Wire.available()) {
        temp[0] = Wire.read();
    }
    return temp[0];
}

/**
 * @brief Reads multiple registers from the sensor.
 * 
 * This function reads multiple registers from the sensor specified by the address parameter.
 * It uses the altAddress parameter to specify the starting address of the registers within the device.
 * The count parameter represents the number of registers to read, and the target parameter is a pointer to an array where the read data will be stored.
 * 
 * The function initiates a transmission to the sensor using Wire.beginTransmission(),
 * writes the altAddress to request the data from the specified starting register using Wire.write(),
 * and ends the transmission using Wire.endTransmission().
 * It then requests the specified number of bytes of data from the sensor using Wire.requestFrom(),
 * and for each byte of data available, it reads it using Wire.read() and stores it in the target array.
 * 
 * @param address The address of the sensor.
 * @param altAddress The starting address of the registers within the sensor.
 * @param count The number of registers to read.
 * @param target Pointer to an array where the read data will be stored.
 */
void caliPile::readRegisters(uint8_t address, uint8_t altAddress, uint8_t count, uint8_t *target) {
    Wire.beginTransmission(address);
    Wire.write(altAddress);
    Wire.endTransmission();
    Wire.requestFrom(address, count);
    for (uint8_t i = 0; i < count; i++) {
        if (Wire.available()) {
            target[i] = Wire.read();
        }
    }
}