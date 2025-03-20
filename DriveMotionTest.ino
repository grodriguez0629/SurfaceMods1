//include AccelStepper library classes
#include <AccelStepper.h>
#include <MultiStepper.h>

//Name of motor(Type of driver, STEP, DIR)
AccelStepper frontLeft(1, 38, 41); //positionvals[0] step: 38 dir: 41 
AccelStepper rearLeft(1, 34, 36); //positionvals[1] step: 34 dir: 36
AccelStepper frontRight(1, 40, 37); //positionvals[2]
AccelStepper rearRight(1, 33, 32); //positionvals[3]
//Create multi stepper instance
MultiStepper steppersControl;

long positionvals[4] = {0,0,0,0};
//long positionvals[2] = {0,0};

void setBeginVals(AccelStepper &stepper_name) {
  stepper_name.setMaxSpeed(1000);
  stepper_name.setAcceleration(500);
  stepper_name.setCurrentPosition(0);
  steppersControl.addStepper(stepper_name);
  Serial.println("Initializing Motor");
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino is working!");
  //initialize each value
  setBeginVals(frontLeft);
  setBeginVals(rearLeft);
  setBeginVals(frontRight);
  setBeginVals(rearRight);
}


void turnLeft(int steps) {
  //Set the movements that need to be done
  positionvals[0] -= steps;
  positionvals[1] -= steps;
  positionvals[2] += steps;
  positionvals[3] += steps;
  //Execute all of the movements simultaneously
  steppersControl.moveTo(positionvals);
  steppersControl.runSpeedToPosition();
}

void turnRight(int steps) {
  //Set the movements that need to be done
  positionvals[0] += steps;
  positionvals[1] += steps;
  positionvals[2] -= steps;
  positionvals[3] -= steps;
  steppersControl.moveTo(positionvals);
  steppersControl.runSpeedToPosition();
}

void moveForward(int steps) {
  //Set the movements that need to be done
  positionvals[0] += steps;
  positionvals[1] += steps;
  positionvals[2] += steps;
  positionvals[3] += steps;
  steppersControl.moveTo(positionvals);
  steppersControl.runSpeedToPosition();
}

void moveBackward(int steps) {
  //Set the movements that need to be done
  positionvals[0] -= steps;
  positionvals[1] -= steps;
  positionvals[2] -= steps;
  positionvals[3] -= steps;
  steppersControl.moveTo(positionvals);
  steppersControl.runSpeedToPosition();
}

void loop() {
  /* 
  The Nema 17 Motor Moves 1.8 degrees per step. So to turn left or right 90 degrees would be 50 steps.
  Turn left 90 degrees: turnLeft(50);
  Wait half a second: delay(500); 
  */


  //Prompt for user input
  Serial.println("Enter command (F, B, L, R) and steps. (ex: 'F 200'): ");
  //Wait for user input
  while (Serial.available() == 0) {}
  //Read string from command line until newline character
  String cmd = Serial.readStringUntil('\n');
  //Trim out extra characters from string.
  cmd.trim();

  //Save input Values
  char direction = cmd.charAt(0);
  int steps = cmd.substring(2).toInt();

  //Print out user choices
  Serial.print("Direction: ");
  Serial.println(direction);
  Serial.print("Steps: ");
  Serial.println(steps);

  //Switch statement to execute proper movement based on choice.
  switch(direction) {
    case 'F':
      Serial.println("Moving forward");
      moveForward(steps);
      break;
    case 'B':
      Serial.println("Moving backward");
      moveBackward(steps);
      break;
    case 'L':
      Serial.println("Turning left");
      turnLeft(steps);
      break;
    case 'R':
      Serial.println("Turning right");
      turnRight(steps);
      break;
    default:
      Serial.println("Invalid command.");
      break;
  }
  delay (1000);

  /*  
  Test section
  Serial.println("Before move forward");
  moveForward(1000);
  Serial.println("After move forward");
  */
}
