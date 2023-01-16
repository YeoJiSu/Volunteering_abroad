/**
   Simon Game for Arduino

   Copyright (C) 2016, Uri Shaked

   Released under the MIT License.
*/
#include <LiquidCrystal.h>
#define NOTE_G3 196
#define NOTE_C4 262
#define NOTE_DS5 622
#define NOTE_D5 587
#define NOTE_CS5 554
#define NOTE_E4 330
#define NOTE_G4 392
#define NOTE_E5 659
#define NOTE_C5 523
#define NOTE_G5 784
#define SPEAKER_PIN 8
#define MAX_GAME_LENGTH 100

LiquidCrystal lcd(6,A1,A2,A3,A4,A5);

const byte ledPins[] = {9, 10, 11, 12};
const byte buttonPins[] = {2, 3, 4, 5};

const int gameTones[] = { NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

byte gameSequence[MAX_GAME_LENGTH] = {0};
byte gameIndex = 0;

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
