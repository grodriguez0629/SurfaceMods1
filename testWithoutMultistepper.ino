#include <AccelStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper1(1, 22, 23); // (Typeof driver: with 2 pins, STEP, DIR) //Front Left
AccelStepper stepper2(1, 26, 27); //Front Right
AccelStepper stepper3(1, 28, 29); //Rear Right
AccelStepper stepper4(1, 24,25); //Rear Left


//Set up is automatically run once the second the code runs
void setup() {
  Serial.begin(9600); //Enable serial
  stepper1.setMaxSpeed(1000); // Set maximum speed value for the stepper
  stepper2.setMaxSpeed(1000);
  stepper3.setMaxSpeed(1000);
  stepper4.setMaxSpeed(1000);

  stepper1.setSpeed(1000);
  stepper2.setSpeed(1000);
  stepper3.setSpeed(1000);
  stepper4.setSpeed(1000);

}


void moveForward(int steps) {
  //Set the movements that need to be done
  stepper1.move(steps); 
  stepper2.move(-steps);
  stepper3.move(-steps); 
  stepper4.move(steps); 
   //While the steppers have not reached their final position
while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {    //run each stepper
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    stepper4.runSpeed();

  }
}

void moveBackward(int steps) {
  //Set the movements that need to be done
  stepper1.move(-steps); 
  stepper2.move(steps);
  stepper3.move(steps); 
  stepper4.move(-steps); 
  //While the steppers have not reached their final position
while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {    //run each stepper
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    stepper4.runSpeed();
  }
}

void turnLeft(int steps) {
  //Set the movements that need to be done
  stepper1.move(-steps); 
  stepper2.move(-steps);
  stepper3.move(-steps); 
  stepper4.move(-steps); 
  //While the steppers have not reached their final position
while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {    //run each stepper
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    stepper4.runSpeed();
  }
}
void turnRight(int steps) {
  //Set the movements that need to be done
  stepper1.move(steps); 
  stepper2.move(steps);
  stepper3.move(steps); 
  stepper4.move(steps); 

  //While the steppers have not reached their final position
  while(stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {
    stepper1.runSpeed();
    stepper2.runSpeed();
    stepper3.runSpeed();
    stepper4.runSpeed();
  }
}



void loop() {
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
    case 'F': //If F, run move forward the number of steps
      Serial.println("Moving forward");
      moveForward(steps);
      break;
    case 'B': //If B, run move backward the number of steps
      Serial.println("Moving backward");
      moveBackward(steps);
      break;
    case 'L': //If L, run turn left the number of steps
      Serial.println("Turning left");
      turnLeft(steps);
      break;
    case 'R': //If R, run turn right the number of steps
      Serial.println("Turning right");
      turnRight(steps);
      break;
    default: //If it doesn't match any of these, print out invalid
      Serial.println("Invalid command.");
      break;
  }
  delay (1000);
}
