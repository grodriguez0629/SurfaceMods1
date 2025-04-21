const int FLstep = 2;
const int FLdir = 23;
const int FRstep = 4;
const int FRdir = 27;
const int RRstep = 6;
const int RRdir = 29;
const int RLstep = 8;
const int RLdir = 25;

//Set up is automatically run once the second the code runs
void setup() {
  Serial.begin(9600); //Enable serial
  //Setup FL pins
  pinMode(FLstep, OUTPUT);
  pinMode(FLdir, OUTPUT);

  //Setup RL pins
  pinMode(RLstep, OUTPUT);
  pinMode(RLdir, OUTPUT);

  //Setup FR pins
  pinMode(FRstep, OUTPUT);
  pinMode(FRdir, OUTPUT);

  //Setup RR pins
  pinMode(RRstep, OUTPUT);
  pinMode(RRdir, OUTPUT);

  //Set rotational direction
  digitalWrite(FLdir, HIGH);
  digitalWrite(RLdir, HIGH);
  digitalWrite(FRdir, HIGH);
  digitalWrite(RRdir, HIGH);

}


void moveForward(int steps) {
   //While the steppers have not reached their final position
  for(int i = 0; i <= steps; i++) {
    digitalWrite(FLstep, HIGH);
    digitalWrite(RLstep, HIGH);
    digitalWrite(FRstep, HIGH);
    digitalWrite(RRstep, HIGH);
    delayMicroseconds(1000);
    digitalWrite(FLstep, LOW);
    digitalWrite(RLstep, LOW);
    digitalWrite(FRstep, LOW);
    digitalWrite(RRstep, LOW);
    delayMicroseconds(1000);
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
      //moveBackward(steps);
      break;
    case 'L': //If L, run turn left the number of steps
      Serial.println("Turning left");
      //turnLeft(steps);
      break;
    case 'R': //If R, run turn right the number of steps
      Serial.println("Turning right");
      //turnRight(steps);
      break;
    default: //If it doesn't match any of these, print out invalid
      Serial.println("Invalid command.");
      break;
  }
  delay (1000);
}
