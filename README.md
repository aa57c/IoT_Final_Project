# Mining Worker Safety Helmet: An IoT-Based Solution for Hazardous Environments

## Overview
The Mining Worker Safety Helmet is an IoT-enabled wearable device designed to enhance safety in hazardous underground mining environments. By integrating environmental monitoring sensors, location tracking, and real-time communication, this innovative helmet provides miners and management with critical safety data and alerts to improve situational awareness and response times.

## Features
- **Environmental Monitoring**: Continuous gas (e.g., methane, CO) and temperature monitoring to detect hazardous conditions early.
- **Real-Time Alerts**: Visual and auditory alerts (via LEDs and a buzzer) warn workers of dangers.
- **Emergency Button**: A panic button allows workers to manually trigger an emergency alert.
- **Location Tracking**: Bluetooth-based tracking provides real-time worker positioning in underground environments.
- **LCD Display**: Displays live temperature and gas levels for immediate feedback.
- **Web Application Dashboard**: Developed with Node.js and React for centralized safety monitoring and management.

## System Architecture
The system consists of:
1. **ESP32**: Handles wireless communication and Bluetooth-based location tracking.
2. **Arduino Uno**: Interfaces with gas and temperature sensors, processing and transmitting data to the ESP32.
3. **Sensors**: Gas and temperature sensors for environmental monitoring.
4. **User Interaction Components**:
   - Panic button for emergencies.
   - LEDs and buzzer for local alerts.
   - LCD screen for real-time data display.

## Benefits
- **Enhanced Safety**: Early detection of hazards and emergency response capabilities.
- **Power Efficiency**: Low-power design extends operational time without frequent recharges.
- **Scalability**: Cost-effective and easily deployable across multiple helmets.
- **Data Insights**: Real-time data logging for safety analysis and risk management.

## Expected Output
- Real-time environmental monitoring and alerts.
- Emergency alerts and tracking via a user-friendly web dashboard.
- Historical data logging for safety audits and optimization.

## Future Enhancements
- Incorporating advanced sensors (e.g., vibration and heart rate monitors).
- Integration with robust communication protocols like LoRaWAN.
- Augmenting dashboards with machine learning for predictive safety analysis.
- Improved UI, AR integration, and energy harvesting technologies.

## Installation and Usage
1. Set up the hardware components (ESP32, Arduino Uno, sensors, etc.) as per the system architecture.
2. Deploy the Node.js backend and React-based web dashboard.
3. Configure the ESP32 for Wi-Fi and Bluetooth communication.
4. Monitor safety metrics and alerts via the web dashboard or the helmet's local displays.

## Demo
You can find the demo of this project using this link: https://youtu.be/157T500kwEQ
