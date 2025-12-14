#include "Arduino_LED_Matrix.h"
#include <Modulino.h>

ArduinoLEDMatrix matrix;
ModulinoMovement movement;

// LED matrix size
#define WIDTH 12
#define HEIGHT 8

// Ball position
int ballX = WIDTH / 2;
int ballY = HEIGHT / 2;

// Frame buffer
uint8_t frame[HEIGHT][WIDTH] = {0};

// Sensitivity (smaller = slower movement)
float sensitivity = 0.4;

void setup() {
  Serial.begin(9600);

  matrix.begin();
  Modulino.begin();
  movement.begin();
}

void loop() {
  movement.update();

  float x = movement.getX(); // left / right
  float y = movement.getY(); // up / down

  // Debug
  Serial.print("X: ");
  Serial.print(x, 2);
  Serial.print("  Y: ");
  Serial.println(y, 2);

  // Center-based movement
  if (x > sensitivity) ballX++;
  if (x < -sensitivity) ballX--;

  if (y > sensitivity) ballY--;
  if (y < -sensitivity) ballY++;

  // Clamp to screen
  ballX = constrain(ballX, 0, WIDTH - 1);
  ballY = constrain(ballY, 0, HEIGHT - 1);

  drawBall();

  delay(100);
}

void drawBall() {
  // Clear frame
  memset(frame, 0, sizeof(frame));

  // Draw ball
  frame[ballY][ballX] = 1;

  matrix.renderBitmap(frame, HEIGHT, WIDTH);
}
