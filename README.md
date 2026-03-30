# Boardoza TPiS 1T 1386 L5.5 H Thermopile Sensor Breakout Board

The **Boardoza TPiS 1T 1386 L5.5 H Breakout Board** is a non-contact infrared temperature sensor board designed for makers, developers, and embedded system enthusiasts who need accurate temperature measurements without physical contact. It uses the **Excelitas TPiS 1T 1386 L5.5 H** thermopile sensor to measure object temperature in a fast, hygienic, and contactless way, making it ideal for projects where traditional contact sensors are not suitable.

The onboard sensor provides strong thermal stabilization, ensuring reliable and consistent measurements even under harsh or changing environmental conditions. Communication is handled via the **I²C interface**, allowing easy integration with popular development platforms such as Arduino, ESP32, and Raspberry Pi. This breakout board is well suited for **maker projects and prototypes including non-contact thermometers, industrial temperature monitoring, HVAC systems, smart home applications, medical devices, and general IoT-based temperature sensing solutions**.

## [Click here to purchase!](https://www.ozdisan.com/p/gelistirme-kitleri-ve-aksesuarlari-617/boardoza-tpis-1t-1386-1202719)
| Front Side | Back Side |
|:---:|:---:|
| ![TPiS 1T 1386 Front](./assets/TPiS_1T_1386L5%20Front.png) | ![TPiS 1T 1386 Back](./assets/TPiS_1T_1386L5%20Back.png) |

---

## Key Features

- **Non-Contact Temperature Measurement:** Measures object temperature without physical contact using infrared sensing technology.
- **Excelitas Thermopile Sensor:** Equipped with the **TPiS 1T 1386 L5.5 H** medical-grade thermopile sensor.
- **Thermal Stabilization:** Delivers stable and reliable temperature readings even in harsh or changing environmental conditions.
- **I²C Communication Interface:** Standard I²C protocol for easy integration with microcontrollers and development boards.
- **Wide Supply Voltage Support:** Compatible with both **3.3V and 5V** systems.
- **Interrupt Output:** Open-drain, active-low interrupt pin for event-based temperature monitoring.

---

## Technical Specifications

**Model:** TPiS 1T 1386 L5.5 H  
**Manufacturer:** Boardoza  
**Manufacturer IC:** Excelitas Technologies  
**Supply Voltage:** 3.3V - 5V  
**Functions:** Non-contact infrared temperature sensor  
**Communication Interface:** I²C  
**I²C Clock Speed:** Up to 400 kHz (clock stretching supported)  
**Interrupt Output:** Open-drain, active-low  
**Maximum Current per Pin:** −100 mA to +100 mA  
**Storage Temperature:** −40 °C to +105 °C  
**Operating Temperature:** −20 °C to +105 °C  
**Board Dimensions:** 20 mm × 40 mm

---

## Board Pinout

### ( J1 ) I²C & Power Connector

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | VCC | Power supply input (3.3V - 5V) |
| 2 | SCL | I²C serial clock line |
| 3 | SDA | I²C bidirectional data line |
| 4 | GND | Ground |

### ( J2 ) Interrupt Connector

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | INT | Open-drain, active-low interrupt output. Cleared by reading sensor registers |

---

## Board Dimensions

<img src="./assets/TPiS_1T_1386L5 Dimensions.png" alt="Board Dimensions" width="450"/>

---

## Step Files

[Boardoza TPiS_1T_1386L5.step](./assets/TPiS_1T_1386L5%20Step.step)

---

## Datasheet

[TPiS 1T 1386 L5.5 H Datasheet.pdf](./assets/TPiS_1T_1386L5%20Datasheet.pdf)

---

## Version History

- V1.0.0 – Initial Release

---

## Support

- If you have any questions or need support, please contact **support@boardoza.com**

---

## **License**

This repository contains both hardware and software components:

### **Hardware Design**

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

All hardware design files are licensed under [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

### **Software/Firmware**

[![BSD-3-Clause][bsd-shield]][bsd]

All software and firmware are licensed under [BSD 3-Clause License][bsd].

[bsd]: https://opensource.org/licenses/BSD-3-Clause
[bsd-shield]: https://img.shields.io/badge/License-BSD%203--Clause-blue.svg
