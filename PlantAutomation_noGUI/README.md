# Automated Plant Care System with Soil Moisture Sensing and Lighting Control using Raspberry Pi 4B

**Author:** Kevin Fox
**Email:** kpf5297@psu.edu

![Project Image](link-to-project-image.jpg)

## Table of Contents
- [Abstract](#abstract)
- [Functional Specification](#functional-specification)
  - [Soil Moisture Sensing](#soil-moisture-sensing)
  - [Lighting Control](#lighting-control)
  - [Water Supply Control](#water-supply-control)
  - [User Interface](#user-interface)
- [Software Design](#software-design)
  - [System State Diagram](#system-state-diagram)
  - [System Class Diagram](#system-class-diagram)
- [Progress](#progress)
- [Materials Required](#materials-required)

## Abstract
The Automated Plant Care System project aims to enhance plant care by utilizing a Raspberry Pi 4B microcomputer to monitor soil moisture levels and control lighting and water supply for optimal plant growth. This system is designed to make plant care effortless, with the core components including capacitive soil moisture sensors, solid-state relays for lighting control, and a water pump. It has the potential to reduce water wastage while maintaining healthy plant soil moisture levels, even during extended periods of absence.

## Functional Specification
### Soil Moisture Sensing
- **Input:** Soil moisture data from capacitive soil sensors through the ADC.
- **Output:** Real-time soil moisture levels.
- **Operation:** Periodically monitors soil moisture and provides data for decision-making.

### Lighting Control
- **Input:** Desired lighting conditions (On/Off).
- **Output:** Control signals to solid-state relays for lighting.
- **Operation:** Adjusts lighting based on predefined conditions to promote plant growth.

### Water Supply Control
- **Input:** Soil moisture thresholds for hydration.
- **Output:** Control signals to the water pump solid-state relay.
- **Operation:** Provides on-demand hydration to maintain soil moisture levels.

### User Interface
- **Input:** User-defined settings and manual control.
- **Output:** User input and system status.
- **Operation:** Offers a menu-based interface for monitoring and manual control.

## Software Design
### System State Diagram
The system state diagram illustrates the procedure followed by the system to maintain the optimal plant environment. Users can set the desired moisture level and make changes to light cycle times. When monitoring is active, the system continuously monitors soil moisture and initiates the hydration cycle when necessary. Lighting is controlled based on a timer and duration method.

![System State Diagram](link-to-system-state-diagram.jpg)

### System Class Diagram
The system class diagram displays the relationships between various classes used to fulfill the functional requirements. The architecture follows the Model-View-Controller (MVC) pattern. The `mainActivity` class handles the program's logic, while user experience elements are located in the `userInterface` class.

![System Class Diagram](link-to-system-class-diagram.jpg)

## Progress
At this stage, the project's architecture has been designed successfully, and hardware assembly is complete. Code is managed through Git for traceability. Classes for reading soil moisture data via I2C communication and controlling relays are functional in the prototype stage. However, timing aspects of the system and components require further refinement.

The project is progressing as per the documented work plan. Upcoming work includes refining system responses, implementing user-friendly menu-based interfaces, addressing timing issues for system stability, and conducting extensive testing under various conditions.

## Materials Required
### Bill Of Materials
1. Raspberry Pi 4B (Raspberry Pi Foundation)
2. Case with Power Supply, 4A (iUniker)
3. RPi GPIO Terminal Block Breakout Board Module (CZH-LABS)
4. 4-Tier Wire Shelving Unit Metal Storage Rack (REGILLER)
5. LED Plant Grow Light Strips, 6000K Full Spectrum, 5V (Wiaxulay)
6. 1.5" 3:1 Waterproof Heat Shrink Tubing (XHF)
7. Capacitive Soil Moisture Sensor, Output voltage: 0 ~ 3.0 VDC (Esooho)
8. Water Pump, OD outlet: 0.29"/7.5mm (WayinTop)
9. Vinyl Tubing, ID 0.22"/5.54mm, OD 0.32"/8.20mm (Generic)
10. Wire, 18 AWG (Generic)
11. ADS1115 16-Bit, 16 Byte, 4-Channel, I2C, ADC (HiLetgo)
12. 1/4 inch Cord Protector Wire Loom Tubing Cable Sleeve (Alex-Tech)

![Materials Image](link-to-materials-image.jpg)

---
