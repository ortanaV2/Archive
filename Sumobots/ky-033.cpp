int linetracking = 2;
int value;
   
void setup () {
  pinMode(linetracking, INPUT);
  digitalWrite(linetracking, HIGH);
  Serial.begin(9600);
  Serial.println("KY-033 Linetracking");
}
   
void loop () {
  value = digitalRead(linetracking);

  if (value == HIGH) {
    Serial.println("Line: True");
    delay(200);
  }
  if (value == LOW) {
    Serial.println("Line: False");
    delay(200);
  }
}
