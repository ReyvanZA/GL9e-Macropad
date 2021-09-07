#include <DigiKeyboard.h>

int analogInPin = 1;
int analogIn;

void setup() {
  delay(1000);
  DigiKeyboard.sendKeyStroke(0);
  pinMode(analogInPin, INPUT);
  
}

bool between(int start, int finish, int value){
  bool isBetween = false;
  if (value > start and value < finish ) isBetween = true;
  return isBetween;
}


void loop() {
  analogIn = analogRead(analogInPin);

  if (analogIn< 50){
    DigiKeyboard.sendKeyStroke(KEY_F13);
    delay(500);
  };
  if (between(50, 200, analogIn)) {
    DigiKeyboard.sendKeyStroke(KEY_F14);
    delay(500);
  };
  if (between(200, 350, analogIn)){
    DigiKeyboard.sendKeyStroke(KEY_F15);
    delay(500);
  };
  if (between(350, 550, analogIn)) {
    DigiKeyboard.sendKeyStroke(KEY_F16);
    delay(500);
  };
  
}
