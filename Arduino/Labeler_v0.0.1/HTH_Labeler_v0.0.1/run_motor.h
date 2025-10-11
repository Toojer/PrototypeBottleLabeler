#ifndef RUN_MOTOR_H
#define RUN_MOTOR_H
#endif

#include "AccelStepper.h"

void RunMotor(AccelStepper &stepper, bool runAllowed);
  //Runs the motor according to the stimulus given.

void RunStickerMotor(AccelStepper &stepper, bool runAllowed, int sensorValue);
  //Runs the sticker motor until the sensor reads the sticker edge.