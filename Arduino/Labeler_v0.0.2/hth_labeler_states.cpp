#include "hth_labeler_states.h"
#include "AccelStepper.h"
#include <Arduino.h>
#include "run_motor.h"

//This file is for controlling the labeling process state machine.

//int stickerEdgeReading = HIGH;
int labelerTriggerReading = HIGH;


LabelerStates startLabeler(AccelStepper &stickerStepper) {
  //This will be the first state the labeler enters when powered on
  //will prime the sticker reel until a sticker gap/edge is detected.
  Serial.println("Starting Hollow Tree Honey Labeler....");
  stickerStepper.enableOutputs();
  stickerStepper.move(-10000);
  return INITIALIZE;
}

//Initialize - find the first sticker edge, then stop. no conveyor motor.  Next is "Peel"
LabelerStates InitializeLabeler(AccelStepper &stickerStepper, int stickerSensor, LabelerStates PreviousState) {  
  if (PreviousState != INITIALIZE) {
    Serial.println("Initializing Labeler...  Finding Sticker Edge...");
  }
  int stickerEdgeReading = digitalRead(stickerSensor);
  RunStickerMotor(stickerStepper, true, stickerEdgeReading);

  if ((stickerStepper.distanceToGo() == 0) || !stickerEdgeReading) {
    Serial.println("Initialize found sticker edge, now moving to PEEL sticker");
    return PEEL;
  } else {
    return INITIALIZE;
  }
}

LabelerStates WaitForStimulus(AccelStepper &convStepper, int triggerSensor, LabelerStates PreviousState) {
  if (PreviousState != WAIT) {
    Serial.println("Waiting for jar to be detected..");
  }
  labelerTriggerReading = digitalRead(triggerSensor);
  if (!labelerTriggerReading) {
    Serial.println("Labeler Trigger sensed, moving from Wait to Label bottle");
    return LABEL;
  } else {
    RunMotor(convStepper,true); //keep the conveyor belt moving if previously in peel state from Label state
    return WAIT;
  }
}

//there's a problem here where it goes from Exit to this point but doesn't move the sticker roller.
LabelerStates PeelSticker(AccelStepper &stickerStepper, AccelStepper &convStepper, int stickerSensor, LabelerStates PreviousState)  //When sticker sensor triggered then stop peeling the sticker
{
  if (PreviousState != PEEL) {
    Serial.println("Peeling Sticker to not see sticker gap on sensor");
    stickerStepper.move(-5000);
  }
  int peelEdgeReading = false;
  int loop = 0;
  while (!peelEdgeReading) {  //while reading the gaps between stickers the signal will be low
    Serial.println(loop++);
    peelEdgeReading = digitalRead(stickerSensor);
    RunStickerMotor(stickerStepper, true, !peelEdgeReading);
    RunMotor(convStepper, true); //Must keep the conveyor belt moving if this was previously in LABEL state.
  }
  Serial.println("Peeled sticker passed sticker edge.. moving from PEEL to WAIT");
  return WAIT;
}


LabelerStates LabelBottle(AccelStepper &stickerStepper, AccelStepper &convStepper, int stickerSensor, LabelerStates PreviousState)  //running both the conveyor and the sticker motor until sticker edge sensor is triggered
{
  if (PreviousState != LABEL) {
    Serial.println("Labeling Bottle...");
    stickerStepper.move(-5500);
    convStepper.move(-11000);
  }

  int stickerEdgeRead = digitalRead(stickerSensor);
  RunStickerMotor(stickerStepper, true, stickerEdgeRead);
  RunMotor(convStepper, true);
  if (stickerEdgeRead) {
    return LABEL;
  } else {
    return PEEL;
  }
}

//Not Used
LabelerStates ExitBottle(AccelStepper &convStepper, int TriggerSensor, LabelerStates PreviousState)  //Continue the conveyer belt until the bottle labeler is triggered again, or finishes the distance
{
  if (PreviousState != EXIT) {
    Serial.println("Exiting Bottle from conveyor belt.");
  }
  labelerTriggerReading = digitalRead(TriggerSensor);
  if ((convStepper.distanceToGo() == 0) || (!labelerTriggerReading)) {
    return PEEL;
  } else {
    RunMotor(convStepper, true);
    return EXIT;
  }
}