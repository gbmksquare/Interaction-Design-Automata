const int ledPin = 13;
const int motorPin1 = 6;
const int motorPin2 = 7;

const int blinkDelay = 300;

bool isMotorClockwise = true;

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
      handleLed(value);
    } else if (module == "ledblink") {
      ledBlink(value);
    } else if (module == "motor") {
      handleMotorPower(value);
    } else if (module == "motorcw") {
      handleMotorRotation(value);
    }
  }
}

// Hanlder
void handleLed(int value) {
  if (value == 1) {
    ledOn();
  } else if (value == 0) {
    ledOff();
  }
}

void handleMotorPower(int value) {
  if (value == 1) {
    if (isMotorClockwise == true) {
      motorRotateClockwise();  
    } else {
      motorRotateCounterClockwise();
    }
  } else if (value == 0) {
    motorStop();
  }
}

void handleMotorRotation(int value) {
  if (value == 1) {
    motorRotateClockwise();
    isMotorClockwise = true;
  } else if (value == 0) {
    motorRotateCounterClockwise();
    isMotorClockwise = false;
  }
}

// LED
void ledOn() {
  digitalWrite(ledPin, HIGH);
}

void ledOff() {
  digitalWrite(ledPin, LOW);
}

void ledBlink(int count) {
  int originalStatus = digitalRead(ledPin);
  int newStatus = (originalStatus == LOW) ? HIGH : LOW;

  for (int i = 0; i < count; i++) {
    digitalWrite(ledPin, originalStatus);
    delay(blinkDelay);
    digitalWrite(ledPin, newStatus);
    delay(blinkDelay);
  }
  digitalWrite(ledPin, originalStatus);
}

// Motor
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
