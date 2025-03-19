//CHANGELOG
//v0.4: added changelog and documentation and comments for each variable, definition, and function parameter.
//v0.3: added direction switching via setDirection() and SWITCH command
//v0.2: rewritten code to use the CytronMotorDriver library by cytron technologies
//v0.1: initial version of code using the AccelStepper library by arduino

//libs
//this code uses the cytron motor driver library by cytron technologies:
//https://github.com/CytronTechnologies/CytronMotorDriver
#include <CytronMotorDriver.h>

//pins
//[PWM_PIN] defines the pwm pin on the arduino being used; controls the motor's speed
//[DIR_PIN] defines the digital pin on the arduino being used; controls the motor's direction
#define PWM_PIN 44
#define DIR_PIN 42

//auger
//type, pwm pin, dir pin
//pwm pin and dir pin can be changed via the definitions at the top.
CytronMD myAuger(PWM_DIR, PWM_PIN, DIR_PIN);

//spd profile variables
//the [ACCEL_DELAY] definition is used to control the delay between each acceleration iteration.
//[int acc] is the acceleration value of the motor; each iteration will add this value to the current speed
//[int dec] is the decceleration of the motor; each iteration will subtract this value from the current speed 
//[int maxSpd] is the maximum speed the motor is allowed to turn
//[int currSpd] is the current speed of the motor.
//              note: library functions accept only up to 255; for all purposes, currSpd and maxSpd should be no greater than 255.
//[String currProf] is the current speed profile being used by the motor
//                  note: does not do anything in code; is used for debugging and serial monitor line commands.
//[String currDir] is the current spinning direction of the motor.
//                 note: only use values of "CW" or "CCW". not necessary if motor being used is not bidirectional
//                       on a bidirectional motor, negative speeds will correspond to spinning in the opposite direction.
#define ACCEL_DELAY 300
int acc = 0;
int dec = 0;
int maxSpd = 0;
int currSpd = 0;
String currProf = "OFF";
String currDir = "CW";

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  spdProfile(3);
}

//allow the user to select a speed profile
//param [int prof] will current only accept a value from 0-3. anything out of range will return with an error and continue to run the program
//more speed profiles can be added as necessary.
//each speed profile has its own acceleration (int acc), decceleration (int dec), and max speed (int maxSpd) and can be modified as necessary.
//
//speed profile 0 (low): acc/dec == 1, maxSpd == 20; 20 is the threshold required to start moving the motor.
//speed profile 1 (high): acc/dec == 1, maxSpd == 40
//speed profile 2 (off): acc/dec == 1, maxSpd == 0; will deccelerate motor to a halt.
//speed profile 3 (emergenct stop): acc == 0, dec == 255, maxSpd == 0; will instantly stop the motor.
void spdProfile(int prof) {
  switch(prof) {
    //low speed
    case 0:
      acc = 1;
      dec = 1;
      maxSpd = 20;
      break;
    //high speed
    case 1:
      acc = 1;
      dec = 1;
      maxSpd = 40;
      break;
    //off
    case 2:
      acc = 1;
      dec = 1;
      maxSpd = 0;
      break;
    //emergency stop
    case 3:
      acc = 0;
      dec = 0;
      maxSpd = 0;
      break;
    default:
      Serial.println("ERROR: Invalid speed profile selected.");
  }
}

//switches the direction of the motor to either clockwise or counterclockwise
//param [String dir] will accept either "CW" or "CCW". other values will not do anything.
//will only switch directions if currently capable, and will not switch if the max speed is set incorrectly.
void setDirection(String dir) {
  if(dir == "CW" && maxSpd < 0) {
    maxSpd *= -1;
  }
  else if(dir == "CCW" && maxSpd > 0) {
    maxSpd *= -1;
  }
  else {
    Serial.println("ERROR: Invalid direction specified.");
  }
}

//moves the auger drill towards a desired speed
//param [CytronMD auger] is a given auger you want to set the speed for.
//param [int acc] is the acceleration value. will add or subtract from the speed for each iteration while increasing or decreasing speed.
//                note: acceleration is also reliant on the delay, which can be found in the definitions at the top.
//                      a longer delay will decrease the acceleration speed. 
//                      additionally, commands cannot be input while the motor is accelerating or deccelerating due to this occuring in a loop.
void dig(CytronMD auger, int acc) {
  setDirection(currDir);
  //checks current speed
  int spd = currSpd;
  Serial.println("Current speed: " + String(spd));
  Serial.println("Target speed: " + String(maxSpd));

  if(spd < maxSpd) {
    //accelerates by increasing speed by ~2%/0.3ms up to maxSpd
    while(spd < maxSpd) {
      spd += acc;
      currSpd = spd;
      auger.setSpeed(spd);
      delay(ACCEL_DELAY);
    }
  }
  else if(spd > maxSpd) {
    //deccelerates by decreasing speed by ~2%/0.3s down to maxSpd
    while(spd > maxSpd) {
      spd -= dec;
      currSpd = spd;
      auger.setSpeed(spd);
      delay(ACCEL_DELAY);
    }
  }
  else {
    Serial.println("No change in speed.");
  }

  Serial.println("Finished altering speed.");
}

void emergencyStop(CytronMD auger) {
  auger.setSpeed(0);
  while(true) {} //do nothing 
}

void loop() {
  //serial input; use via serial monitor OR a serial communication script interfaced with another board like a raspberry pi
  String in;
  String cmd;

  //displays available commands as well as the current values or options for each command.
  Serial.println("COMMANDS");
  Serial.println("=================");
  Serial.println("Speed: LOW/HIGH/OFF [CURRENT: " + currProf + "]");
  Serial.println("Direction: SWITCH [CURRENT: " + currDir + "]");
  Serial.println("Emergency Stop: ESTOP");

  //takes in input and trims it to remove terminating characters
  in = Serial.println("");
  while (Serial.available() == 0) {} 
  cmd = Serial.readString();
  cmd.trim();

  //interprets the given command:
  //LOW == accelerates auger towards the low speed setting (refer to speed profile)
  //HIGH == accelerates auger towards the high speed setting (refer to speed profile)
  //OFF == deccelerates motor down to a stop
  //SWITCH == will switch the current direction of the auger between CW (clockwise) and CCW (counter-clockwise)
  //ESTOP == emergency stop. will immediately halt the motor and stop the program from running by leaving it in a loop
  //if none of these commands are given, an error will printed in the serial monitor and the program will continue running like usual.
  if(cmd == "LOW") {
    spdProfile(0);
  }
  else if(cmd == "HIGH") {
    spdProfile(1);
  }
  else if(cmd == "OFF") {
    spdProfile(2);
  }
  else if(cmd == "SWITCH") {
    if(currDir == "CW") {
      currDir = "CCW";
      setDirection("CCW");
    }
    else {
      currDir = "CW";
      setDirection("CW");
    }

    Serial.println("Switching to " + currDir);
  }
  else if(cmd == "ESTOP") {
    spdProfile(3);
    emergencyStop(myAuger);
    return;
  }
  else {
    Serial.println("ERROR: Invalid command entered.");
  }

  //function call that allows the motor to spin.
  dig(myAuger, acc);
}
