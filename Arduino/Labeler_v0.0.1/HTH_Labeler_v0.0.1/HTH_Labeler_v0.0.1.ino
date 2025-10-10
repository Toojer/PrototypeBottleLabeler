#include <AccelStepper.h>
#include "cmd_line_debug.h"

int sensorValue = HIGH;         //Assign HIGH signaling sticker, not sticker the gap.

bool runAllowed = false;
#define irSensorPin 2  //Assign pin two as digital input.
AccelStepper stepper(1, 8, 9);  // direction Digital 9 (CCW), pulses Digital 8 (CLK)

void setup() {
  
  stepper.setMaxSpeed(400);                                 //SPEED = Steps / second
  stepper.setAcceleration(800);                             //ACCELERATION = Steps /(second)^2
  stepper.disableOutputs();                                 //disable outputs
 
  Serial.begin(9600);                                       //define baud rate
  Serial.println("Demonstration of AccelStepper Library");  //print a messages
  Serial.println("Send 'C' for printing the commands.");
  
  pinMode(irSensorPin, INPUT);                              // Set IR sensor pin as input
}

void loop() {
  //Constantly looping through these 2 functions.
  //We only use non-blocking commands, so something else (should also be non-blocking) can be done during the movement of the motor

  runAllowed = checkSerial(stepper);  //check serial port for new commands
  sensorValue = digitalRead(irSensorPin);  // Read the value from the IR sensor
  RunTheMotor(stepper, runAllowed, sensorValue);  //function to handle the motor

  
}

void RunTheMotor(AccelStepper &stepper, bool runAllowed, int sensorValue)  //function for the motor
{
  if ((runAllowed) && (sensorValue)) {
    stepper.enableOutputs();  //enable pins
    stepper.run();            //step the motor (this will step the motor by 1 step at each loop)
  } else                      //program enters this part if the runallowed is FALSE, we do not do anything
  {
    stepper.disableOutputs();  //disable outputs
    stepper.stop();
    return;
  }
}