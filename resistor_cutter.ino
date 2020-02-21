// Resistor Cutter functions

void cutResistorOn() {
  digitalWrite(CUTTER_PIN, HIGH);

  cutterOn = true;
  cutStamp = millis();
}


void cutResisitorOff() {
  digitalWrite(CUTTER_PIN, LOW);

  Serial.println("Cutter off...");

  cutterOn = false;
}
