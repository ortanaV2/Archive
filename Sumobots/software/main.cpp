// Motor-Pins
const int MOTOR_LEFT_FORWARD = 2;
const int MOTOR_LEFT_BACKWARD = 0;
const int MOTOR_RIGHT_FORWARD = 4;
const int MOTOR_RIGHT_BACKWARD = 5;

// Sensors
const int SENSOR_RIGHT = 12;
const int SENSOR_LEFT = 14;

int right_line;
int left_line;

void setup() {
  // Sensor setup
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);

  // Motor setup
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  Serial.begin(9600);
  freeze_all();
}

// Alle Motoren stoppen
void freeze_all() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
}

// Sichere Einzelsteuerung für linken Motor
void setLeftMotor(bool forward, bool enable) {
  digitalWrite(MOTOR_LEFT_FORWARD, (forward && enable) ? HIGH : LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, (!forward && enable) ? HIGH : LOW);
}

// Sichere Einzelsteuerung für rechten Motor
void setRightMotor(bool forward, bool enable) {
  digitalWrite(MOTOR_RIGHT_FORWARD, (forward && enable) ? HIGH : LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, (!forward && enable) ? HIGH : LOW);
}

// Kombinationen
void driveForward(int time) {
  setLeftMotor(true, true);
  setRightMotor(true, true);
  delay(time);
  freeze_all();
}

void driveBackward(int time) {
  setLeftMotor(false, true);
  setRightMotor(false, true);
  delay(time);
  freeze_all();
}

void rotateLeft(int time) {
  setLeftMotor(false, true);
  setRightMotor(true, true);
  delay(time);
  freeze_all();
}

void rotateRight(int time) {
  setLeftMotor(true, true);
  setRightMotor(false, true);
  delay(time);
  freeze_all();
}

void loop() {
  right_line = digitalRead(SENSOR_RIGHT);
  left_line = digitalRead(SENSOR_LEFT);

  if (left_line == HIGH && right_line == LOW) {
    rotateRight(250);
    delay(10);
  }

  else if (right_line == HIGH && left_line == LOW) {
    rotateLeft(250);
    delay(10);
  }

  else if (right_line == HIGH && left_line == HIGH) {
    rotateRight(500);
    delay(10);
  }

  else {
    driveForward(50);
    delay(10);
  }
}
