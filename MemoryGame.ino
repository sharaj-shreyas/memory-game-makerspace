// LED and button pin definitions
const int redLED = 2;
const int yellowLED = 4;
const int greenLED = 6;
const int blueLED = 8;

const int redButton = 3;
const int yellowButton = 5;
const int greenButton = 7;
const int blueButton = 9;

const int buzzer = 12;

// Maximum sequence length
const int MAX_LEVEL = 100;

int sequence[MAX_LEVEL];
int playerInput[MAX_LEVEL];

int level = 0;
bool gameOver = false;

// ===============================
// SPEED SETTINGS
// ===============================

// Starting LED ON time
int sequenceOnTime = 300;

// Starting gap between LEDs
int sequenceGap = 180;

// Amount sequence speeds up every round
const int SPEED_INCREASE = 15;

// Minimum speed limits
const int MIN_ON_TIME = 100;
const int MIN_GAP = 50;

// Player button feedback duration
const int playerLEDTime = 180;


void setup() {

  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  pinMode(redButton, INPUT_PULLUP);
  pinMode(yellowButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);
  pinMode(blueButton, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);

  randomSeed(analogRead(A0));

  startGame();
}


void loop() {

  if (!gameOver) {

    // Show sequence
    playSequence();

    // Get player response
    if (!getPlayerInput()) {
      endGame();
    }
    else {

      // Make sequence faster after every successful round
      sequenceOnTime -= SPEED_INCREASE;
      sequenceGap -= SPEED_INCREASE;

      // Prevent it from becoming too fast
      if (sequenceOnTime < MIN_ON_TIME) {
        sequenceOnTime = MIN_ON_TIME;
      }

      if (sequenceGap < MIN_GAP) {
        sequenceGap = MIN_GAP;
      }

      Serial.print("Level: ");
      Serial.println(level);

      Serial.print("Sequence ON time: ");
      Serial.println(sequenceOnTime);

      Serial.print("Sequence gap: ");
      Serial.println(sequenceGap);

      delay(400);
    }
  }

  else {

    delay(1500);

    startGame();
  }
}


// ==================================
// START / RESET GAME
// ==================================

void startGame() {

  level = 1;
  gameOver = false;

  // Reset speed
  sequenceOnTime = 300;
  sequenceGap = 180;

  // Startup animation
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);

  tone(buzzer, 1000, 400);

  delay(500);

  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);

  delay(500);
}


// ==================================
// PLAY MEMORY SEQUENCE
// ==================================

void playSequence() {

  // Add ONE new random color
  sequence[level - 1] = random(1, 5);

  delay(300);

  for (int i = 0; i < level; i++) {

    playSequenceLED(sequence[i]);

    delay(sequenceGap);
  }
}


// ==================================
// PLAYER INPUT
// ==================================

bool getPlayerInput() {

  for (int i = 0; i < level; i++) {

    bool inputReceived = false;

    while (!inputReceived) {

      if (digitalRead(redButton) == LOW) {

        playerInput[i] = 1;
        playPlayerLED(1);

        inputReceived = true;

        waitForButtonRelease(redButton);
      }

      else if (digitalRead(yellowButton) == LOW) {

        playerInput[i] = 2;
        playPlayerLED(2);

        inputReceived = true;

        waitForButtonRelease(yellowButton);
      }

      else if (digitalRead(greenButton) == LOW) {

        playerInput[i] = 3;
        playPlayerLED(3);

        inputReceived = true;

        waitForButtonRelease(greenButton);
      }

      else if (digitalRead(blueButton) == LOW) {

        playerInput[i] = 4;
        playPlayerLED(4);

        inputReceived = true;

        waitForButtonRelease(blueButton);
      }
    }

    // Wrong button
    if (playerInput[i] != sequence[i]) {
      return false;
    }

    delay(100);
  }

  // Next level
  level++;

  // Prevent array overflow
  if (level >= MAX_LEVEL) {
    level = MAX_LEVEL - 1;
  }

  return true;
}


// ==================================
// WAIT UNTIL BUTTON IS RELEASED
// ==================================

void waitForButtonRelease(int buttonPin) {

  while (digitalRead(buttonPin) == LOW) {
    delay(10);
  }

  // Small debounce delay
  delay(40);
}


// ==================================
// GAME OVER
// ==================================

void endGame() {

  gameOver = true;

  noTone(buzzer);

  for (int i = 0; i < 4; i++) {

    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, HIGH);
    digitalWrite(blueLED, HIGH);

    tone(buzzer, 250, 150);

    delay(180);

    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);

    delay(180);
  }

  noTone(buzzer);
}


// ==================================
// PLAY LED DURING MEMORY SEQUENCE
// ==================================

void playSequenceLED(int color) {

  turnOnColor(color);

  delay(sequenceOnTime);

  turnOffAllLEDs();

  noTone(buzzer);
}


// ==================================
// PLAY LED WHEN PLAYER PRESSES BUTTON
// ==================================

void playPlayerLED(int color) {

  turnOnColor(color);

  delay(playerLEDTime);

  turnOffAllLEDs();

  noTone(buzzer);
}


// ==================================
// TURN ON COLOR + SOUND
// ==================================

void turnOnColor(int color) {

  switch (color) {

    case 1:
      digitalWrite(redLED, HIGH);
      tone(buzzer, 500);
      break;

    case 2:
      digitalWrite(yellowLED, HIGH);
      tone(buzzer, 600);
      break;

    case 3:
      digitalWrite(greenLED, HIGH);
      tone(buzzer, 700);
      break;

    case 4:
      digitalWrite(blueLED, HIGH);
      tone(buzzer, 800);
      break;
  }
}


// ==================================
// TURN OFF ALL LEDs
// ==================================

void turnOffAllLEDs() {

  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
}