#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define NOTE_C4  262  // Define the frequency of the C4 note in Hz
#define NOTE_G3  196  // Define the frequency of the G3 note in Hz
#define NOTE_A3  220  // Define the frequency of the A3 note in Hz
#define NOTE_B3  247  // Define the frequency of the B3 note in Hz

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
#define NOTE_B5 988

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
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_D6 1175
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_G6 1568
#define NOTE_A6 1760
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_D7 2349
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_G7 3136
#define NOTE_A7 3520
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_D8 4699
#define NOTE_E8 5274
#define NOTE_F8 5588
#define NOTE_G8 6272
#define NOTE_A8 7040
#define NOTE_B8 7902

const int MELODY[] = {
  NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};

const int NOTE_DURATIONS[] = {
  500, 500, 500, 500, 500, 500, 1000,
  500, 500, 500, 500, 500, 500, 1000
};

const int MELODY_LENGTH = sizeof(MELODY) / sizeof(MELODY[0]);

// Buzzer pin
const int BUZZER_PIN = 51;

MCUFRIEND_kbv tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

const int BALL_SIZE = 10;
const int BAR_WIDTH = 60;  // Original width of the bar
const int BAR_HEIGHT = 8;  // Height of the bar
const int BAR_Y = 220;     // Vertical position of the bar

const int NUM_BLOCKS = 78;    // Increase the number of blocks
const int BLOCK_WIDTH = 20;   // Reduce the width of the blocks
const int BLOCK_HEIGHT = 10;  // Reduce the height of the blocks
const int BLOCK_SPACING = 5;  // Space between blocks

int ballX, ballY;
int ballVelX = 4;  // Increase the ball speed in X
int ballVelY = 4;  // Increase the ball speed in Y

int barX;  // Horizontal position of the bar

const int JOYSTICK_X_PIN = A8;
const int JOYSTICK_Y_PIN = A9;
const int JOYSTICK_BUTTON_PIN = 53; // Joystick button pin
const int JOYSTICK_THRESHOLD = 50;
struct Block {
  int x, y;
  bool active;
  uint16_t color;  // New field for the block's color
};

Block blocks[NUM_BLOCKS];

int attemptsLeft = 3;  // Remaining attempts before clearing the blocks
int score = 0;         // Variable to store the score

bool gameStarted = false; // Variable to control if the game has started

void setup() {

  tft.reset();
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);

  // Initially, display a black screen with the text "Start" centered
  tft.fillScreen(0x0000);  // Black color

  // Calculate the position to center the "Breakout" text
  String title = "Breakout";
  int textWidth = title.length() * 12; // Approximate width of the "Breakout" text in pixels (each character is roughly 12 pixels wide)
  int textHeight = 16; // Approximate height of the "Breakout" text in pixels
  int startX = (tft.width() - textWidth) / 2; // X position to center horizontally
  int startY = (tft.height() - textHeight) / 2; // Y position to center vertically

  // Display the "Breakout" text centered on the screen
  tft.setTextColor(0xFFFF); // White text color
  tft.setTextSize(2); // Text size
  tft.setCursor(startX, startY);
  tft.print(title);

  // Calculate the position to center the "Press to Start" text
  int pressTextWidth = 90; // Approximate width of the "Press to Start" text in pixels
  int pressTextHeight = 8; // Approximate height of the "Press to Start" text in pixels
  int pressStartX = (tft.width() - pressTextWidth) / 2; // X position to center horizontally
  int pressStartY = startY + textHeight + 10; // Y position below "Start"

  // Display the "Press to Start" text centered on the screen, below "Start"
  tft.setTextColor(0xFFE0); // Yellow text color
  tft.setTextSize(1); // Smaller text size
  tft.setCursor(pressStartX, pressStartY); // Adjust vertical position below "Start"
  tft.print("Press to Start");

  // Set the joystick button pin as input
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // If the game hasn't started and the joystick button is pressed, start the game
  if (!gameStarted && digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
    startGame();
  }

  // If the game has started, run the game code
  if (gameStarted) {
    playGame();
  } else {
    // Display the "Start" text on the screen
    displayStartText();
  }
}

