void setup() {
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void freeze_all() {
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}

void forward(int time) {
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  delay(time);
  freeze_all();
}

void backward(int time) {
  digitalWrite(0, HIGH);
  digitalWrite(5, HIGH);
  delay(time);
  freeze_all();
}

void rotate(int time) {
  digitalWrite(2, HIGH);
  digitalWrite(5, HIGH);
  delay(time);
  freeze_all();
}

void forward_pwm(int time, int pwm) {
  analogWrite(2, pwm);
  analogWrite(4, pwm);
  delay(time);
  freeze_all();
}

void loop() {
  forward_pwm(1000, 225);
  freeze_all();
  delay(2000);
}
