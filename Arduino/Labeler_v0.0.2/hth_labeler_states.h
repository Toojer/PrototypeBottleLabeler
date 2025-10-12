#ifndef HTH_LABELER_STATES_h
#define HTH_LABELER_STATES_h

//Definition of the LabelerStates of the HTH Labeler
enum LabelerStates {
  START,
  INITIALIZE,
  PEEL,
  LABEL,
  EXIT,
  REMOVE,
  WAIT,
  UNKNOWN,
  NUM_STATES
};

// Create a corresponding array of strings
//extern const char* LabelerStateNames[NUM_STATES] = {"START", "INITIALIZE", "PEEL", "LABEL", "EXIT", "REMOVE", "WAIT", "UNKNOWN"};

#endif

#include "AccelStepper.h"

//This will be the first state the labeler enters when powered on. This will prime the sticker reel to run.
LabelerStates startLabeler(AccelStepper &stepper);

//Initializes the sticker until gap/edge detected.
LabelerStates InitializeLabeler(AccelStepper &stepper, int stickerSensor, LabelerStates PreviousState);

//Waiting for stimulus to signal a bottle is ready to be labeled.
LabelerStates WaitForStimulus(AccelStepper &stepper, int triggerSensor, LabelerStates PreviousState);

//Between ExitBottle LabelerStates, must peel the sticker until edge gap detected, then go to Wait state
LabelerStates PeelSticker(AccelStepper &stickerStepper, AccelStepper &convStepper, int stickerSensor, LabelerStates PreviousState);

//This will signal the sticker is peeled and conveyor belt will start turning to label a bottle.
LabelerStates LabelBottle(AccelStepper &stickerStepper, AccelStepper &convStepper, int stickerSensor, LabelerStates PreviousState);

//When no stimulus received that a new bottle is present, the conveyor belt will continue to roll out the bottles.
LabelerStates ExitBottle(AccelStepper &stepper, int TriggerSensor, LabelerStates PreviousState);
