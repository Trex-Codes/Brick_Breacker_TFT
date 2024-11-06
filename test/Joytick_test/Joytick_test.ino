#define JOYSTICK_X_PIN A8

#define JOYSTICK_THRESHOLD 20 // Umbral del joystick
#define NEUTRAL_RANGE_LOW 492 // Límite inferior del rango neutro
#define NEUTRAL_RANGE_HIGH 532 // Límite superior del rango neutro

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int joystickX = analogRead(JOYSTICK_X_PIN); 
  
  if (joystickX >= NEUTRAL_RANGE_LOW && joystickX <= NEUTRAL_RANGE_HIGH) {
    Serial.println("repose"); 
  } else {
    joystickX = map(joystickX, 0, 1023, -512, 512);
    
    Serial.print("X: ");
    
    if (joystickX < 0) {
      Serial.println("negativo");
    } else {
      Serial.println("positivo");
    }
  }

  delay(100); 
}