//CHANGELOG
//v0.5: updated microstep to 32 for more precision and added target rotation (untested)
//v0.4: added changelog and documentation and comments for each variable, definition, and function parameter.
//v0.3: completed code after testing and updating to ensure functionality.
//v0.2: rewritten code to use the HighPowerStepperDriver library by pololu
//v0.1: initial version of code using the AccelStepper library by arduino

//libs
//uses the high power stepper driver arduino library by polulu:
//https://github.com/pololu/high-power-stepper-driver-arduino
#include <HighPowerStepperDriver.h>

//pin outs
//[CS_PIN] defines the chipset pin on the arduino being used; necessary for communication with SPI.
//[STEPPIN] defines the step pin on the arduino being used; tells the motor when to step forward.
//[DIR_PIN] defines the digital pin on the arduino being used; controls the motor's direction.
//since there are 2 different shoulders to control, there are 2 different sets of pins here.
#define CS_PIN1 29
#define STEP_PIN1 30
#define DIR_PIN1 31

#define CS_PIN2 32
#define STEP_PIN2 34
#define DIR_PIN2 35

//definitions that determine direction, step angle, and max turning angles
//mainly used for tracking the current position of the motor.
//[UP] is used to increase the position tracker in the positive direction.
//[DOWN] is used to decrease the position tracker in the negative direction.
//[STEP] defines the stepper angle and is mainly used to convert the step count to degrees for position tracking.
//       note: when changing the microstep resolution, make sure to change this value divided by the resolution.
//             for example, if using HPSDStepMode::MicroStep256, divide 1.8/256 for a step angle of ~0.007deg.
//[MAX_ANGLE] defines the maximum turning angle for the shoulder in degrees.
//[MIN_ANGLE] defines the minimum turning angle for the shoulder in degrees.
#define UP 1
#define DOWN -1

#define STEP 0.05625
#define MAX_ANGLE 90.0
#define MIN_ANGLE 0.0

//steppers labeled for left and right shoulders
//the high power stepper driver library does *not* have constructors,
//so defining them here is the only thing necessary.
HighPowerStepperDriver leftStepper;
HighPowerStepperDriver rightStepper;

//control vars that determines the steppers' current position, measured in degrees
//tracks the current position of the shoulders.
//this WILL reset back to 0 if the power of the device is shut off while turning and would have to be saved elsewhere to prevent over or underturning.
float posLeft = 0;
float posRight = 0;

void setup() {
  //begin SPI and serial communication
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
  rightStepper.setStepMode(HPSDStepMode::MicroStep32);
  rightStepper.enableDriver();
}

//function that allows the motor to step by writing HIGH to the desired pin.
//param [int stepPin] accepts the stepPin you want to write to.
void step(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(2);
}

//function that sets the direction of the motor by writing HIGH or LOW to the desired pin.
//param [int dirPin] accepts the current direction pin you want to write to.
//param [int dir] accepts the current direction the motor should change to.
//                note: this function is reliant on the definitions for UP and DOWN at the top.
//                      it can be changed to accept 0 and 1 for LOW and HIGH respectively, 
//                      rather than using a conditional to read whcih direction was sent in. 
void setDirection(int dirPin, int dir) {
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
//param [char side] accepts either 'L' or 'R', and is used with the serial input to determine which shoulder to move.
//param [int dir] determines which direction the shoulder is moving in
//                note: similarly to setDirection(), this is reliant on the defintions for UP and DOWN at the top.
//                      in this case, it is primarily used to track the position in conjunction with the STEP definition.
void moveShoulder(char side, int dir, int target) {
  int stepPin;

  //checks for the current shoulder being moved and updates the position
  if(side == 'L') {
    stepPin = STEP_PIN1;

    //move one step
    step(stepPin);
    Serial.println("Current position of left shoulder: " + String(posLeft));
    posLeft += STEP*dir;

    while(posLeft > MIN_ANGLE && posLeft < MAX_ANGLE) {
      if(dir == DOWN && posLeft <= target) {
          break; 
      }
      else if(dir == UP && posLeft >= target) {
        break;
      }
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
      if(dir == DOWN && posLeft <= target) {
        break; 
      }
      else if(dir == UP && posLeft >= target) {
        break;
      }
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
  //serial input; use via serial monitor OR a serial communication script interfaced with another board like a raspberry pi
  String input;
  String cmd;

  //select shoulder to change
  //takes in input and trims it to remove terminating characters
  input = Serial.println("Shoulder (L/R): ");
  while (Serial.available() == 0) {}
  cmd = Serial.readString();
  cmd.trim();

  //command direction to move the shoulder in
  //takes in input and trims it to remove terminating characters
  input = Serial.println("Direction (U/D): ");
  while (Serial.available() == 0) {} 
  cmd += Serial.readString();
  cmd.trim();

  input = Serial.println("Target (0-90): ");
  while (Serial.available() == 0) {} 
  cmd += Serial.readString();
  cmd.trim();

  //interprets the given command:
  //L == selects the left shoulder for movement
  //R == selects the right shoulder for movement
  //U == commands the shoulder to move upwards. will send an error if shoulder is currently unable to turn
  //D == commands eh shoulder to move downwards. will send an error if shoulder is currently unable to turn
  if(cmd[0] == 'L') {
    if(cmd[1] == 'U' && posLeft < MAX_ANGLE) {
      setDirection(DIR_PIN1, UP);
      moveShoulder(cmd[0], UP, cmd.substring(2));
    }
    else if(cmd[1] == 'D' && posLeft > MIN_ANGLE) {
      setDirection(DIR_PIN1, DOWN);
      moveShoulder(cmd[0], DOWN, cmd.substring(2));
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else if(cmd[0] == 'R') {
    if(cmd[1] == 'U' && posRight < MAX_ANGLE) {
      setDirection(DIR_PIN2, UP);
      moveShoulder(cmd[0], UP, cmd.substring(2));
    }
    else if(cmd[1] == 'D' && posRight > MIN_ANGLE) {
      setDirection(DIR_PIN2, DOWN);
      moveShoulder(cmd[0], DOWN, cmd.substring(2));
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else {
    Serial.println("ERROR: Invalid command entered. (L/R)");
  }
}

