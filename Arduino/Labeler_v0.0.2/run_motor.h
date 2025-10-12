#ifndef RUN_MOTOR_H
#define RUN_MOTOR_H
#endif

#include "AccelStepper.h"
//Runs the motor according to the stimulus given.
void RunMotor(AccelStepper &stepper, bool runAllowed);
  
//Runs the sticker motor until the sensor reads the sticker edge.
void RunStickerMotor(AccelStepper &stepper, bool runAllowed, bool sensorValue);
  