// Función para iniciar el juego
void startGame() {

  tft.fillScreen(0x0000);  // Borra la pantalla
  ballX = tft.width() / 2;
  ballY = tft.height() * 0.75;
  barX = (tft.width() - BAR_WIDTH) / 2;
  generateBlocks();  // Generar bloques inicialmente
  gameStarted = true; // Marca el juego como iniciado
}

// Function to play the game
void playGame() {
  int joystickX = analogRead(JOYSTICK_X_PIN);

  // Move the paddle (bar) with the joystick
  if (joystickX < 512 - JOYSTICK_THRESHOLD) {
    barX -= 5;  // Move the paddle to the left
  } else if (joystickX > 512 + JOYSTICK_THRESHOLD) {
    barX += 5;  // Move the paddle to the right
  }

  // Limit the paddle's boundaries
  if (barX < 0) {
    barX = 0;
  } else if (barX > tft.width() - BAR_WIDTH) {
    barX = tft.width() - BAR_WIDTH;
  }

  // Clear the ball from its previous position
  tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000); // Clear the ball from its previous position

  // Move the ball
  ballX += ballVelX;
  ballY += ballVelY;

  // Draw the paddle
  tft.fillRect(0, BAR_Y, tft.width(), BAR_HEIGHT, 0x0000);   // Clear the previous paddle
  tft.fillRect(barX, BAR_Y, BAR_WIDTH, BAR_HEIGHT, 0xFFE0);  // Draw the paddle in its new position (yellow color)

  // Draw the ball in its new position
  tft.fillCircle(ballX, ballY, BALL_SIZE, 0x7FFF); // Draw the ball in its new position (light green color)

  // Bounce the ball when it reaches the screen edges
  if (ballX - BALL_SIZE / 2 <= 0 || ballX + BALL_SIZE / 2 >= tft.width()) {
    ballVelX = -ballVelX;
  }
  if (ballY - BALL_SIZE / 2 <= 0) {
    ballVelY = -ballVelY;
  }
  if (ballY + BALL_SIZE / 2 >= tft.height()) {
    // Clear the ball from its previous position
    tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000);

    // Reset the ball's position when it touches the bottom of the screen
    ballX = tft.width() / 2;
    ballY = tft.height() * 0.75;
    ballVelX = 4;    // Reset the X velocity
    ballVelY = 4;    // Reset the Y velocity
    attemptsLeft--;  // Decrease the number of remaining attempts

    // Draw a black circle in the ball's previous position
    tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000);
  }

  // Bounce the ball if it hits the paddle
  if (ballY + BALL_SIZE / 2 >= BAR_Y && ballY - BALL_SIZE / 2 <= BAR_Y + BAR_HEIGHT && ballX + BALL_SIZE / 2 >= barX && ballX - BALL_SIZE / 2 <= barX + BAR_WIDTH) {
    ballVelY = -ballVelY;
  }

  // Check for collisions with the blocks
  for (int i = 0; i < NUM_BLOCKS; i++) {
    if (blocks[i].active && ballX + BALL_SIZE / 2 >= blocks[i].x && ballX - BALL_SIZE / 2 <= blocks[i].x + BLOCK_WIDTH && ballY + BALL_SIZE / 2 >= blocks[i].y && ballY - BALL_SIZE / 2 <= blocks[i].y + BLOCK_HEIGHT) {
      blocks[i].active = false;
      tft.fillRect(blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT, 0x0000);  // Clear the block
      ballVelY = -ballVelY;

      score += 5;  // Increase the score for each destroyed block
      // Play a note on the buzzer
      playNote(1000, 100); // Frequency of 1000 Hz, duration of 100 ms
    }
  }

  // If attempts run out, regenerate the blocks
  if (attemptsLeft <= 0) {
    generateBlocks();  // Regenerate blocks
    attemptsLeft = 3;  // Reset the number of attempts
  }

  drawScore();  // Call the function to draw the score

  delay(10);
}

