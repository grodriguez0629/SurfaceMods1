//libs
//uses the cytron motor driver library by cytron technologies:
//https://github.com/CytronTechnologies/CytronMotorDriver
#include <CytronMotorDriver.h>

//pins
#define PWM_PIN 44
#define DIR_PIN 45

//auger
//type, pwm, pwm
CytronMD auger(PWM_DIR, PWM_PIN, DIR_PIN);

//spd profile variables
int acc = 0;
int dec = 0;
int maxSpd = 0;
int currSpd = 0;

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
      maxSpd = 10;
      break;

    //high speed
    case 1:
      acc = 1;
      dec = 1;
      maxSpd = 20;
      break;

    //off
    case 3:
      acc = 1;
      dec = 1;
      maxSpd = 0;
      break;
  }
}

//moves the auger drill towards a desired speed
void dig(CytronMD auger, int acc) {
  //checks current speed
  int spd = analogRead(PWM_PIN);

  if(spd < maxSpd) {
    //accelerates by increasing speed by ~2%/0.5ms up to maxSpd
    while(spd < maxSpd) {
      spd += acc;
      auger.setSpeed(spd);
      delay(500);
    }
  }
  else if(spd > maxSpd) {
    //deccelerates by decreasing speed by ~2%/0.5s down to maxSpd
    while(spd < maxSpd) {
      spd -= dec;
      auger.setSpeed(spd);
      delay(500);
    }
  }  
}

void emergencyStop() {
  auger.setSpeed(0);
}


void loop() {
  //serial input; use via serial monitor (or when i interface it w the le potato)
  String in;
  String cmd;

  in = Serial.println("Speed (Low/High/Off): ");
  while (Serial.available() == 0) {} 
  cmd = Serial.readString();
  cmd.trim();
  if (cmd == "Low") {
    spdProfile(0);
  }
  else if (cmd == "High") {
    spdProfile(1);
  }
  else if (cmd == "Off") {
    spdProfile(2);
  }
  else {
    Serial.println("ERROR: Invalid command entered.");
  }

  dig(auger, acc);
}
