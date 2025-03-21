#include <AccelStepper.h>
#include <MultiStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 34, 36); // (Typeof driver: with 2 pins, STEP, DIR)
AccelStepper stepper2(1, 35, 33);
AccelStepper stepper3(1, 32, 31);

long gotoposition[3]; // An array to store the target positions for each stepper motor

void setup() {
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000); // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(1000);
  stepper3.setMaxSpeed(1000);

  // Adding the 3 steppers to the steppersControl instance for multi stepper control
}



void moveForward(int steps) {
  //Set the movements that need to be done
  stepper1.move(steps);  // 800 steps - full rotation with quater-step resolution
  stepper2.move(steps);
  stepper3.move(steps); 
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
}

void moveBackward(int steps) {
  //Set the movements that need to be done
  stepper1.move(-steps);  // 800 steps - full rotation with quater-step resolution
  stepper2.move(-steps);
  stepper3.move(-steps); 
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
}

void turnLeft(int steps) {
  //Set the movements that need to be done
  stepper1.move(steps);  // 800 steps - full rotation with quater-step resolution
  stepper2.move(steps);
  stepper3.move(steps); 
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
}

void turnRight(int steps) {
  //Set the movements that need to be done
  stepper1.move(steps);  // 800 steps - full rotation with quater-step resolution
  stepper2.move(steps);
  stepper3.move(steps); 
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0) {
    stepper1.run();
    stepper2.run();
    stepper3.run();
  }
}



void loop() {
  // Store the target positions in the "gotopostion" array
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
}
