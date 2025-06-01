const int MOTOR_LEFT_FORWARD = 5;
const int MOTOR_LEFT_BACKWARD = 4;
const int MOTOR_RIGHT_FORWARD = 2;
const int MOTOR_RIGHT_BACKWARD = 0;

const int SENSOR_RIGHT = 12;
const int SENSOR_LEFT = 14;

// global speed (0-255)
const int motor_speed = 230;
const int left_motor_calib = 7;
const int right_motor_calib = 0;

int right_line;
int left_line;
bool isMovingForward = false;
bool wasOnBothLines = false;

void setup() {
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);

  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  Serial.begin(9600);
  freeze_all();
}

void freeze_all() {
  analogWrite(MOTOR_LEFT_FORWARD, 0);
  analogWrite(MOTOR_LEFT_BACKWARD, 0);
  analogWrite(MOTOR_RIGHT_FORWARD, 0);
  analogWrite(MOTOR_RIGHT_BACKWARD, 0);
}

void setLeftMotor(bool forward, bool enable) {
  if (forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, motor_speed - left_motor_calib);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, motor_speed - left_motor_calib);
  } else {
    analogWrite(MOTOR_LEFT_FORWARD, 0);
    analogWrite(MOTOR_LEFT_BACKWARD, 0);
  }
}

void setRightMotor(bool forward, bool enable) {
  if (forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, motor_speed - right_motor_calib);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  } else if (!forward && enable) {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, motor_speed - right_motor_calib);
  } else {
    analogWrite(MOTOR_RIGHT_FORWARD, 0);
    analogWrite(MOTOR_RIGHT_BACKWARD, 0);
  }
}

void driveForward() {
  setLeftMotor(true, true);
  setRightMotor(true, true);
  isMovingForward = true;
}

void driveBackward() {
  setLeftMotor(false, true);
  setRightMotor(false, true);
  isMovingForward = false;
}

void stopMoving() {
  freeze_all();
  isMovingForward = false;
}

void rotateLeft() {
  setLeftMotor(false, true);
  setRightMotor(true, true);
}

void rotateRight() {
  setLeftMotor(true, true);
  setRightMotor(false, true);
}

void loop() {
  right_line = digitalRead(SENSOR_RIGHT);
  left_line = digitalRead(SENSOR_LEFT);

  Serial.print("L: ");
  Serial.print(left_line);
  Serial.print(" R: ");
  Serial.println(right_line);

  if (left_line == HIGH && right_line == LOW) {
    stopMoving();
    driveBackward();
    delay(300);
    stopMoving();
    rotateRight();
    delay(200);
    freeze_all();
  } 
  else if (right_line == HIGH && left_line == LOW) {
    stopMoving();
    driveBackward();
    delay(300);
    stopMoving();
    rotateLeft();
    delay(200);
    freeze_all();
  } 
  else if (right_line == HIGH && left_line == HIGH) {
    driveBackward();
    delay(300);
    freeze_all();
    wasOnBothLines = true;
  } 
  else {
    if (wasOnBothLines) {
      stopMoving();
      if (random(0, 2) == 0) {
        rotateLeft();
      } else {
        rotateRight();
      }
      delay(300);
      freeze_all();
      wasOnBothLines = false;
    }
    if (!isMovingForward) {
      driveForward();
    }
  }

  delay(10);
}
