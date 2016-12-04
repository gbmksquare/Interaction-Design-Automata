#define ON HIGH
#define OFF LOW

const int indicatorLedPin = 13; // Digital
const int redLedPin = 9;        // Analog
const int motorSpeedPin = 5;    // Analog
const int motorControl1Pin = 6; // Analog
const int motorControl2Pin = 7; // Digital

const int blinkDelay = 300;

bool isMotorClockwise = true;

void setup() {
  // Setup
  pinMode(indicatorLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorControl1Pin, OUTPUT);
  pinMode(motorControl2Pin, OUTPUT);
  Serial.begin(9600);
  blinkLed(indicatorLedPin, 3);
}

void loop() {
  if (Serial.available() > 0) {
    String module = Serial.readStringUntil(':');
    Serial.read();
    int value = Serial.readStringUntil('\n').toInt();

    // Serial.println(module);
    // Serial.println(value);

    handle(module, value);
  }
}

// Hanlder
void handle(String module, int value) {
  if (module == "led") {
    handleLed(redLedPin, value);
  } else if (module == "ledblink") {
    blinkLed(redLedPin, value);
  } else if (module == "indicator") {
    handleLed(indicatorLedPin, value);
  } else if (module == "indicatorblink") {
    blinkLed(indicatorLedPin, value);
  } else if (module == "motor") {
    handleMotorPower(value);
  } else if (module == "motorcw") {
    handleMotorRotation(value);
  } else if (module == "motorspeed") {
    motorSpeed(value);
  }
}

void handleLed(int ledPin, int value) {
  if (value == 1) {
    turnOnLed(ledPin);
  } else if (value == 0) {
    turnOffLed(ledPin);
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
void turnOnLed(int ledPin) {
  digitalWrite(ledPin, ON);
}

void turnOffLed(int ledPin) {
  digitalWrite(ledPin, OFF);
}

void blinkLed(int ledPin, int count) {
  int originalStatus = digitalRead(ledPin);
  int newStatus = (originalStatus == OFF) ? ON : OFF;

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
  digitalWrite(motorControl1Pin, LOW);
  digitalWrite(motorControl2Pin, LOW);
}

void motorSpeed(int value) {
  value = map(value, 0, 100, 0, 255);
  value = constrain(value, 0, 255);
  analogWrite(motorSpeedPin, value);
}

void motorRotateClockwise() {
  digitalWrite(motorControl1Pin, LOW);
  digitalWrite(motorControl2Pin, HIGH);
}

void motorRotateCounterClockwise() {
  digitalWrite(motorControl1Pin, HIGH);
  digitalWrite(motorControl2Pin, LOW);
}
