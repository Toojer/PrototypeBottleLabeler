#include "WString.h"
#include "HardwareSerial.h"
#include "cmd_line_debug.h"
#include "AccelStepper.h"
#include <Arduino.h>

//Allows user to use debug commands when connected to a serial port.  See PrintCommands() function for details on available commands.
enum Cmds {
  CONVEYOR,
  STOP,
  ACCEL,
  COMMANDS,
  STICKER,
  HEX18,
  UNKNOWN
};

Cmds inputToCmds(String input) {
  input.trim();
  input.toUpperCase();
  Serial.println(input);
  if (input == "CONVEYOR") 
    {Serial.println(CONVEYOR); return CONVEYOR;}
  else if (input == "STOP") 
    {Serial.println(STOP); return STOP;}
  else if (input == "ACCEL") 
    {Serial.println(ACCEL); return ACCEL;}
  else if (input == "COMMANDS") 
    {Serial.println(COMMANDS); return COMMANDS;}
  else if (input == "STICKER") 
    {Serial.println(STICKER); return STICKER;}
  else if (input == "HEX18") 
    {Serial.println(HEX18); return HEX18;}
  else return UNKNOWN;
}


bool checkSerial(AccelStepper &sticker, AccelStepper &conveyor) {  //function for receiving the commands

  //local variables
  bool newData = false;           // booleans for new data from serial, and runallowed flag
  int receivedSteps = 0;         //Number of steps
  int receivedSpeed = 0;         //Steps / second
  int receivedAcceleration = 0;  //Steps / second^2 indicate that there is a new data by setting this bool to true
  bool runallowed = false;
  int directionMultiplier = 1;  // = 1: positive direction, = -1: negative direction
  String receivedCommand = "";

  if (Serial.available() > 0)  //if new data is being sent.
  {
    receivedCommand = Serial.readStringUntil('\n');  // pass the value to the receivedCommand variable

    Serial.print("received command:");
    Serial.println(receivedCommand);
    Cmds c = inputToCmds(receivedCommand);
    newData = true;
    if (newData == true)  //we only enter this long switch-case statement if there is a new command from the computer
    {
      switch (c)  //we check what is the command
      {
        case 0:   //Conveyor belt.
          receivedSteps = getUserInput("Enter in amount of steps...");                                                          //value for the steps
          receivedSpeed = getUserInput("Enter in speed...");                                                          //value for the speed
          directionMultiplier = 1;                                                                   //We define the direction
          Serial.println("Moving conveyor belt.");                                                   //print the action
          runallowed = RotateRelative(conveyor, directionMultiplier, receivedSpeed, receivedSteps);  //Run the function
          //example: P2000 400 - 2000 steps (5 revolution with 400 step/rev microstepping) and 400 steps/s speed
          //In theory, this movement should take 5 seconds
          break;

        case 1:  // Stops the motors

          conveyor.stop();                         //stop motor
          conveyor.disableOutputs();               //disable power
          sticker.stop();                          //stop motor
          sticker.disableOutputs();                //disable power
          Serial.println("Stopped both motors.");  //print action
          runallowed = false;                      //disable running
          break;

        case 2:  // Acceleration Updates

          runallowed = false;         //we still keep running disabled, since we just update a variable
          conveyor.disableOutputs();  //disable power
          sticker.disableOutputs();
          receivedAcceleration = getUserInput("Enter in Acceleration...");;         //receive the acceleration from serial
          conveyor.setAcceleration(receivedAcceleration);  //update the value of the variable
          sticker.setAcceleration(receivedAcceleration);
          Serial.print("New acceleration value: ");  //confirm update by message
          Serial.println(receivedAcceleration);      //confirm update by message
          break;

        case 3:  // Prints Commands

          PrintCommands();  //Print the commands for controlling the motor
          runallowed = true;
          break;

        case 4:   //Sticker motor control.  This will rotate the sticker motor to a relative value
          receivedSteps = Serial.parseFloat();  //value for the steps
          receivedSpeed = Serial.parseFloat();
          Serial.println("Moving sticker motor");
          runallowed = RotateRelative(sticker, 1, receivedSpeed, receivedSteps);

        case 5:                                        //supposed to label the 18 oz Hex jar
          runallowed = DispenseSticker(sticker, 500, 4000);  //control the sticker dispenser
          RotateRelative(conveyor, 1, 500, 10000);           //control the conveyor belt


        default:
          runallowed = true;
          break;
      }
      //after we went through the above tasks, newData is set to false again, so we are ready to receive new commands again.
      newData = false;
      return runallowed;
    }
  } else {
    return true;  //return runallowed if there is no serial input, allowing other stimulants to trigger the motors.
  }
}

bool RotateRelative(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps) {
  //We move X steps from the current position of the stepper motor in a given direction.
  //The direction is determined by the multiplier (+1 or -1)
  Serial.println(receivedSpeed);
  Serial.println(receivedSteps);
  stepper.setMaxSpeed(receivedSpeed);                 //set speed
  stepper.move(directionMultiplier * receivedSteps);  //set relative distance and direction
  return true;                                        //allow running - this allows entering the RunMotor() function.
}

bool RotateAbsolute(AccelStepper &stepper, int directionMultiplier, long receivedSpeed, long receivedSteps) {
  //We move to an absolute position.
  //The AccelStepper library keeps track of the position.
  //The direction is determined by the multiplier (+1 or -1)
  Serial.println(receivedSpeed);
  Serial.println(receivedSteps);
  stepper.setMaxSpeed(receivedSpeed);                   //set speed
  stepper.moveTo(directionMultiplier * receivedSteps);  //set relative distance
  return true;                                          //allow running - this allows entering the RunMotor() function.
}

void PrintCommands() {
  //Printing the commands
  Serial.println(" input command, then user will be prompted to enter appropriate settings with the command");
  Serial.println(" 'commands' : Prints all the commands and their functions.");
  Serial.println(" 'conveyor' : Rotates the conveyor motor.  i.e. conveyor then 1000, then 100  (1000 steps at 100 steps/sec)");
  Serial.println(" 'stop' : Stops the motors immediately. (stop,)");
  Serial.println(" 'accel' : Sets an acceleration value. (accel then 1000)");
  Serial.println(" 'sticker' : adjust the sticker motor location. i.e. sticker then 1000, then 100,  (1000 steps at 100 steps/sec)");
  Serial.println(" 'hexEighteen' : Simulate labeling the Hex 18oz Jar. controls conveyor belt and sticker motor/sensors");
}

bool DispenseSticker(AccelStepper &stepper, long receivedSpeed, long receivedSteps) {
  Serial.println("dispensing sticker");
  stepper.setMaxSpeed(receivedSpeed);  //set speed
  stepper.move(receivedSteps);         //set relative distance and direction
  return true;
}

int getUserInput(String prompt) {
  Serial.println(prompt);

  // Wait for input
  while (true) {
    if (Serial.available()) {
      // Read until newline (user presses Enter)
      String input = Serial.readStringUntil('\n');
      input.trim();  // remove newline and spaces
      if (input.length() > 0) {
        return input.toInt();
      }
    }
  }
}