// Function to generate blocks
void generateBlocks() {
  score = 0; // Reset the score to 0 when regenerating blocks

  // Generate random colors for each row
  uint16_t colors[NUM_BLOCKS / 13];
  for (int i = 0; i < NUM_BLOCKS / 13; i++) {
    colors[i] = random(0xFFFF); // Generate a random color for each row
  }

  // Generate blocks
  for (int i = 0; i < NUM_BLOCKS; i++) {
    // Calculate the current row and column
    int row = i / 13;
    int col = i % 13;

    // Calculate the block's position
    int x = col * (BLOCK_WIDTH + BLOCK_SPACING) + BLOCK_SPACING;
    int y = row * (BLOCK_HEIGHT + BLOCK_SPACING) + BLOCK_SPACING * 7;

    // Assign position and other properties to the block
    blocks[i].x = x;
    blocks[i].y = y;
    blocks[i].active = true;

    // Assign the row color to the block
    blocks[i].color = colors[row];

    // Draw the block on the screen
    tft.fillRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, blocks[i].color);
  }
}

void drawScore() {
  // Clear the area where the score is displayed by drawing a black rectangle
  tft.fillRect(10, 10, 50, 20, 0x0000); // Clear a 50x20 area at (10, 10)

  // Set the position where the score will be drawn
  tft.setCursor(10, 10);

  // Set the text color to white and the text size to 2
  tft.setTextColor(0xFFFF);  // White color for the text
  tft.setTextSize(2);        // Text size set to 2 for better visibility

  // Display the current score on the screen
  tft.print(score);  // Print the current score value
}

void displayStartText() {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  static bool showText = true;
  static bool melodyStarted = false;

  // Time interval for blinking (250 ms)
  const unsigned long interval = 250;

  // If the interval time has passed, toggle the text visibility
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Update the previous time

    // Toggle the visibility of the text
    showText = !showText;

    // Calculate the position to center the "Start" text
    int startX = (tft.width() - 60) / 2; // X position to center horizontally
    int startY = (tft.height() - 16) / 2; // Y position to center vertically

    // Clear the area where the "Press to Start" text is shown
    int textWidth = 90; // Approximate width of the "Press to Start" text in pixels
    int textHeight = 8; // Approximate height of the "Press to Start" text in pixels
    int pressStartX = (tft.width() - textWidth) / 2; // X position to center horizontally
    int pressStartY = startY + 16 + 10; // Y position below "Start"
    tft.fillRect(pressStartX, pressStartY, textWidth, textHeight, 0x0000); // Clear the area where the "Press to Start" text is shown

    // Display the text "Karen Riveros y Juan Giraldo" in the bottom-right corner
    String names = "Karen Riveros y Juan Giraldo";
    int textWidth2 = names.length() * 6; // Approximate width of the text in pixels (each character is approximately 6 pixels wide)
    tft.setTextColor(TFT_GREEN); // Text color (green)
    tft.setTextSize(1); // Text size
    tft.setCursor(tft.width() - textWidth2, tft.height() - 10); // Position in the bottom-right corner
    tft.print(names);

    // Display the text "Press to Start" if showText is false
    if (!showText) {
      tft.setTextColor(0xFFE0); // Text color yellow
      tft.setTextSize(1); // Text size
      tft.setCursor(pressStartX, pressStartY);
      tft.print("Press to Start");
    }

    // If the melody has not started yet, start the melody
    if (!melodyStarted) {
      melodyStarted = true; // Mark that the melody has started
      playMelody(); // Start the melody
    }
  }
}

void playMelody() {
  // Repeat the melody 5 times (you can adjust this number as per your preference)
  for (int j = 0; j < 5; j++) {
    // Play the complete melody
    for (int i = 0; i < MELODY_LENGTH; i++) {
      playNote(MELODY[i], 100); // Duration of each note: 100 ms
    }
  }
}

void playNote(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}