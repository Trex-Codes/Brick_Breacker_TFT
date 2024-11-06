#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define NOTE_C4  262  // Definir la frecuencia de la nota C4 en Hz
#define NOTE_G3  196  // Definir la frecuencia de la nota G3 en Hz
#define NOTE_A3  220  // Definir la frecuencia de la nota A3 en Hz
#define NOTE_B3  247  // Definir la frecuencia de la nota B3 en Hz

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
const int BAR_WIDTH = 60;  // Ancho original de la barra
const int BAR_HEIGHT = 8;  // Altura de la barra
const int BAR_Y = 220;     // Posición vertical de la barra

const int NUM_BLOCKS = 78;    // Aumenta el número de bloques
const int BLOCK_WIDTH = 20;   // Reducir el ancho de los bloques
const int BLOCK_HEIGHT = 10;  // Reducir la altura de los bloques
const int BLOCK_SPACING = 5;  // Espacio entre bloques

int ballX, ballY;
int ballVelX = 4;  // Incrementar la velocidad de la pelota en X
int ballVelY = 4;  // Incrementar la velocidad de la pelota en Y

int barX;  // Posición horizontal de la barra

const int JOYSTICK_X_PIN = A8;
const int JOYSTICK_Y_PIN = A9;
const int JOYSTICK_BUTTON_PIN = 53; // Pin del botón del joystick
const int JOYSTICK_THRESHOLD = 50;

struct Block {
  int x, y;
  bool active;
  uint16_t color;  // Nuevo campo para el color del bloque
};

Block blocks[NUM_BLOCKS];

int attemptsLeft = 3;  // Intentos restantes antes de borrar los bloques
int score = 0;         // Variable para almacenar el puntaje

bool gameStarted = false; // Variable para controlar si el juego ha comenzado

