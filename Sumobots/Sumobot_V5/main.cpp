#include <Arduino.h>

const int MOTOR_LEFT_FORWARD = 13;
const int MOTOR_LEFT_BACKWARD = 12;
const int MOTOR_RIGHT_FORWARD = 27;
const int MOTOR_RIGHT_BACKWARD = 14;

const int SENSOR_RIGHT = 26;
const int SENSOR_LEFT = 15;

int MOTOR_SPEED = 230;
const float LEFT_MOTOR_CALIB = 0;
const float RIGHT_MOTOR_CALIB = 4;

int RIGHT_LINE;
int LEFT_LINE;
bool IS_MOVING_FORWARD = false;

int lastLineStatus = -1;
unsigned long lastLineChangeTime = 0;

unsigned long lastDetectionTime = 0;
unsigned long currentDetectionTime = 0;
int detectionCounter = 0;

void freeze_all() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  IS_MOVING_FORWARD = false;
}

void brake_all() {
  analogWrite(MOTOR_LEFT_FORWARD, 255);
  analogWrite(MOTOR_LEFT_BACKWARD, 255);
  analogWrite(MOTOR_RIGHT_FORWARD, 255);
  analogWrite(MOTOR_RIGHT_BACKWARD, 255);
  IS_MOVING_FORWARD = false;
}

void setLeftMotor(bool forward, bool enable) {
  if (forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, MOTOR_SPEED - LEFT_MOTOR_CALIB);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, MOTOR_SPEED - LEFT_MOTOR_CALIB);
  } else {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  }
}

void setRightMotor(bool forward, bool enable) {
  if (forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, MOTOR_SPEED - RIGHT_MOTOR_CALIB);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, MOTOR_SPEED - RIGHT_MOTOR_CALIB);
  } else {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  }
}

void driveForward() {
  setLeftMotor(true, true);
  setRightMotor(true, true);
  IS_MOVING_FORWARD = true;
}

void driveBackward(int speed) {
  int previous = MOTOR_SPEED;
  MOTOR_SPEED = speed;
  setLeftMotor(false, true);
  setRightMotor(false, true);
  IS_MOVING_FORWARD = false;
  MOTOR_SPEED = previous;
}

void rotateLeft() {
  setLeftMotor(false, true);
  setRightMotor(true, true);
  IS_MOVING_FORWARD = false;
}

void rotateRight() {
  setLeftMotor(true, true);
  setRightMotor(false, true);
  IS_MOVING_FORWARD = false;
}

int getLineStatusFromValues(int left, int right) {
  bool l = left == HIGH;
  bool r = right == HIGH;
  return (l << 1) | r;
}

void checkUnchangedLineStatus(int currentStatus) {
  unsigned long now = millis();
  if (currentStatus != lastLineStatus) {
    lastLineStatus = currentStatus;
    lastLineChangeTime = now;
  } else {
    if (now - lastLineChangeTime >= 4000) {
      driveBackward(255);
      delay(300);
      if (random(0, 2)) {
        rotateLeft();
      } else {
        rotateRight();
      }
      delay(500);
      freeze_all();
      lastLineChangeTime = millis();
    }
  }
}

void rotateUntilNoLine(bool leftDir) {
  if (leftDir) {
    rotateLeft();
  } else {
    rotateRight();
  }
  while (digitalRead(SENSOR_LEFT) == HIGH || digitalRead(SENSOR_RIGHT) == HIGH) {
    delay(10);
  }
  delay(250);
  freeze_all();
}

void checkLoopBreaker(bool lineDetected) {
  if (lineDetected) {
    currentDetectionTime = millis();
    unsigned long delta = currentDetectionTime - lastDetectionTime;
    if (delta <= 1200) {
      detectionCounter++;
    } else {
      detectionCounter = 1;
    }
    lastDetectionTime = currentDetectionTime;
    if (detectionCounter >= 4) {
      driveBackward(255);
      delay(250);
      if (random(0, 2)) {
        rotateLeft();
      } else {
        rotateRight();
      }
      delay(500);
      freeze_all();
      delay(100);
      detectionCounter = 0;
    }
  }
}

void setup() {
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  freeze_all();
  delay(3000);
}

void loop() {
  RIGHT_LINE = digitalRead(SENSOR_RIGHT);
  LEFT_LINE = digitalRead(SENSOR_LEFT);

  if (LEFT_LINE == HIGH && RIGHT_LINE == LOW) {
    driveBackward(255);
    delay(300);
    freeze_all();
    rotateUntilNoLine(false);
  } else if (RIGHT_LINE == HIGH && LEFT_LINE == LOW) {
    driveBackward(255);
    delay(300);
    freeze_all();
    rotateUntilNoLine(true);
  } else if (RIGHT_LINE == HIGH && LEFT_LINE == HIGH) {
    driveBackward(255);
    delay(300);
    freeze_all();
  } else {
    driveForward();
  }

  checkLoopBreaker(LEFT_LINE == HIGH || RIGHT_LINE == HIGH);
  int currentLineStatus = getLineStatusFromValues(LEFT_LINE, RIGHT_LINE);
  checkUnchangedLineStatus(currentLineStatus);
  delay(5);
}
