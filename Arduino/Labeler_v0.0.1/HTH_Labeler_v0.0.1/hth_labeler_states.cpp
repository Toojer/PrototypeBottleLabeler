#include "hth_labeler_states.h"
#include "AccelStepper.h"
#include <Arduino.h>
#include "run_motor.h"

//This file is for controlling the labeling process state machine.

int stickerEdgeReading = HIGH;
int labelerTriggerReading = HIGH;

LabelerStates startLabeler(AccelStepper &stepper) {
  //This will be the first state the labeler enters when powered on
  //will prime the sticker reel until a sticker gap/edge is detected.
  Serial.println("Starting....");
  stepper.enableOutputs();
  stepper.move(10000);
  while (stepper.distanceToGo() != 9900) {  //Let the sticker motor move a little before starting.
    RunMotor(stepper, true);
  }
  return INITIALIZE;
}

//Initialize - find the first sticker edge, then stop. no conveyor motor.  Next is "Peel"
LabelerStates InitializeLabeler(AccelStepper &stepper, int stickerSensor, LabelerStates PreviousState) {  
  if (PreviousState != INITIALIZE) {
    Serial.println("Initializing Labeler...  Finding Sticker Edge...");
  }
  stickerEdgeReading = digitalRead(stickerSensor);
  RunStickerMotor(stepper, true, stickerEdgeReading);

  if ((stepper.distanceToGo() == 0) || !stickerEdgeReading) {
    return WAIT;
  } else {
    return INITIALIZE;
  }
}

LabelerStates WaitForStimulus(bool labelTrigger, int triggerSensor, LabelerStates PreviousState) {
  if (PreviousState != WAIT) {
    Serial.println("Waiting for Stimulus");
  }
  labelerTriggerReading = digitalRead(triggerSensor);
  if (!labelerTriggerReading) {
    return PEEL;
  } else {
    return WAIT;
  }
}

LabelerStates PeelSticker(AccelStepper &stepper, int stickerSensor, LabelerStates PreviousState)  //When sticker sensor triggered then stop peeling the sticker
{
  if (PreviousState != PEEL) {
    Serial.println("Peeling Sticker to sticker Edge");
    stepper.move(5000);
  }

  while (!stickerEdgeReading) {  //while reading the gaps between stickers the signal will be low
    stickerEdgeReading = digitalRead(stickerSensor);
    RunStickerMotor(stepper, true, stickerEdgeReading);
  }
  Serial.println("Peeled sticker passed sticker edge..");
  return LABEL;
}


LabelerStates LabelBottle(AccelStepper &stickerStepper, AccelStepper &convStepper, int stickerSensor, LabelerStates PreviousState)  //running both the conveyor and the sticker motor until sticker edge sensor is triggered
{
  if (PreviousState != LABEL) {
    Serial.println("Labeling Bottle...");
    stickerStepper.move(5500);
    convStepper.move(10000);
    stickerStepper.enableOutputs();
    convStepper.enableOutputs();
  }

  stickerEdgeReading = digitalRead(stickerSensor);
  RunStickerMotor(stickerStepper, true, stickerEdgeReading);
  RunMotor(convStepper, true);
  if (stickerEdgeReading) {
    return LABEL;
  } else {
    return EXIT;
  }
}

LabelerStates ExitBottle(AccelStepper &stepper, int TriggerSensor, LabelerStates PreviousState)  //Continue the conveyer belt until the bottle labeler is triggered again, or finishes the distance
{
  labelerTriggerReading = digitalRead(TriggerSensor);
  Serial.println("Finishing rolling jar out of conveyor belt");
  if ((stepper.distanceToGo() == 0) || (!labelerTriggerReading)) {
    return PEEL;
  } else {
    return EXIT;
  }
}