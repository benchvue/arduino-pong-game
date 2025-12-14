#include <Modulino.h>

// Create a ModulinoBuzzer object
ModulinoBuzzer buzzer;

void setup(){
  // Initialize Modulino I2C communication
  Modulino.begin();
  // Detect and connect to buzzer module
  buzzer.begin();
}

void loop(){

  soundWall();
  delay(1000);

  soundPaddle();
  delay(1000);

  soundGameOver();
  delay(1000);

}

void soundWall() {
  buzzer.tone(1200, 40);   // short high beep
}

void soundPaddle() {
  buzzer.tone(800, 60);    // medium beep
}

void soundGameOver() {
  buzzer.tone(300, 300);   // low long buzz
}
