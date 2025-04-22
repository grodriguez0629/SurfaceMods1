//Map pins for DIR and STEP
#define direction_pin 5
#define stepper_pin 6

void setup() {
  // put your setup code here, to run once:
  pinMode(direction_pin, OUTPUT);
  pinMode(stepper_pin, OUTPUT);

  //Sets the initial direction of the stepper motor (based on where A1, A2, B1, and B2 are connected)
  digitalWrite(direction_pin, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
   
  //Takes 200 steps in the direction specified.
  for(int i = 0; i < 200; i++) {
    digitalWrite(stepper_pin, HIGH);  
    //Controls the speed at which the steps are taken. Higher delay = Slower, Shorter delay = Faster.
    delayMicroseconds(50);
  }
}
