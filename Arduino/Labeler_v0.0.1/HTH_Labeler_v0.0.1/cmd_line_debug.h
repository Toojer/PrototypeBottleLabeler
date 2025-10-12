#ifndef CMDLINEDEBUG_H
#define CMDLINEDEBUG_H
#endif

#include "AccelStepper.h"

// Function declarations
bool checkSerial(AccelStepper &sticker, AccelStepper &conveyor);
  //This checks for user inputs on the command line and responds to the commands
bool RotateRelative(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps);
  //Rotates the amount of steps specified relative to the current location
bool RotateAbsolute(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps);
  //Rotates to the absolute value specified
void PrintCommands();  
  //Specific for sticker dispenser.  will stop sticker motor when edge of sticker located.
bool DispenseSticker(AccelStepper &stepper, long receivedSpeed, long receivedSteps);
 //Parses the input number strings
float ParseNextFloat();
int getUserInput(String prompt);