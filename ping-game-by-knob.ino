#include "Arduino_LED_Matrix.h"
#include <Modulino.h>

// Initialize LED matrix and knob instances
ArduinoLEDMatrix matrix;
ModulinoKnob knob;

// Game variables
int paddlePos = 4;  // Paddle position (range 0-9, starts at center)
int ballX = 6;      // Ball X position
int ballY = 4;      // Ball Y position
int ballDirX = -1;  // Ball X direction (-1: left, 1: right)
int ballDirY = 1;   // Ball Y direction (-1: up, 1: down)
unsigned long lastMoveTime = 0;
int gameSpeed = 400;  // Ball movement speed (ms)
bool gameOver = false;
int score = 0;

// 8x12 LED frame buffer
uint8_t frame[8][12] = { 0 };

void setup() {
  Serial.begin(9600);

  // Initialize LED matrix
  matrix.begin();

  // Initialize Modulino
  Modulino.begin();
  knob.begin();

  // Start game
  initGame();

  Serial.println("Ping Pong Game Started!");
  Serial.println("Turn the knob to move the paddle");
}

void loop() {
  if (gameOver) {
    displayGameOver();
    delay(3000);
    initGame();
    return;
  }

  // Handle knob input
  handleKnob();

  // Move ball at regular intervals
  if (millis() - lastMoveTime > gameSpeed) {
    lastMoveTime = millis();
    moveBall();
  }

  // Draw screen
  drawGame();

  delay(10);
}

void initGame() {
  gameOver = false;
  score = 0;
  paddlePos = 4;
  ballX = 6;
  ballY = 4;
  ballDirX = -1;
  ballDirY = 1;
  gameSpeed = 400;

  Serial.println("\n--- New Game ---");
}

void handleKnob() {
  // Get the rotation direction from knob
  int8_t direction = knob.getDirection();

  // Check if knob was rotated
  if (direction == 1) {
    // Clockwise rotation - move paddle right
    if (paddlePos < 9) {  // Max position is 9 (paddle is 3 wide, max x is 11)
      paddlePos++;
    }
  } else if (direction == -1) {
    // Counter-clockwise rotation - move paddle left
    if (paddlePos > 0) {
      paddlePos--;
    }
  }
}

void moveBall() {
  // Update ball position
  ballX += ballDirX;
  ballY += ballDirY;

  // Top wall collision
  if (ballY <= 0) {
    ballY = 0;
    ballDirY = 1;
  }

  // Left wall collision
  if (ballX <= 0) {
    ballX = 0;
    ballDirX = 1;
  }

  // Right wall collision
  if (ballX >= 11) {
    ballX = 11;
    ballDirX = -1;
  }

  // Bottom - paddle collision check
  if (ballY >= 7) {
    // Only check collision if ball is moving down
    if (ballDirY > 0) {
      // Check paddle collision (paddle is 3 pixels wide)
      if (ballX >= paddlePos && ballX <= paddlePos + 2) {
        ballY = 6;  // Position ball just above paddle
        ballDirY = -1;
        score++;

        // Increase speed based on score
        if (score % 5 == 0 && gameSpeed > 150) {
          gameSpeed -= 30;
        }

        Serial.print("Hit! Score: ");
        Serial.println(score);
      } else {
        // Game over - ball missed the paddle
        gameOver = true;
        Serial.println("\n=== GAME OVER ===");
        Serial.print("Final Score: ");
        Serial.println(score);
      }
    }
  }
}

void drawGame() {
  // Clear frame buffer
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 12; x++) {
      frame[y][x] = 0;
    }
  }

  // Draw ㄷ-shaped walls
  // Top wall
  for (int x = 0; x < 12; x++) {
    frame[0][x] = 1;
  }
  // Left wall
  for (int y = 0; y < 8; y++) {
    frame[y][0] = 1;
  }
  // Right wall
  for (int y = 0; y < 8; y++) {
    frame[y][11] = 1;
  }

  // Draw ball
  if (ballX >= 0 && ballX < 12 && ballY >= 0 && ballY < 8) {
    frame[ballY][ballX] = 1;
  }

  // Draw paddle (bottom row, 3 pixels wide)
  for (int i = 0; i < 3; i++) {
    if (paddlePos + i >= 0 && paddlePos + i < 12) {
      frame[7][paddlePos + i] = 1;
    }
  }

  // Output to LED matrix
  matrix.renderBitmap(frame, 8, 12);
}

void displayGameOver() {
  // Flash all LEDs 3 times on game over
  for (int i = 0; i < 3; i++) {
    matrix.clear();
    delay(200);

    // Turn on all LEDs
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 12; x++) {
        frame[y][x] = 1;
      }
    }
    matrix.renderBitmap(frame, 8, 12);
    delay(200);
  }
}
