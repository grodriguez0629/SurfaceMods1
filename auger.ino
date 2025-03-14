//libs
#include <AccelStepper.h>

//auger stepper
//type, pin1, pin2, pin3, pin4
AccelStepper auger(AccelStepper::driver, 22, 23);

void setup() {
  Serial.begin(9600);
  
  /*
  stepper initialization
  set max speed, in steps/s
  at max, move 90deg in 1s (50 steps/s * 1.8deg/step = 90deg/s)
  */
  auger.setMaxSpeed(1000);
  auger.setAcceleration(500);
}

//moves the auger drill at a desired speed
void dig(AccelStepper stepper, float spd) {
  auger.setSpeed(spd);
  auger.runSpeed();
}

void emergencyStop() {
  auger.stop();
}


void loop() {
  //init variables
  AccelStepper stepper;
  float spd = 100.0;

  //serial input; use via serial monitor (or when i interface it w the le potato)
  string in;
  string cmd;

  //select shoulder to change

  in = Serial.println("Status (On/Off): ");
  while (Serial.available() == 0) {} 
  cmd = Serial.readString();
  cmd.trim();
  if (cmd == "On") {
    dig(stepper, spd);
  }
  else if (cmd == "Off") {
    emergencyStop();
  }
  else {
    Serial.println("ERROR: Invalid command entered.");
  }
}
