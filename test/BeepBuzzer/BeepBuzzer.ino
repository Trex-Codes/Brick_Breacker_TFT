#define BUZZER_PIN 51 // Buzzer pin on Arduino Mega

// Define musical notes and their frequencies in Hz
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

// Structure to represent a note with duration
struct Note {
  int pitch;
  int duration;
};

// Function to play a note with a specific duration
void playNote(int note, int duration) {
  tone(BUZZER_PIN, note, duration);
  delay(duration + 10); // Wait until the note finishes before moving to the next one
}

// Function to play a sequence of notes
void playMelody(Note melody[], int length) {
  for (int i = 0; i < length; i++) {
    playNote(melody[i].pitch, melody[i].duration);
  }
}

// Setup function
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

// Main loop function
void loop() {
    // Example: play a sequence of notes
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
    
    // Calculate the length of the melody
    int length = sizeof(melody) / sizeof(melody[0]);
    
    // Play the melody
    playMelody(melody, length);
  
  // Do some other task in the main loop if necessary
}