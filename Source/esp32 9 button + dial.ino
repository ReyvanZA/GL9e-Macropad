
//BOARD: DOIT ESP32 DEVKIT V1

/*#TODO#
 * Make 1 profile based on F13-F24 
 * Make Encoder press F13, left turn F14, right turn F15
 *     User Autohotkey to deal with profiles
 * Done - Make set board blue light on whe bluetooth connected
 * Glow under encoder light based on profile (!!!??!!!)
*/

#include <ESP32Encoder.h>
#include <BleKeyboard.h>
#include <Keypad.h>

//-- LED --
const int RED = 21;
const int BLUE = 1;
const int ONBOARD = 2; //Same pin as encoder wheel!
const int GREEN = 3;
const int DELAY = 200;
//--**--

//--Setup keypad--
  const byte ROWS = 3;
  const byte COLS = 3;
  
  char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
  };
  byte rowPins[ROWS] =  {19, 18, 5};
  byte colPins[COLS] =  {32, 22, 23};
  Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//--**--

//-- Setup Encoder --
  const int ENCBUTTON = 15;
  ESP32Encoder enc;
  unsigned long posEnc  = -999;
  bool encbut = false;
  unsigned long lastButtonPress = 0;
  unsigned long lastRotation = 0;
  long newPosEnc = 0;
//--**--

BleKeyboard bKey("GL 9E MacroPad", "GreenLight", 100);

int profile = 0;

void setup() {

  bKey.begin();
  
  ESP32Encoder::useInternalWeakPullResistors = UP;
  enc.attachSingleEdge(2, 4);
  enc.setCount(0);
  pinMode(ENCBUTTON, INPUT_PULLUP);

  //LED's
  pinMode(ONBOARD, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(ONBOARD,LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
}

//Windows Key funciton
void winKey(char ch) { //Windows  + Character
  bKey.press(KEY_LEFT_GUI);
  bKey.press(ch);
  delay(100);
  bKey.releaseAll();
}

void ctrlKey(char ch) {  //Ctrl + Character
  bKey.press(KEY_LEFT_CTRL);
  bKey.press(ch);
  delay(100);
  bKey.releaseAll();
}

void shiftKey(char ch){ //Shift + Character
  bKey.press(KEY_LEFT_SHIFT);
  bKey.press(ch);
  delay(100);
  bKey.releaseAll();
}

void writeKey(char ch){
  bKey.write(ch);
}

void flash(int Color = RED){
  digitalWrite(Color, HIGH);
  delay(DELAY);
  digitalWrite(Color, LOW);
}

void glow(int Color = RED){
  
}

void loop() {

  char key = keypad.getKey();
  
  //rotate profiles
  encbut = (digitalRead(ENCBUTTON)== LOW);
  if (encbut){
    if (!bKey.isConnected()){
      ESP.restart();
    }
    if (millis() - lastButtonPress > 50) { //Debounce
        if (profile < 2){
          profile++;
        }else{
          profile = 0;
        }
        switch (profile){
          case 0:
            flash(RED);
            break;
          case 1:
            flash(GREEN);
            break;
          case 2:
            flash(RED);
            flash(GREEN);
            break;
        }
    }
    lastButtonPress = millis();
  }

  //Check if bluetooth is connected
  if (bKey.isConnected()){
    digitalWrite(ONBOARD,HIGH);
  }else{
    digitalWrite(ONBOARD,LOW);
  }

  if (bKey.isConnected() &&  key) {
    switch (profile){
          case 0:
            switch (key) {
              case '1':
                writeKey(KEY_F5); //Refresh
                break;
              case '2':
                ctrlKey('t'); //New Tab (Chrome)
                break;
              case '3':
                ctrlKey(KEY_TAB); //Next Tab (Chrome)
                break;
              case '4':
                bKey.write(KEY_MEDIA_PREVIOUS_TRACK);
                break;
              case '5':
                bKey.write(KEY_MEDIA_PLAY_PAUSE);
                break;
              case '6':
                bKey.write(KEY_MEDIA_NEXT_TRACK);
                break;
              case '7':
                winKey('l'); //Lock screen
                break;
              case '8':
                winKey('m'); //Minimize all
                break;
              case '9':
                ctrlKey('w'); //Close Window
                break;
            }
            break;
          case 1:
            switch (key) {
              case '1':
                writeKey('1');
                break;
              case '2':
                writeKey('2');
                break;
              case '3':
                writeKey('3');
                break;
              case '4':
                writeKey('4');
                break;
              case '5':
                writeKey('5');
                break;
              case '6':
                writeKey('6');
                break;
              case '7':
                writeKey('7');
                break;
              case '8':
                writeKey('8');
                break;
              case '9':
                writeKey('9');
                break;
            }
            break;
        case 2:
            switch (key) {
              case '1':
                ctrlKey(KEY_F16);
                break;
              case '2':
                ctrlKey(KEY_F17);
                break;
              case '3':
                ctrlKey(KEY_F18);
                break;
              case '4':
                ctrlKey(KEY_F19);
                break;
              case '5':
                ctrlKey(KEY_F20);
                break;
              case '6':
                ctrlKey(KEY_F21);
                break;
              case '7':
                ctrlKey(KEY_F22);
                break;
              case '8':
                ctrlKey(KEY_F23);
                break;
              case '9':
                ctrlKey(KEY_F24);
                break;
            }
            break;
      }
   }else{
       if (key) ESP.restart();
   }

  newPosEnc = enc.getCount();
  if  (millis() - lastRotation > 100){ //debounce
    if (newPosEnc > posEnc) {
      switch (profile){
        case 0:
         bKey.write(KEY_MEDIA_VOLUME_UP);
         break;
        case 1:
         writeKey('+');
         break;
        case 2:
         ctrlKey(KEY_F15);
         break;
      }
      posEnc = newPosEnc;
      lastRotation = millis();
    } else if (newPosEnc < posEnc) {
      switch (profile){
        case 0:
         bKey.write(KEY_MEDIA_VOLUME_DOWN);
         break;
        case 1:
         writeKey('0');
         break;
        case 2:
         ctrlKey(KEY_F14);
         break;
      }
      posEnc = newPosEnc;
      lastRotation = millis();
    } else {
      enc.setCount(posEnc); //Reset encoder position as not enough time has passed
    }//End Else If
  }//End if
        
}//End
