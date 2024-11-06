#define JOYSTICK_X_PIN A8

#define JOYSTICK_THRESHOLD 20 // Joystick threshold
#define NEUTRAL_RANGE_LOW 492 // Lower limit of the neutral range
#define NEUTRAL_RANGE_HIGH 532 // Upper limit of the neutral range

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int joystickX = analogRead(JOYSTICK_X_PIN); 
  
  // Check if joystick is in the neutral range
  if (joystickX >= NEUTRAL_RANGE_LOW && joystickX <= NEUTRAL_RANGE_HIGH) {
    Serial.println("rest"); // Joystick is in the neutral position
  } else {
    joystickX = map(joystickX, 0, 1023, -512, 512); // Map joystick value to a range from -512 to 512
    
    Serial.print("X: ");
    
    // Determine whether the joystick is pushed in a negative or positive direction
    if (joystickX < 0) {
      Serial.println("negative");
    } else {
      Serial.println("positive");
    }
  }

  delay(100); // Delay for 100 ms
}