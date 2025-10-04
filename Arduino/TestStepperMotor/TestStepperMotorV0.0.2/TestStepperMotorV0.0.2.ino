#include <AccelStepper.h>
AccelStepper stickerMotor(1, 8, 9);// direction Digital 9 (CCW), pulses Digital 8 (CLK)

void setup() {
  Serial.begin(9600); //define baud rate
  Serial.println("Demonstration of AccelStepper Library"); //print a messages
  stickerMotor.setMaxSpeed(400); //SPEED = Steps / second
  stickerMotor.setAcceleration(800); //ACCELERATION = Steps /(second)^2
}

void loop() {
  // put your main code here, to run repeatedly:

}
