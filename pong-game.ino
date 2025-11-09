#include "Arduino_LED_Matrix.h"
#include <Modulino.h>

// LED 매트릭스 및 버튼 객체 생성
ArduinoLEDMatrix matrix;
ModulinoButtons buttons;

// Game variables
int paddlePos = 4; // Paddle position (range 0-10, starts at center)
int ballX = 6;     // Ball X position
int ballY = 4;     // Ball Y position
int ballDirX = -1; // Ball X direction (-1: left, 1: right)
int ballDirY = 1;  // Ball Y direction (-1: up, 1: down)
unsigned long lastMoveTime = 0;
int gameSpeed = 400; // Ball movement speed (ms) - increased for slower movement
bool gameOver = false;
int score = 0;

// 8x12 LED frame buffer
uint8_t frame[8][12] = {0};

void setup() {
  Serial.begin(9600);
  
  // Initialize LED matrix
  matrix.begin();
  
  // Initialize Modulino buttons
  Modulino.begin();
  buttons.begin();
  buttons.setLeds(true, false, true); // Only use A and C buttons
  
  // Start game
  initGame();
}

void loop() {
  if (gameOver) {
    displayGameOver();
    delay(3000);
    initGame();
    return;
  }
  
  // Handle button input
  handleButtons();
  
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
  gameSpeed = 400; // Reset to slower speed
}

void handleButtons() {
  if (buttons.update()) {
    // A button: Move paddle left
    if (buttons.isPressed('A')) {
      if (paddlePos > 0) {
        paddlePos--;
      }
      Serial.print("Paddle Left: ");
      Serial.println(paddlePos);
    }
    // C button: Move paddle right
    else if (buttons.isPressed('C')) {
      if (paddlePos < 10) {
        paddlePos++;
      }
      Serial.print("Paddle Right: ");
      Serial.println(paddlePos);
    }
  }
}

void moveBall() {
  // Update ball position
  ballX += ballDirX;
  ballY += ballDirY;
  
  // Top wall collision (top of ㄷ shape)
  if (ballY <= 0) {
    ballY = 0;
    ballDirY = 1;
  }
  
  // Left wall collision (left side of ㄷ shape)
  if (ballX <= 0) {
    ballX = 0;
    ballDirX = 1;
  }
  
  // Right wall collision (right side of ㄷ shape)
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
        
        Serial.print("Score: ");
        Serial.println(score);
      } else {
        // Game over - ball missed the paddle
        gameOver = true;
        Serial.println("Game Over!");
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
  
  matrix.clear();
}
