#define BUZZER_PIN 51 // Pin del buzzer en Arduino Mega

// Define las notas musicales y sus frecuencias en Hz
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880

// Estructura para representar una nota con duración
struct Note {
  int pitch;
  int duration;
};

// Función para reproducir una nota con una duración específica
void playNote(int note, int duration) {
  tone(BUZZER_PIN, note, duration);
  delay(duration + 10); // Espera hasta que termine la nota antes de pasar a la siguiente
}

// Función para reproducir una secuencia de notas
void playMelody(Note melody[], int length) {
  for (int i = 0; i < length; i++) {
    playNote(melody[i].pitch, melody[i].duration);
  }
}

// Función principal
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

// Función de bucle principal
void loop() {
    // Ejemplo: tocar una secuencia de notas
    Note melody[] = {
      {NOTE_C4, 500},
      {NOTE_D4, 500},
      {NOTE_E4, 500},
      {NOTE_F4, 500},
      {NOTE_G4, 500},
      {NOTE_A4, 500},
      {NOTE_B4, 500},
      {NOTE_C5, 500}
    };
    
    // Calcular la longitud de la melodía
    int length = sizeof(melody) / sizeof(melody[0]);
    
    // Reproducir la melodía
    playMelody(melody, length);
  
  // Hacer alguna otra tarea en el bucle principal si es necesario
}