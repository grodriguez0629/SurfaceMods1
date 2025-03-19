//libs
//uses the cytron motor driver library by cytron technologies:
//https://github.com/CytronTechnologies/CytronMotorDriver
#include <CytronMotorDriver.h>

//pins
#define PWM_PIN 44
#define DIR_PIN 42

//auger
//type, pwm pin, dir pin
CytronMD auger(PWM_DIR, PWM_PIN, DIR_PIN);

//direction definitions
#define CW 1
#define CCW -1

//spd profile variables
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
  }
}

//moves the auger drill towards a desired speed
void dig(CytronMD auger, int acc) {
  //checks current speed
  int spd = currSpd;
  Serial.println("Current speed: " + String(spd));
  Serial.println("Target speed: " + String(maxSpd));

  if(spd < maxSpd) {
    //accelerates by increasing speed by ~2%/0.5ms up to maxSpd
    while(spd < maxSpd) {
      spd += acc;
      currSpd = spd;
      auger.setSpeed(spd);
      delay(500);
    }
  }
  else if(spd > maxSpd) {
    //deccelerates by decreasing speed by ~2%/0.5s down to maxSpd
    while(spd > maxSpd) {
      spd -= dec;
      currSpd = spd;
      auger.setSpeed(spd);
      delay(500);
    }
  }
  else {
    Serial.println("No change in speed.");
  }

  Serial.println("Finished altering speed.");
}

void emergencyStop() {
  auger.setSpeed(0);
}


void loop() {
  //serial input; use via serial monitor (or when i interface it w the le potato)
  String in;
  String cmd;

  Serial.println("COMMANDS");
  Serial.println("=================");
  Serial.println("Speed: LOW/HIGH/OFF [CURRENT: " + currProf);
  Serial.println("Direction: SWITCH [CURRENT: " + currDir);

  in = Serial.println("");
  while (Serial.available() == 0) {} 
  cmd = Serial.readString();
  cmd.trim();
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
    maxSpd *= -1;
  }
  else {
    Serial.println("ERROR: Invalid command entered.");
  }

  dig(auger, acc);
}
