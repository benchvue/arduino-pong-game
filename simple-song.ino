#include <Modulino.h>

ModulinoBuzzer buzzer;

// Notes
void do_()     { buzzer.tone(262, 200); delay(220); }
void re_()     { buzzer.tone(294, 200); delay(220); }
void mi_()     { buzzer.tone(330, 200); delay(220); }
void fa()      { buzzer.tone(349, 200); delay(220); }
void sol()     { buzzer.tone(392, 200); delay(220); }
void la()      { buzzer.tone(440, 200); delay(220); }
void si()      { buzzer.tone(494, 200); delay(220); }
void highDo()  { buzzer.tone(523, 300); delay(320); }

void rest(int t) { buzzer.tone(0, t); delay(t); }

void setup() {
  Modulino.begin();
  buzzer.begin();
}

void loop() {
  // Simple song (Twinkle-style)
  do_(); do_(); sol(); sol();
  la(); la(); sol();
  rest(300);

  fa(); fa(); mi_(); mi_();
  re_(); re_(); do_();
  rest(600);
}
