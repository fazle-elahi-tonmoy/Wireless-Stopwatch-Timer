bool sonar_read() {
  if (millis() - timer > 30) {
    timer = millis();
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 15000);
    int distance = duration * 0.01715;  // Speed of sound in air is approximately 343 m/s
    // Serial.println(distance);
    if (distance >= 20 || distance <= 2) return 0;
    else return 1;
  }
  return detect;
}

