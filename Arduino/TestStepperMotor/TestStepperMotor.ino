/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-controls-28byj-48-stepper-motor-using-uln2003-driver
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-button-debounce
 */

// Include the AccelStepper Library
#include <AccelStepper.h>
#include <ezButton.h>

ezButton button(2);  // create ezButton object that attach to pin 2;

// define step constant
#define FULLSTEP 4 //Doing full steps with stepper motor
#define STEPS_PER_REV 32 //Steps in the 28BYJ-48 Stepper Motor
#define GEAR_SET_REDUCTION 64 //gear set reduction in 28BYJ-48 Stepper Motor is 1/64
//The STEP_PER_REVOLUTION value is calculated for the number of steps in motor with gear set reduction from datasheet
#define STEP_PER_REVOLUTION (STEPS_PER_REV*GEAR_SET_REDUCTION)  

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper stickerMotor(FULLSTEP, 11, 9, 10, 8);  //
AccelStepper conveyorMotor(FULLSTEP, 7, 5, 6, 4);

void setup() {
  Serial.begin(9600);
  button.setDebounceTime(80);         // set debounce time to 80 milliseconds
  stickerMotor.setMaxSpeed(600);      // set the maximum speed
  stickerMotor.setAcceleration(200);  // set acceleration
  stickerMotor.setSpeed(200);         // set initial speed
  conveyorMotor.setMaxSpeed(500);     // set the maximum speed
  conveyorMotor.setAcceleration(100); // set acceleration
  conveyorMotor.setSpeed(200);        // set initial speed
}
void loop() {
  button.loop();  // MUST call the loop() function first


  if (button.isPressed())
    Serial.println("The button is pressed");

  if (button.isReleased()) {
    //stickerMotor.stop();  //stop motors
    //conveyorMotor.stop();
    stickerMotor.setCurrentPosition(0);  // set position
    conveyorMotor.setCurrentPosition(0);
    Serial.println("The button is released");
    stickerMotor.moveTo(-STEP_PER_REVOLUTION * 5);
    conveyorMotor.moveTo(STEP_PER_REVOLUTION * 3);
  }
  stickerMotor.run();
  conveyorMotor.run();
  //stickerMotor.run();  // MUST be called in loop() function
}
