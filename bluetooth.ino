// Bluetooth Functions

String getCommand() {

  String commandString = "";
  byte incomingByte;

  while(ventBlue.available() > 0) {
    //Serial.println(ventBlue.available());
    incomingByte = ventBlue.read();
    Serial.println(incomingByte);
    if (incomingByte >= 32 && incomingByte <= 127) {
      commandString += (char)incomingByte; }
  }

  Serial.println(commandString);

  return commandString;
    
}


void action(String aCommand) {
  Serial.println(aCommand);

  ventBlue.write('R');
  
  if(aCommand == "O") {
    commandOverride = true;
    overrideStamp = millis();
    openVent();
  }
  else if(aCommand == "C") {
    closeVent();
  }
  else if(aCommand == "T") {
    cutResistorOn();
    Serial.println("Cutting...");
  }
  else if(aCommand == "P") {
    ventBlue.write(servoFeedback);
  }
  
}
