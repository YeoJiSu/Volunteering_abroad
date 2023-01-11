/**
   Simon Game for Arduino

   Copyright (C) 2016, Uri Shaked

   Released under the MIT License.
*/

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

/* Constants - define pin numbers for LEDs,
   buttons and speaker, and also the game tones: */
const byte ledPins[] = {9, 10, 11, 12};
const byte buttonPins[] = {2, 3, 4, 5};
#define SPEAKER_PIN 8

#define MAX_GAME_LENGTH 100

const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

/* Global variables - store the game state */
byte gameSequence[MAX_GAME_LENGTH] = {0};
byte gameIndex = 0;

/**
   Set up the Arduino board and initialize Serial communication
*/
void setup() {
   lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.write("Let's Start game");
    lcd.setCursor(0,1);
    lcd.write("Press red button"); 
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(SPEAKER_PIN, OUTPUT);
  // The following line primes the random number generator.
  // It assumes pin A0 is floating (disconnected):
  randomSeed(analogRead(A0));
}

/**
   Lights the given LED and plays a suitable tone
*/
void lightLedAndPlayTone(byte ledIndex) {
  digitalWrite(ledPins[ledIndex], HIGH);
  tone(SPEAKER_PIN, gameTones[ledIndex]);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(SPEAKER_PIN);
}

/**
   Plays the current sequence of notes that the user has to repeat
*/
void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);
    delay(50);
  }
}

/**
    Waits until the user pressed one of the buttons,
    and returns the index of that button
*/
byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i;
      }
    }
    delay(1);
  }
}

/**
  Play the game over sequence, and report the game score
*/
void gameOver() {
  Serial.print("Game over! your score: ");
  Serial.println(gameIndex - 1);
  gameIndex = 0;
  delay(200);

  // Play a Wah-Wah-Wah-Wah sound
  tone(SPEAKER_PIN, NOTE_DS5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(300);
  tone(SPEAKER_PIN, NOTE_CS5);
  delay(300);
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(SPEAKER_PIN, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(SPEAKER_PIN);
  delay(500);
}

/**
   Get the user's input and compare it with the expected sequence.
*/
bool checkUserSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLedAndPlayTone(actualButton);
    if (expectedButton != actualButton) {
      return false;
    }
  }

  return true;
}

/**
   Plays a hooray sound whenever the user finishes a level
*/
void playLevelUpSound() {
  tone(SPEAKER_PIN, NOTE_E4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G4);
  delay(150);
  tone(SPEAKER_PIN, NOTE_E5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_C5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_D5);
  delay(150);
  tone(SPEAKER_PIN, NOTE_G5);
  delay(150);
  noTone(SPEAKER_PIN);
}

/**
   The main game loop
*/
int index = 1;
int start = 0;
void loop() {
  
  
  if (start == 0) {
    byte buttonPin = buttonPins[3];
    if (digitalRead(buttonPin) == LOW){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write("okay! Let's go");
        delay(1500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write("Level:");
        lcd.setCursor(0,1);
        lcd.print(index);
        start+=1; 
    }
  }
  else{
    // Add a random color to the end of the sequence
    
    gameSequence[gameIndex] = random(0, 4);
    gameIndex++;
    if (gameIndex >= MAX_GAME_LENGTH) {
      gameIndex = MAX_GAME_LENGTH - 1;
    }
    playSequence();
    if (!checkUserSequence()) {
      index = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("You Lose");
      gameOver();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Let's restart");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Level:");
      lcd.setCursor(0,1);
      lcd.print(index);
    }
  
    delay(300);
  
    if (gameIndex > 0) {
      index+=1;
      playLevelUpSound();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Correct!");
      delay(300);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Level:");
      lcd.setCursor(0,1);
      lcd.print(index);
    }
  }
}
