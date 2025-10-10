#ifndef CMDLINEDEBUG_H
#define CMDLINEDEBUG_H
#endif

#include "AccelStepper.h"

// Function declarations
bool checkSerial(AccelStepper &stepper);
void GoHome(AccelStepper &stepper);
bool RotateRelative(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps);
bool RotateAbsolute(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps);
void PrintCommands();  