#include <AccelStepper.h>
//#include "cmd_line_debug.h"
#include "run_motor.h"
#include <ezButton.h>
#include "hth_labeler_states.h"

const char* LabelerStateNames[NUM_STATES] = {"START", "INITIALIZE", "PEEL", "LABEL", "EXIT", "REMOVE", "WAIT", "UNKNOWN"};
int stickerEdgeSensor = HIGH;  //Assign HIGH signaling sticker, not the sticker edge.
int triggerLabeler = HIGH; //Assign HIGH to show button high

bool runAllowed = true;
#define irStickerSensorPin 2                 //Assign pin two as digital input.
#define trigger 3 //Assign pin 1 as digital input.
AccelStepper conveyorMotor(1, 8, 9);  // pulses Digital 8 (CLK), direction Digital 9 (CCW) 
AccelStepper stickerMotor(1, 5, 6);   // pulses Digital 5 (CLK), Direction digital 6 (CCW)
LabelerStates state = START;
LabelerStates previousState = UNKNOWN;

void setup() {
  //Conveyor belt control motor
  conveyorMotor.setMaxSpeed(1500);      //SPEED = Steps / second
  conveyorMotor.setAcceleration(400);  //ACCELERATION = Steps /(second)^2
  conveyorMotor.enableOutputs();   
  //Sticker Motor Control
  stickerMotor.setMaxSpeed(1100);
  stickerMotor.setAcceleration(200);
  stickerMotor.enableOutputs();

  //debug serial if available
  Serial.begin(9600);                                       //define baud rate
  //Serial.println("Demonstration of AccelStepper Library");  //print a messages
  Serial.println("Powering Up..");

  //edge of sticker sensor
  pinMode(irStickerSensorPin, INPUT);  // Set IR sensor pin as input

  //label dispenser trigger
  pinMode(trigger, INPUT); //Stimulus to start the labeler.
}


void loop() {
  //runAllowed = checkSerial(conveyorMotor, stickerMotor);     //check serial port for new commands
  //stickerEdgeSensor = digitalRead(irSensorPin);
  switch (state) {
    case START:
      previousState = START;
      state = startLabeler(stickerMotor);  //starts the motor moving
      break;

    case INITIALIZE:
      state = InitializeLabeler(stickerMotor, irStickerSensorPin, previousState);
      previousState = INITIALIZE;
      break;

    case WAIT:
      state = WaitForStimulus(conveyorMotor, trigger, previousState);
      previousState = WAIT;
      break;

    case PEEL:
      Serial.println("entered peel state");
      state = PeelSticker(stickerMotor, conveyorMotor, irStickerSensorPin,  previousState);
      previousState = PEEL;
      break;

    case LABEL:
      state = LabelBottle(stickerMotor, conveyorMotor, irStickerSensorPin, previousState);
      previousState = LABEL;
      break;

    case EXIT:     
      state = ExitBottle(conveyorMotor, trigger, previousState);
      previousState = EXIT;
      break;

    default:
      previousState = UNKNOWN;
      state = WAIT;
      break;
  }
}

