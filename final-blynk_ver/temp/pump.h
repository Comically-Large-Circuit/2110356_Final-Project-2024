#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

// Function declarations
void initPump(int buttonPin, int pumpPin);
void startPump();
void pumpControl(int buttonPin, int pumpPin);
void handlePumpState(unsigned long inputPumpDuration);

#endif
