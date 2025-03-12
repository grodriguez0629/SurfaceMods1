//libs
#include <AccelStepper.h>

//definitions that determine direction, step angle, and max turning angles
#define UP 1     //+ val moves stepper cw
#define DOWN -1  //- val moves stepper ccw

#define STEP 1.8     //stepper angle = 1.8deg. mainly used for converting steps -> deg
#define MAX_ANGLE 90.0 //max angle allowed to turn = 90deg
#define MIN_ANGLE 0.0  //min angle allowed to turn = 0deg

//steppers labeled for left and right shoulders
//type, pin1, pin2, pin3, pin4
AccelStepper leftStepper(AccelStepper::FULL4WIRE, 22, 23, 24, 25);
AccelStepper rightStepper(AccelStepper::FULL4WIRE, 28, 29, 30, 31);

//control var that determines the steppers' current position
//position is in steps; consider that 50 steps = 90deg
float posLeft = 0;
float posRight = 0;
bool left;

/*
additional stepper info
4 wire full stepper
1.8deg stepper angle (200 steps/rev)
*/

void setup() {
  Serial.begin(9600);
  
  /*
  stepper initialization
  set max speed, in steps/s
  at max, move 90deg in 1s (50 steps/s * 1.8deg/step = 90deg/s)
  */
  leftStepper.setMaxSpeed(50);
  rightStepper.setMaxSpeed(50);

  //note: no acceleration being set means it'll just run at a constant speed.
}

//moves the shoulder of the selected stepper, at a desired spd, in the direction given (up=1, down=-1)
void moveShoulder(AccelStepper &stepper, float spd, int dir) {
  //checks for the current shoulder being moved and updates the position
  //also determines whether to keep moving; should auto stop when <= 0deg or >= 90deg to prevent overturning
  if(left) {
    Serial.println("Current position of left shoulder: " + String(posLeft));
    //move one step up or down
    stepper.move(STEP*dir);
    stepper.run();
    posLeft = stepper.currentPosition() * STEP;

    while(posLeft > MIN_ANGLE && posLeft < MAX_ANGLE) {
      stepper.setSpeed(spd*dir);
      stepper.runSpeed();

      //converts the position in steps to degrees
      posLeft = stepper.currentPosition() * STEP;
      Serial.println("Current position of left shoulder: " + String(posLeft));
    }

    //if the stepper is turned at 0 or 90deg, immediately stop turning
    Serial.println("Stopping movement!");
    stepper.stop();
  }
  else {
    Serial.println("Current position of right shoulder: " + String(posRight));
    //move one step up or down
    stepper.move(STEP*dir);
    stepper.run();
    posRight = stepper.currentPosition() * STEP;

    while(posRight > MIN_ANGLE && posRight < MAX_ANGLE) {
      stepper.setSpeed(spd*dir);
      stepper.runSpeed();

      //converts the position in steps to degrees
      posRight = stepper.currentPosition() * STEP;
      Serial.println("Current position of right shoulder: " + String(posRight));
    }

    //if the stepper is turned at 0 or 90deg, immediately stop turning
    Serial.println("Stopping movement!");
    stepper.stop();
  }
}

void emergencyStop() {
  leftStepper.stop();
  rightStepper.stop();

  posLeft = leftStepper.currentPosition() * STEP;
  posRight = rightStepper.currentPosition() * STEP;
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
    left = true;

    if(cmd[1] == 'U' && posLeft < MAX_ANGLE) {
      moveShoulder(leftStepper, spd, UP);
    }
    else if(cmd[1] == 'D' && posLeft > MIN_ANGLE) {
      moveShoulder(leftStepper, spd, DOWN);
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else if(cmd[0] == 'R') {
    left = false;

    if(cmd[1] == 'U' && posRight < MAX_ANGLE) {
      moveShoulder(rightStepper, spd, UP);
    }
    else if(cmd[1] == 'D' && posRight > MIN_ANGLE) {
      moveShoulder(rightStepper, spd, DOWN);
    }
    else {
      Serial.println("ERROR: Invalid command entered. (U/D)");
    }
  }
  else {
    Serial.println("ERROR: Invalid command entered. (L/R)");
  }
}
