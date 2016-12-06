#define ON HIGH
#define OFF LOW

const int indicatorLedPin = 13; // Digital
const int redLedPin = 9;        // Analog
const int motorSpeedPin = 5;    // Analog
const int motorControl1Pin = 6; // Analog
const int motorControl2Pin = 7; // Digital

const int defaultBlinkDelay = 300;

bool isMotorClockwise = true;

bool isHeartbeatOn = false;
bool currentBpm = 0;
int heartbeatTransitionDelay = 0;
int heartbeatOffDelay = 0;
int heartbeatCurrentBrightness = 0;
int isHeartbeatBrightnessAscending = true;

int currentStep = 0;

// Run loop
void setup() {
  Serial.begin(9600);
  pinMode(indicatorLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(motorControl1Pin, OUTPUT);
  pinMode(motorControl2Pin, OUTPUT);
  blinkLed(indicatorLedPin, 5, 100);
}

void loop() {
  if (Serial.available() > 0) {
    String module = Serial.readStringUntil(':');
    Serial.read(); // Skip ':'
    int value = Serial.readStringUntil('\n').toInt();
    handle(module, value);

    // Serial.println(module);
    // Serial.println(value);
  }
  asyncHeartbeat();
}

// Hanlder
void handle(String module, int value) {
  blinkLed(indicatorLedPin, 2, 100);

  if (module == "led") {
    handleLedOnOff(redLedPin, value);
  } else if (module == "ledblink") {
    blinkLed(redLedPin, value, defaultBlinkDelay);
  } else if (module == "ledbrightness") {
    setLedBrightness(redLedPin, value);
  } 

  else if (module == "indicator") {
    handleLedOnOff(indicatorLedPin, value);
  } else if (module == "indicatorblink") {
    blinkLed(indicatorLedPin, value, defaultBlinkDelay);
  }

  else if (module == "motor") {
    handleMotorPower(value);
  } else if (module == "motorcw") {
    handleMotorRotation(value);
  } else if (module == "motorspeed") {
    motorSpeed(value);
  } 

  else if (module == "heartbeat") {
    setHeartbeat(value);
    if (value > 0) {
      turnOnHearbeat();
    } else {
      turnOffHeartbeat();
    }
  }

  else if (module == "step") {
    currentStep = value;
    if (value > 0) {
      value = map(value, 0, 3000, 0, 100);
      value = constrain(value, 0, 100);
      motorSpeed(value);
    } else {
      motorStop();
    }
  }
}

void handleLedOnOff(int ledPin, int value) {
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

void blinkLed(int ledPin, int count, int blinkDelay) {
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

void fadeInOutLed(int ledPin, int transitionDelay) {
  for (int brightness = 0; brightness <= 255; brightness += 1) {
    analogWrite(ledPin, brightness);
    delay(transitionDelay);
  }
  for (int brightness = 255; brightness >= 0; brightness -= 1) {
    analogWrite(ledPin, brightness);
    delay(transitionDelay);
  }
}

// Motor
void motorStop() {
  digitalWrite(motorControl1Pin, OFF);
  digitalWrite(motorControl2Pin, OFF);
}

void motorSpeed(int value) {
  value = map(value, 0, 100, 0, 255);
  value = constrain(value, 0, 255);
  analogWrite(motorSpeedPin, value);
}

void motorRotateClockwise() {
  digitalWrite(motorControl1Pin, OFF);
  digitalWrite(motorControl2Pin, ON);
}

void motorRotateCounterClockwise() {
  digitalWrite(motorControl1Pin, ON);
  digitalWrite(motorControl2Pin, OFF);
}

// Application
void turnOnHearbeat() {
  isHeartbeatOn = true;
}

void turnOffHeartbeat() {
  turnOffLed(redLedPin);
  isHeartbeatOn = false;
  currentBpm = 0;
}

void setHeartbeat(int bpm) {
  if (bpm <= 0) {
    return;
  }

  currentBpm = bpm;

  bpm = bpm / 2;
  unsigned int millisecondPerMinute = 1 * 60 * 1000;
  unsigned int millisecondPerBeat = millisecondPerMinute / bpm;

  if (bpm >= 100) {
    double onDelay = millisecondPerBeat / 8 * 7;
    heartbeatOffDelay = millisecondPerBeat / 8;
    heartbeatTransitionDelay = ceil(onDelay / (256 * 2));
    heartbeatTransitionDelay = heartbeatTransitionDelay - 1;
  } else {
    double onDelay = millisecondPerBeat / 4 * 3;
    heartbeatOffDelay = millisecondPerBeat / 4;
    heartbeatTransitionDelay = ceil(onDelay / (256 * 2));
  }
}

void asyncHeartbeat() {
  if (isHeartbeatOn == true) {

    if (isHeartbeatBrightnessAscending == true) {
      analogWrite(redLedPin, heartbeatCurrentBrightness);

      heartbeatCurrentBrightness += 1;
      if (heartbeatCurrentBrightness > 255) {
        isHeartbeatBrightnessAscending = false;
      }
    } else {
      analogWrite(redLedPin, heartbeatCurrentBrightness);

      heartbeatCurrentBrightness -= 1;
      if (heartbeatCurrentBrightness < 0) {
        delay(heartbeatOffDelay);
        isHeartbeatBrightnessAscending = true;
      }
    }
    delay(heartbeatTransitionDelay);
  }
}
