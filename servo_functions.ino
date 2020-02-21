// Servo Functions

void openVent() {
  ventServo.write(0);

  // determine which LED to turn on depending on if a command was received or not
  if(commandOverride) {
    digitalWrite(COMMAND_LED,HIGH);
    digitalWrite(AUTO_LED,LOW);
    Serial.println("Command opening...");
  }
  else {
    digitalWrite(AUTO_LED,HIGH);
    Serial.println("Auto opening...");
  }

  ventOpen = true;

  openStamp = millis();

}


void closeVent() {
  ventServo.write(170);

  commandOverride = false;
  ventOpen = false;

  Serial.println("Closing vent...");
  // turn off both LEDs
  digitalWrite(COMMAND_LED,LOW);
  digitalWrite(AUTO_LED,LOW);
  
}
