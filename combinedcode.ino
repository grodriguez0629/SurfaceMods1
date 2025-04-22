void setup() {
  // put your setup code here, to run once:

}

void loop() {
  String motorMovement;
  while(!Serial.available());
  motorMovement = Serial.readString();
  motorMovement.trim();
  Serial.println(motorMovement);

  if(motorMovement == "DRIVE") {
    
  }
  else if(motorMovement == "AUGER") {

  }
  else if(motorMovement == "SHOULDER") {

  }

}