void setup() {
  tft.reset();
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);

  // Inicialmente, muestra una pantalla negra con el texto "Start" centrado
  tft.fillScreen(0x0000);  // Color negro

  // Calcula la posición para centrar el texto "Breakout"
  String title = "Breakout";
  int textWidth = title.length() * 12; // Ancho aproximado del texto "Breakout" en píxeles (cada caracter tiene un ancho aproximado de 12 píxeles)
  int textHeight = 16; // Altura aproximada del texto "Breakout" en píxeles
  int startX = (tft.width() - textWidth) / 2; // Posición X para centrar horizontalmente
  int startY = (tft.height() - textHeight) / 2; // Posición Y para centrar verticalmente

  // Muestra el texto "Breakout" centrado en la pantalla
  tft.setTextColor(0xFFFF); // Color del texto blanco
  tft.setTextSize(2); // Tamaño del texto
  tft.setCursor(startX, startY);
  tft.print(title);

  // Calcula la posición para centrar el texto "Press to Start"
  int pressTextWidth = 90; // Ancho aproximado del texto "Press to Start" en píxeles
  int pressTextHeight = 8; // Altura aproximada del texto "Press to Start" en píxeles
  int pressStartX = (tft.width() - pressTextWidth) / 2; // Posición X para centrar horizontalmente
  int pressStartY = startY + textHeight + 10; // Posición Y debajo de "Start"

  // Muestra el texto "Press to Start" centrado en la pantalla, debajo de "Start"
  tft.setTextColor(0xFFE0); // Color del texto amarillo
  tft.setTextSize(1); // Tamaño del texto más pequeño
  tft.setCursor(pressStartX, pressStartY); // Ajusta la posición vertical debajo de "Start"
  tft.print("Press to Start");

  // Configura el pin del botón del joystick como entrada
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Si el juego no ha empezado y se presiona el botón del joystick, inicia el juego
  if (!gameStarted && digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {
    startGame();
  }

  // Si el juego ha empezado, ejecuta el código del juego
  if (gameStarted) {
    playGame();
  } else {
    // Muestra el texto "Start" en la pantalla
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

// Función para jugar el juego
void playGame() {
  int joystickX = analogRead(JOYSTICK_X_PIN);

  // Mover la barra (paleta) con el joystick
  if (joystickX < 512 - JOYSTICK_THRESHOLD) {
    barX -= 5;  // Mover la barra hacia la izquierda
  } else if (joystickX > 512 + JOYSTICK_THRESHOLD) {
    barX += 5;  // Mover la barra hacia la derecha
  }

  // Limitar los límites de la barra
  if (barX < 0) {
    barX = 0;
  } else if (barX > tft.width() - BAR_WIDTH) {
    barX = tft.width() - BAR_WIDTH;
  }

  // Borra la pelota en su posición anterior
  tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000); // Borra la pelota en su posición anterior

  // Mover la pelota
  ballX += ballVelX;
  ballY += ballVelY;

  // Dibujar la barra
  tft.fillRect(0, BAR_Y, tft.width(), BAR_HEIGHT, 0x0000);   // Borra la barra anterior
  tft.fillRect(barX, BAR_Y, BAR_WIDTH, BAR_HEIGHT, 0xFFE0);  // Dibuja la barra en su nueva posición (color amarillo)

  // Dibujar la pelota en su nueva posición
  tft.fillCircle(ballX, ballY, BALL_SIZE, 0x7FFF); // Dibuja la pelota en su nueva posición (color lightgreen)

  // Rebotar la pelota cuando alcanza los límites de la pantalla
  if (ballX - BALL_SIZE / 2 <= 0 || ballX + BALL_SIZE / 2 >= tft.width()) {
    ballVelX = -ballVelX;
  }
  if (ballY - BALL_SIZE / 2 <= 0) {
    ballVelY = -ballVelY;
  }
  if (ballY + BALL_SIZE / 2 >= tft.height()) {
    // Borra la pelota en su posición anterior
    tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000);

    // Resetear la posición de la pelota cuando toca la parte inferior de la pantalla
    ballX = tft.width() / 2;
    ballY = tft.height() * 0.75;
    ballVelX = 4;    // Reiniciar la velocidad en X
    ballVelY = 4;    // Reiniciar la velocidad en Y
    attemptsLeft--;  // Reducir el número de intentos restantes

    // Dibujar un círculo negro en la posición anterior de la pelota
    tft.fillCircle(ballX, ballY, BALL_SIZE, 0x0000);
  }

  // Rebotar la pelota si toca la barra
  if (ballY + BALL_SIZE / 2 >= BAR_Y && ballY - BALL_SIZE / 2 <= BAR_Y + BAR_HEIGHT && ballX + BALL_SIZE / 2 >= barX && ballX - BALL_SIZE / 2 <= barX + BAR_WIDTH) {
    ballVelY = -ballVelY;
  }

  // Verificar colisiones con los bloques
  for (int i = 0; i < NUM_BLOCKS; i++) {
    if (blocks[i].active && ballX + BALL_SIZE / 2 >= blocks[i].x && ballX - BALL_SIZE / 2 <= blocks[i].x + BLOCK_WIDTH && ballY + BALL_SIZE / 2 >= blocks[i].y && ballY - BALL_SIZE / 2 <= blocks[i].y + BLOCK_HEIGHT) {
      blocks[i].active = false;
      tft.fillRect(blocks[i].x, blocks[i].y, BLOCK_WIDTH, BLOCK_HEIGHT, 0x0000);  // Borra el bloque
      ballVelY = -ballVelY;

      score += 5;  // Incrementar el puntaje por cada bloque destruido
      // Toca una nota en el buzzer
      playNote(1000, 100); // Frecuencia de 1000 Hz, duración de 100 ms
    }
  }

  // Si se agotan los intentos, regenerar los bloques
  if (attemptsLeft <= 0) {
    generateBlocks();  // Regenerar bloques
    attemptsLeft = 3;  // Restablecer el número de intentos
  }

  drawScore();  // Llama a la función para dibujar el puntaje

  delay(10);
}

