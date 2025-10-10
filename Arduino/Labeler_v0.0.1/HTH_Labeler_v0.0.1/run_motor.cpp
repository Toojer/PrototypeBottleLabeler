#include "AccelStepper.h"

void RunMotor(AccelStepper &stepper, bool runAllowed, int sensorValue)  //function for the motor
{
  if ((runAllowed == true) && (sensorValue != LOW)) {
    stepper.enableOutputs();  //enable pins
    stepper.run();            //step the motor (this will step the motor by 1 step at each loop)
  } else                      //program enters this part if the runallowed is FALSE, we do not do anything
  {
    stepper.disableOutputs();  //disable outputs
    stepper.stop();
    return;
  }
}