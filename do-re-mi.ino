#include <Modulino.h>

ModulinoBuzzer buzzer;

void do_()     { buzzer.tone(262, 200); delay(250); } // Do
void re_()     { buzzer.tone(294, 200); delay(250); } // Re
void mi_()     { buzzer.tone(330, 200); delay(250); } // Mi
void fa()      { buzzer.tone(349, 200); delay(250); } // Fa
void sol()     { buzzer.tone(392, 200); delay(250); } // Sol
void la()      { buzzer.tone(440, 200); delay(250); } // La
void si()      { buzzer.tone(494, 200); delay(250); } // Si
void highDo()  { buzzer.tone(523, 200); delay(250); } // High Do

void setup() {
  Modulino.begin();
  buzzer.begin();
}

void loop() {
  do_();
  re_();
  mi_();
  fa();
  sol();
  la();
  si();
  highDo();

  buzzer.tone(0, 0); // stop
  delay(1000);
}