// Función para generar bloques
void generateBlocks() {
  score = 0; // Restablece el puntaje a 0 cuando se regeneran los bloques

  // Generar colores aleatorios para cada fila
  uint16_t colors[NUM_BLOCKS / 13];
  for (int i = 0; i < NUM_BLOCKS / 13; i++) {
    colors[i] = random(0xFFFF); // Genera un color aleatorio para cada fila
  }

  // Generar bloques
  for (int i = 0; i < NUM_BLOCKS; i++) {
    // Calcular la fila y la columna actual
    int row = i / 13;
    int col = i % 13;

    // Calcular la posición del bloque
    int x = col * (BLOCK_WIDTH + BLOCK_SPACING) + BLOCK_SPACING;
    int y = row * (BLOCK_HEIGHT + BLOCK_SPACING) + BLOCK_SPACING * 7;

    // Asignar la posición y otras propiedades al bloque
    blocks[i].x = x;
    blocks[i].y = y;
    blocks[i].active = true;

    // Asignar el color de la fila correspondiente al bloque
    blocks[i].color = colors[row];

    // Dibujar el bloque en la pantalla
    tft.fillRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, blocks[i].color);
  }
}

void drawScore() {
  // Borra el área donde se muestra el puntaje
  tft.fillRect(10, 10, 50, 20, 0x0000);
  // Establece la posición donde se dibujará el puntaje
  tft.setCursor(10, 10);
  // Configura el color y el tamaño del texto
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  // Dibuja el puntaje actual
  tft.print(score);
}

void displayStartText() {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  static bool showText = true;
  static bool melodyStarted = false;

  // Intervalo de tiempo para el parpadeo (250 ms)
  const unsigned long interval = 250;

  // Si ha pasado el tiempo del intervalo, alternar la visibilidad del texto
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Actualizar el tiempo anterior

    // Alternar la visibilidad del texto
    showText = !showText;

    // Calcula la posición para centrar el texto "Start"
    int startX = (tft.width() - 60) / 2; // Posición X para centrar horizontalmente
    int startY = (tft.height() - 16) / 2; // Posición Y para centrar verticalmente

    // Borra el área donde se muestra el texto "Press to Start"
    int textWidth = 90; // Ancho aproximado del texto "Press to Start" en píxeles
    int textHeight = 8; // Altura aproximada del texto "Press to Start" en píxeles
    int pressStartX = (tft.width() - textWidth) / 2; // Posición X para centrar horizontalmente
    int pressStartY = startY + 16 + 10; // Posición Y debajo de "Start"
    tft.fillRect(pressStartX, pressStartY, textWidth, textHeight, 0x0000); // Borra el área donde se muestra el texto "Press to Start"

    // Muestra el texto "Karen Riveros y Juan Giraldo" en el borde inferior derecho
    String names = "Karen Riveros y Juan Giraldo";
    int textWidth2 = names.length() * 6; // Ancho aproximado del texto en píxeles (cada caracter tiene un ancho aproximado de 6 píxeles)
    tft.setTextColor(TFT_GREEN); // Color del texto (verde)
    tft.setTextSize(1); // Tamaño del texto
    tft.setCursor(tft.width() - textWidth2, tft.height() - 10); // Posición en el borde inferior derecho
    tft.print(names);

    // Muestra el texto "Press to Start" si showText es falso
    if (!showText) {
      tft.setTextColor(0xFFE0); // Color del texto amarillo
      tft.setTextSize(1); // Tamaño del texto
      tft.setCursor(pressStartX, pressStartY);
      tft.print("Press to Start");
    }

    // Si la melodía aún no ha comenzado, inicia la melodía
    if (!melodyStarted) {
      melodyStarted = true; // Marca que la melodía ha comenzado
      playMelody(); // Inicia la melodía
    }
  }
}

void playMelody() {
  // Repite la melodía 3 veces (puedes ajustar este número según tu preferencia)
  for (int j = 0; j < 5; j++) {
    // Reproduce la melodía completa
    for (int i = 0; i < MELODY_LENGTH; i++) {
      playNote(MELODY[i], 100); // Duración de cada nota: 100 ms
    }
  }
}

void playNote(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}