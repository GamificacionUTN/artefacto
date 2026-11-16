// Local variables (pinout) 
int xPin = A0;
int yPin = A1;
int Enter = 2;

// Local variable (store value)
int xVal;
int yVal;
int StatusButton;

void setup() {
  Serial.begin(9600);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(Enter, INPUT_PULLUP);
}

void loop() {
  // Retrieves data from JoyStick
  xVal = analogRead(xPin);
  yVal = analogRead(yPin);
  StatusButton = digitalRead(Enter);

  // Print complete data
  //Serial.print("X: "); Serial.print(xVal);
  //Serial.print(" | Y: "); Serial.print(yVal);
  //Serial.print(" | Enter: "); Serial.println(StatusButton);

  // Print only 
  Serial.println(String(xVal) + ";" + String(yVal));
    
  delay(100);
}
