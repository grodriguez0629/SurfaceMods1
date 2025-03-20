#define stepPin 34
#define dirPin 36


void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(dirPin,HIGH);
  Serial.println("Starting loop...");
  for(int x = 0; x < 800; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(700);    // by changing this time delay between the steps we can change the rotation speed
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(700); 
    Serial.println("Looping...");
  }
  Serial.println("Loop finished...");
}
