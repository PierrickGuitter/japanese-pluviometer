# japanese-pluviometer

This project is all about turning a japanese fountain "shishi-odishi" into a pluviometer, 
thanks to a proximity (InfraRed) detector.

## Electrical Design

The main components are nothing more than a micro-controller MSP430G2553, a IR proximity sensor VL6180X, and a 16x2 LCD.

## Software Design

The whole purpose is to detect the movement of the bamboo tube when the fountain becomes empty. 
Every passage of the tube gives the volume, as it is constant. 
