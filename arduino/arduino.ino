const int ledPin = 13;
const int motorPin1 = 6;
const int motorPin2 = 7;

const int blinkDelay = 300;

void setup() {
  // Setup
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String module = Serial.readStringUntil(':');
    Serial.read();
    int value = Serial.readStringUntil('\n').toInt();

    // Serial.println(module);
    // Serial.println(value);

    if (module == "led") {
      blink(value);
    } else if (module == "motor") {
      rotate(value);
    }
  }
}

void blink(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, HIGH);
    delay(blinkDelay);
    digitalWrite(ledPin, LOW);
    delay(blinkDelay);
  }
}

// Motor
void rotate(int mode) {
  if (mode == 0) {
    motorStop();
  } else if (mode == 1) {
    motorRotateClockwise();
  } else if (mode == -1) {
    motorRotateCounterClockwise();
  }
}

void motorStop() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}

void motorRotateClockwise() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
}

void motorRotateCounterClockwise() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
}
