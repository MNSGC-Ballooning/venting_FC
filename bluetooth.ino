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
  if(aCommand == "OPEN") {
    commandOverride = true;
    overrideStamp = millis();
    openVent();
  }
  else if(aCommand == "CLOSE") {
    closeVent();
  }
  else if(aCommand == "CUT") {
    cutResistorOn();
    Serial.println("Cutting...");
  }
  else if(aCommand == "PING") {
    ventBlue.write("PiNG");
  }

  ventBlue.write("Received");
  
}
