/*
    Controlling multiple steppers with the AccelStepper and MultiStepper library

     by Dejan, https://howtomechatronics.com
*/

#include <AccelStepper.h>
#include <MultiStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 34, 36); // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, , );
AccelStepper stepper3(1, 4, 7);

MultiStepper steppersControl;  // Create instance of MultiStepper

long gotoposition[3]; // An array to store the target positions for each stepper motor

void setup() {

  stepper1.setMaxSpeed(1000); // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(2000);
  stepper3.setMaxSpeed(1000);

  // Adding the 3 steppers to the steppersControl instance for multi stepper control
  steppersControl.addStepper(stepper1);
  steppersControl.addStepper(stepper2);
  steppersControl.addStepper(stepper3);
}



void moveForward(int steps) {
  //Set the movements that need to be done
  gotoposition[0] += steps;  // 800 steps - full rotation with quater-step resolution
  gotoposition[1] += steps;
  gotoposition[2] += steps;

  steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
  steppersControl.run(); // Blocks until all steppers are in position
}


void loop() {
  // Store the target positions in the "gotopostion" array
  moveForward(800);
/*
  gotoposition[0] += 800;  // 800 steps - full rotation with quater-step resolution
  gotoposition[1] += 1600;
  gotoposition[2] += 3200;

  steppersControl.moveTo(gotoposition); // Calculates the required speed for all motors
  steppersControl.runSpeedToPosition(); // Blocks until all steppers are in position
*/

}
