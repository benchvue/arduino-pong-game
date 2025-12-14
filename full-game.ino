#include "Arduino_LED_Matrix.h"
#include <Modulino.h>

// ================== Modules ==================
ArduinoLEDMatrix matrix;
ModulinoMovement movement;
ModulinoBuzzer buzzer;

// ================== Game State ==================
int paddlePos = 4;        // 0 ~ 9
int ballX = 6;
int ballY = 4;
int ballDirX = -1;
int ballDirY = 1;

bool gameOver = false;
int score = 0;

// ================== Timing ==================
unsigned long lastBallMove = 0;
unsigned long lastPaddleMove = 0;

int gameSpeed   = 500;    // ⬆ bigger = slower ball
int paddleSpeed = 200;    // ⬆ bigger = slower paddle

// ================== Movement ==================
float threshold = 0.4;    // tilt sensitivity

// ================== Display ==================
uint8_t frame[8][12] = {0};

// =================================================

void setup() {
  Serial.begin(9600);

  matrix.begin();
  Modulino.begin();
  movement.begin();
  buzzer.begin();

  initGame();
}

void loop() {
  if (gameOver) {
    displayGameOver();
    delay(3000);
    initGame();
    return;
  }

  handleMovementX();

  if (millis() - lastBallMove > gameSpeed) {
    lastBallMove = millis();
    moveBall();
  }

  drawGame();
  delay(20);
}

// ================== Game Logic ==================

void initGame() {
  gameOver = false;
  score = 0;
  paddlePos = 4;
  ballX = 6;
  ballY = 4;
  ballDirX = -1;
  ballDirY = 1;
  lastBallMove = millis();
  lastPaddleMove = millis();
}

void handleMovementX() {
  movement.update();
  float x = movement.getX();

  if (millis() - lastPaddleMove < paddleSpeed) return;

  if (x > threshold && paddlePos < 9) {
    paddlePos++;
    lastPaddleMove = millis();
  }
  else if (x < -threshold && paddlePos > 0) {
    paddlePos--;
    lastPaddleMove = millis();
  }
}

void moveBall() {
  ballX += ballDirX;
  ballY += ballDirY;

  // Top wall
  if (ballY <= 0) {
    ballY = 0;
    ballDirY = 1;
    soundWall();
  }

  // Left wall
  if (ballX <= 0) {
    ballX = 0;
    ballDirX = 1;
    soundWall();
  }

  // Right wall
  if (ballX >= 11) {
    ballX = 11;
    ballDirX = -1;
    soundWall();
  }

  // Paddle collision
  if (ballY >= 7 && ballDirY > 0) {
    if (ballX >= paddlePos && ballX <= paddlePos + 2) {
      ballY = 6;
      ballDirY = -1;
      score++;
      soundPaddle();

      if (score % 5 == 0 && gameSpeed > 200) {
        gameSpeed -= 30; // slightly faster over time
      }
    } else {
      gameOver = true;
      soundGameOver();
    }
  }
}

// ================== Rendering ==================

void drawGame() {
  memset(frame, 0, sizeof(frame));

  // Walls
  for (int x = 0; x < 12; x++) frame[0][x] = 1;
  for (int y = 0; y < 8; y++) frame[y][0] = 1;
  for (int y = 0; y < 8; y++) frame[y][11] = 1;

  // Ball
  frame[ballY][ballX] = 1;

  // Paddle
  for (int i = 0; i < 3; i++) {
    frame[7][paddlePos + i] = 1;
  }

  matrix.renderBitmap(frame, 8, 12);
}

void displayGameOver() {
  for (int i = 0; i < 3; i++) {
    matrix.clear();
    delay(200);

    for (int y = 0; y < 8; y++)
      for (int x = 0; x < 12; x++)
        frame[y][x] = 1;

    matrix.renderBitmap(frame, 8, 12);
    delay(200);
  }
}

// ================== Sounds ==================

void soundWall() {
  buzzer.tone(1200, 40);
}

void soundPaddle() {
  buzzer.tone(800, 60);
}

void soundGameOver() {
  buzzer.tone(300, 300);
}
