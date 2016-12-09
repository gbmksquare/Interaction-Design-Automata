#define ON HIGH
#define OFF LOW

const int indicatorLedPin = 13; // Digital
const int redLedPin = 9;        // Analog
const int motorSpeedPin = 5;    // Analog
const int motorControl1Pin = 6; // Analog
const int motorControl2Pin = 7; // Digital

const int defaultBlinkDelay = 300;

bool isMotorRunning = false;
bool isMotorClockwise = true;
int motorSpeed = 100;

bool isHeartbeatOn = false;
int currentBpm = 0;
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
    handleMotorOnOff(value);
  } else if (module == "motorcw") {
    handleMotorRotation(value);
  } else if (module == "motorspeed") {
    setMotorSpeed(value);
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
      value = constrain(value, 0, 3000);
      value = map(value, 0, 3000, 0, 100);
      value = constrain(value, 0, 100);
      Serial.println(value);
      startMotor();
      setMotorSpeed(value);
    } else {
      stopMotor();
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

void handleMotorOnOff(int value) {
  if (value == 1) {
    startMotor();
  } else if (value == 0) {
    stopMotor();
  }
}

void handleMotorRotation(int value) {
  if (value == 1) {
    rotateMotorClockwise();
  } else if (value == 0) {
    rotateMotorCounterClockwise();
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
void startMotor() {
  isMotorRunning = true;
  if (isMotorClockwise == true) {
    rotateMotorClockwise();
    setMotorSpeed(motorSpeed);
  } else {
    rotateMotorCounterClockwise();
    setMotorSpeed(motorSpeed);
  }
}

void stopMotor() {
  isMotorRunning = false;
  digitalWrite(motorControl1Pin, OFF);
  digitalWrite(motorControl2Pin, OFF);
}

void setMotorSpeed(int speed) {
  motorSpeed = speed;
  if (isMotorRunning == true) {
    if (speed >= 100) {
      speed = 255;
    } else if (speed <= 0) {
      speed = 0;
    } else {
      speed = map(speed, 1, 99, 51, 255);
    }
    analogWrite(motorSpeedPin, speed);
  }
}

void rotateMotorClockwise() {
  isMotorClockwise = true;
  if (isMotorRunning == true) {
    digitalWrite(motorControl1Pin, OFF);
    digitalWrite(motorControl2Pin, ON);
  }
}

void rotateMotorCounterClockwise() {
  isMotorClockwise = false;
  if (isMotorRunning == true) {
    digitalWrite(motorControl1Pin, ON);
    digitalWrite(motorControl2Pin, OFF);
  }
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

  double secondPerBeat = 60 / double(bpm);
  double transitionDelay = secondPerBeat / 512 * 1000 * 5;

  heartbeatOffDelay = 0;
  heartbeatTransitionDelay = transitionDelay;
}

void asyncHeartbeat() {
  if (isHeartbeatOn == true) {

    if (isHeartbeatBrightnessAscending == true) {
      analogWrite(redLedPin, heartbeatCurrentBrightness);

      heartbeatCurrentBrightness += 5;
      if (heartbeatCurrentBrightness >= 255) {
        isHeartbeatBrightnessAscending = false;
      }
    } else {
      analogWrite(redLedPin, heartbeatCurrentBrightness);

      heartbeatCurrentBrightness -= 5;
      if (heartbeatCurrentBrightness < 0) {
        delay(heartbeatOffDelay);
        isHeartbeatBrightnessAscending = true;
      }
    }
    delay(heartbeatTransitionDelay);
  }
}
