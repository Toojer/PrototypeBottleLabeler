#include "AccelStepper.h"

void RunMotor(AccelStepper &stepper, bool runAllowed)  //function for the motor
{
  if (runAllowed == true) {
    stepper.run();            //step the motor (this will step the motor by 1 step at each loop)
  } else                      //program enters this part if the runallowed is FALSE, we do not do anything
  {
    stepper.disableOutputs();  //disable outputs
    stepper.stop();
    return;
  }
}

void RunStickerMotor(AccelStepper &stepper, bool runAllowed, bool sensorValue)
{
  if ((runAllowed == true) && (sensorValue)) {
    stepper.run();            //step the motor (this will step the motor by 1 step at each loop)
  } else                      //program enters this part if the runallowed is FALSE, we do not do anything
  {
    stepper.disableOutputs();  //disable outputs
    stepper.stop(); //stop the motor
    stepper.setCurrentPosition(0); //Set Position to 0 to have no reason to move the motor. 
    stepper.moveTo(0); //make sure that the motor doesn't move anymore once sensor triggered
    return;
  }
}