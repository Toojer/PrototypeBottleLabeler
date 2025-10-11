#include <AccelStepper.h>
#include "cmd_line_debug.h"
#include "run_motor.h"

int sensorValue = HIGH;  //Assign HIGH signaling sticker, not the sticker edge.
int buttonValue = HIGH; //Assign HIGH to show button high

bool runAllowed = true;
#define irSensorPin 2                 //Assign pin two as digital input.
AccelStepper conveyorMotor(1, 8, 9);  // direction Digital 9 (CCW), pulses Digital 8 (CLK)
AccelStepper stickerMotor(1, 5, 6);   //Direction digital 6 (CCW) , pulses Digital 5 (CLK)

void setup() {
  //Conveyor belt control motor
  conveyorMotor.setMaxSpeed(400);      //SPEED = Steps / second
  conveyorMotor.setAcceleration(800);  //ACCELERATION = Steps /(second)^2
  conveyorMotor.disableOutputs();      //disable outputs

  //Sticker Motor Control
  stickerMotor.setMaxSpeed(400);
  stickerMotor.setAcceleration(800);
  stickerMotor.disableOutputs();

  //debug serial if available
  Serial.begin(9600);                                       //define baud rate
  Serial.println("Demonstration of AccelStepper Library");  //print a messages
  Serial.println("Send 'commands' for printing the commands.");

  //edge of sticker sensor
  pinMode(irSensorPin, INPUT);  // Set IR sensor pin as input
}



void loop() {
  //Constantly looping through these 2 functions.
  //We only use non-blocking commands, so something else (should also be non-blocking) can be done during the movement of the motor
  runAllowed = checkSerial(conveyorMotor, stickerMotor);     //check serial port for new commands
  sensorValue = digitalRead(irSensorPin);      // Read the value from the IR sensor
  
  RunMotor(conveyorMotor, runAllowed);
  RunStickerMotor(stickerMotor, runAllowed, sensorValue);  //function to handle the motor
  
}