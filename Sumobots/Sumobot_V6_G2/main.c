const int MOTOR_LEFT_FORWARD = 26;
const int MOTOR_LEFT_BACKWARD = 25;
const int MOTOR_RIGHT_FORWARD = 33;
const int MOTOR_RIGHT_BACKWARD = 32;

const int SENSOR_RIGHT = 13;
const int SENSOR_LEFT = 14;

const int LED_BOOT = 2;  

int MOTOR_SPEED = 210;              
const float LEFT_MOTOR_CALIB = 0;
const float RIGHT_MOTOR_CALIB = 0;

int RIGHT_LINE; 
int LEFT_LINE;
bool IS_MOVING_FORWARD = false;

unsigned long lastTriggerLeft = 0;
unsigned long lastTriggerRight = 0;
int triggerCountLeft = 0;
int triggerCountRight = 0;
const unsigned long COOLDOWN = 1000; 
const int TRIGGER_LIMIT = 2;         

bool prevLeftLine = LOW;
bool prevRightLine = LOW;

int rotateSkipCounter = 0;

void setup() {
  
  pinMode(LED_BOOT, OUTPUT);
  digitalWrite(LED_BOOT, HIGH);
  delay(300);
  digitalWrite(LED_BOOT, LOW);

  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  analogWriteFrequency(MOTOR_LEFT_FORWARD, 20000); 
  analogWriteFrequency(MOTOR_LEFT_BACKWARD, 20000); 
  analogWriteFrequency(MOTOR_RIGHT_FORWARD, 20000); 
  analogWriteFrequency(MOTOR_RIGHT_BACKWARD, 20000);

  Serial.begin(9600);
  freeze_all();
  randomSeed(analogRead(0));
}

void freeze_all() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  IS_MOVING_FORWARD = false;
}

void setLeftMotor(bool forward, bool enable, int speed = -1) {
  if (speed < 0) speed = MOTOR_SPEED;
  if (speed > 255) speed = 255;

  if (forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, speed - LEFT_MOTOR_CALIB);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, speed - LEFT_MOTOR_CALIB);
  } else {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  }
}

void setRightMotor(bool forward, bool enable, int speed = -1) {
  if (speed < 0) speed = MOTOR_SPEED;
  if (speed > 255) speed = 255;

  if (forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, speed - RIGHT_MOTOR_CALIB);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, speed - RIGHT_MOTOR_CALIB);
  } else {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  }
}

void driveForward(int speed = -1) {
  setLeftMotor(true, true, speed);
  setRightMotor(true, true, speed);
  IS_MOVING_FORWARD = true;
}

void driveBackward(int speed = 255) {
  setLeftMotor(false, true, speed);
  setRightMotor(false, true, speed);
  IS_MOVING_FORWARD = false;
}

void momentumRotate(bool leftDir, int durationMs = -1) {
  if (durationMs < 0) durationMs = random(200, 300);
  if (leftDir) {
    setLeftMotor(false, false);
    setRightMotor(false, true, MOTOR_SPEED);
  } else {
    setLeftMotor(false, true, MOTOR_SPEED);
    setRightMotor(false, false);
  }
  delay(durationMs);
  freeze_all();
}

void maybeRotateAfterReverse() {
  if (rotateSkipCounter > 0) {
    rotateSkipCounter--;
    return;
  }

  int chance = random(0, 100);
  if (chance < 20) {
    bool turnLeft = random(0, 2) == 0;
    if (turnLeft) {
      setLeftMotor(false, true, 240);
      setRightMotor(true, true, 240);
    } else {
      setLeftMotor(true, true, 240);
      setRightMotor(false, true, 240);
    }
    delay(2000);
    freeze_all();
    delay(200);
    rotateSkipCounter = 3;
  }
}

void loopBreaker(int leftLine, int rightLine) {
  unsigned long now = millis();

  if (leftLine == HIGH && prevLeftLine == LOW) {
    if (now - lastTriggerLeft < COOLDOWN) {
      triggerCountLeft++;
    } else {
      triggerCountLeft = 1;
    }
    lastTriggerLeft = now;
  }

  if (rightLine == HIGH && prevRightLine == LOW) {
    if (now - lastTriggerRight < COOLDOWN) {
      triggerCountRight++;
    } else {
      triggerCountRight = 1;
    }
    lastTriggerRight = now;
  }

  if (triggerCountLeft > TRIGGER_LIMIT) {
    driveBackward(255);
    delay(100);   
    momentumRotate(true);
    maybeRotateAfterReverse();
    triggerCountLeft = 0;
    triggerCountRight = 0;
  }

  if (triggerCountRight > TRIGGER_LIMIT) {
    driveBackward(255);
    delay(250);   
    momentumRotate(false);
    maybeRotateAfterReverse();
    triggerCountLeft = 0;
    triggerCountRight = 0;
  }

  prevLeftLine = leftLine;
  prevRightLine = rightLine;
}

void loop() {
  RIGHT_LINE = digitalRead(SENSOR_RIGHT);
  LEFT_LINE = digitalRead(SENSOR_LEFT);

  loopBreaker(LEFT_LINE, RIGHT_LINE);

  if (LEFT_LINE == HIGH && RIGHT_LINE == LOW) {
    driveBackward(240);
    delay(600);
    momentumRotate(false);
    maybeRotateAfterReverse();
  } else if (RIGHT_LINE == HIGH && LEFT_LINE == LOW) {
    driveBackward(240);
    delay(600);
    momentumRotate(true);
    maybeRotateAfterReverse();
  } else if (RIGHT_LINE == HIGH && LEFT_LINE == HIGH) {
    driveBackward(240);
    delay(600);
    freeze_all();
  } else {
    driveForward();
  }

  delay(5);
}
