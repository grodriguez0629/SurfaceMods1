//include AccelStepper library classes
#include <AccelStepper.h>
#include <MultiStepper.h>

//Name of motor(Type of driver, STEP, DIR)
AccelStepper frontLeft(1, 50, 51);
AccelStepper frontRight(1, 52, 53);
AccelStepper rearLeft(1, 54, 55);
AccelStepper rearRight(1, 56, 57);

//Create multi stepper instance
MultiStepper steppersControl;

void setBeginVals(AccelStepper stepper_name) {
  stepper_name.setMaxSpeed(1000);
  stepper_name.setAcceleration(500);
  stepper_name.setCurrentPosition(0);
  steppersControl.addStepper(stepper_name);
}

void setup() {
  //initialize each value
  setBeginVals(frontLeft);
  setBeginVals(frontRight);
  setBeginVals(rearLeft);
  setBeginVals(rearRight);
}


void turnLeft(int steps) {
  //Set the movements that need to be done
  frontLeft.move(-steps);
  rearLeft.move(-steps);
  frontRight.move(steps);
  rearRight.move(steps);
  //Execute all of the movements simultaneously
  steppersControl.runSpeedToPosition();
}

void turnRight(int steps) {
  //Set the movements that need to be done
  frontLeft.move(steps);
  rearLeft.move(steps);
  frontRight.move(-steps);
  rearRight.move(-steps);
  steppersControl.runSpeedToPosition();
}

void moveForward(int steps) {
  //Set the movements that need to be done
  frontLeft.move(steps);
  rearLeft.move(steps);
  frontRight.move(steps);
  rearRight.move(steps);
  steppersControl.runSpeedToPosition();
}

void moveBackward(int steps) {
  //Set the movements that need to be done
  frontLeft.move(-steps);
  rearLeft.move(-steps);
  frontRight.move(-steps);
  rearRight.move(-steps);
  steppersControl.runSpeedToPosition();
}

void emergencyStop() {
  // Immediately stop all motors
  frontLeft.stop();
  frontRight.stop();
  rearLeft.stop();
  rearRight.stop();
}

void loop() {
  //The Nema 17 Motor Moves 1.8 degrees per step. So to turn left or right 90 degrees would be 50 steps.
  //Turn left 90 degrees
  turnLeft(50);
  //Wait half a second
  delay(500);
  //Move forward 100 steps
  moveForward(100);
  delay(500);
  //Turn right 90 degrees
  turnRight(50);
  //Move Backward 100 steps
  moveBackward(100);
  //Can create an if-condition to run the emergencystop command. However, we don't have sensors so TBD on how we implement that
}
