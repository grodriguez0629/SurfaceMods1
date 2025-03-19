//libs
//uses the high power stepper driver arduino library by polulu:
//https://github.com/pololu/high-power-stepper-driver-arduino
#include <HighPowerStepperDriver.h>

//pin outs
#define CS_PIN1 29
#define STEP_PIN1 30
#define DIR_PIN1 31

#define CS_PIN2 32
#define STEP_PIN2 34
#define DIR_PIN2 35

//definitions that determine direction, step angle, and max turning angles
#define UP 1
#define DOWN -1

#define STEP 1.8     //stepper angle = 1.8deg. mainly used for converting steps -> deg
#define MAX_ANGLE 90.0 //max angle allowed to turn = 90deg
#define MIN_ANGLE 0.0  //min angle allowed to turn = 0deg

//steppers labeled for left and right shoulders
//type, pin1, pin2, pin3, pin4
HighPowerStepperDriver leftStepper;
HighPowerStepperDriver rightStepper;

//control vars that determines the steppers' current position, measured in degrees
float posLeft = 0;
float posRight = 0;

/*
additional stepper info
4 wire full stepper
1.8deg stepper angle (200 steps/rev)
*/

void setup() {
  SPI.begin();
  Serial.begin(9600);

  leftStepper.setChipSelectPin(CS_PIN1);
  rightStepper.setChipSelectPin(CS_PIN2);
  
  //pin initialization
  pinMode(CS_PIN1, OUTPUT);
  pinMode(DIR_PIN1, OUTPUT);
  pinMode(CS_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  digitalWrite(CS_PIN1, LOW);
  digitalWrite(DIR_PIN1, LOW);
  digitalWrite(CS_PIN2, LOW);
  digitalWrite(DIR_PIN2, LOW);

  //stepper initialization
  leftStepper.resetSettings();
  leftStepper.clearStatus();
  leftStepper.setDecayMode(HPSDDecayMode::AutoMixed);
  leftStepper.setCurrentMilliamps36v8(4000);
  leftStepper.setStepMode(HPSDStepMode::MicroStep1);
  leftStepper.enableDriver();

  rightStepper.resetSettings();
  rightStepper.clearStatus();
  rightStepper.setDecayMode(HPSDDecayMode::AutoMixed);
  rightStepper.setCurrentMilliamps36v8(4000);
  rightStepper.setStepMode(HPSDStepMode::MicroStep1);
  rightStepper.enableDriver();
}

void step(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(2);
}

void setDirection(int dirPin, int dir) {
  Serial.println(dirPin);
  delayMicroseconds(1);
  if(dir == UP) {
    Serial.println("MOVING UP");
    digitalWrite(dirPin, HIGH);
  }
  else if(dir == DOWN) {
    Serial.println("MOVING DOWN");
    digitalWrite(dirPin, LOW);
  }
  delayMicroseconds(1);
}

//moves the shoulder of the selected stepper, at a desired spd
void moveShoulder(char side, int dir) {
  int stepPin;

  //checks for the current shoulder being moved and updates the position
  if(side == 'L') {
    stepPin = STEP_PIN1;

    //move one step
    step(stepPin);
    Serial.println("Current position of left shoulder: " + String(posLeft));
    posLeft += STEP*dir;

    while(posLeft > MIN_ANGLE && posLeft < MAX_ANGLE) {
      step(stepPin);

      //for each step, add 1.8deg to the current position
      posLeft += STEP*dir;
      Serial.println("Current position of left shoulder: " + String(posLeft));
    }
  }
  else {
    stepPin = STEP_PIN2;

    //move one step
    step(stepPin);
    Serial.println("Current position of right shoulder: " + String(posRight));
    posRight += STEP*dir;

    while(posRight > MIN_ANGLE && posRight < MAX_ANGLE) {
      step(stepPin);

      //for each step, add 1.8deg to the current position
      posRight += STEP*dir;
      Serial.println("Current position of right shoulder: " + String(posRight));
    }
  }

  //if the stepper is turned at 0 or 90deg, immediately stop turning
  Serial.println("Stopping movement!");
  digitalWrite(stepPin, LOW);
}

void emergencyStop() {
  digitalWrite(STEP_PIN1, LOW);
  digitalWrite(STEP_PIN2, LOW);
}


void loop() {
  //init variables
  float spd = 10.0;

  //serial input; use via serial monitor (or when i interface it w the le potato)
  String input;
  String cmd;

  //select shoulder to change
  input = Serial.println("Shoulder (L/R): ");
  while (Serial.available() == 0) {}
  cmd = Serial.readString();
  cmd.trim();

  input = Serial.println("Direction (U/D): ");
  while (Serial.available() == 0) {} 
  cmd += Serial.readString();
  cmd.trim();

  /*
  debug
  Serial.println(input);
  Serial.println(cmd);
  */

  //this code is pretty clunky. i'll probably clean it up later
  //like you should just be able to take it in as bytes instead, which is probably better for sensors and stuff,
  //but as of right now testing it with the serial monitor like this will make it easier to ensure the functions work
  if(cmd[0] == 'L') {
    if(cmd[1] == 'U' && posLeft < MAX_ANGLE) {
      setDirection(DIR_PIN1, UP);
      moveShoulder(cmd[0], UP);
    }
    else if(cmd[1] == 'D' && posLeft > MIN_ANGLE) {
      setDirection(DIR_PIN1, DOWN);
      moveShoulder(cmd[0], DOWN);
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else if(cmd[0] == 'R') {
    if(cmd[1] == 'U' && posRight < MAX_ANGLE) {
      setDirection(DIR_PIN2, UP);
      moveShoulder(cmd[0], UP);
    }
    else if(cmd[1] == 'D' && posRight > MIN_ANGLE) {
      setDirection(DIR_PIN2, DOWN);
      moveShoulder(cmd[0], DOWN);
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else {
    Serial.println("ERROR: Invalid command entered. (L/R)");
  }
}

