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
  } else if (module == "ledbrightness") {
    setLedBrightness(redLedPin, value);
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
  } else if (module == "heartbeat") {
    fadeInOutLed(redLedPin, value);
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

void setLedBrightness(int ledPin, int brightness) {
  brightness = map(brightness, 0, 100, 0, 255);
  brightness = constrain(brightness, 0, 255);
  analogWrite(ledPin, brightness);
}

void fadeInOutLed(int ledPin, int bpm) {
  bpm = bpm / 2;

  unsigned int millisecInMinute = 1 * 60 * 1000;
  unsigned int millisecPerBeat = millisecInMinute / bpm;

  int offBeat = 0;
  int delayPerLoop = 0;

  if (bpm >= 100) {
    double onBeat = millisecPerBeat / 8 * 7;
    offBeat = millisecPerBeat / 8;
    delayPerLoop = ceil(onBeat / (256 * 2));
    delayPerLoop = delayPerLoop - 1;
  } else {
    double onBeat = millisecPerBeat / 4 * 3;
    offBeat = millisecPerBeat / 4;
    delayPerLoop = ceil(onBeat / (256 * 2));
  }
  
  for (int n = 0; n < 3; n++) {
    for (int i = 0; i <= 255; i++) {
      analogWrite(ledPin, i);
      delay(delayPerLoop);
    }
    for (int i = 255; i >= 0; i--) {
      analogWrite(ledPin, i);
      delay(delayPerLoop);
    }
    delay(offBeat);
  }
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
