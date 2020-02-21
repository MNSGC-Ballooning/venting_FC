// SD functions

void setupSD() {
  pinMode(10, OUTPUT);                                      //Needed for SD library, regardless of shield used
  pinMode(chipSelect, OUTPUT);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect))                                //Attempt to start SD communication
    Serial.println("Card failed, not present, or voltage supply is too low.");          //Print out error if failed; remind user to check card
  else {                                                    //If successful, attempt to create file
    Serial.println("Card initialized successfully.\nCreating File...");
    for (byte i = 0; i < 100; i++) {                        //Can create up to 100 files with similar names, but numbered differently
      fileName[4] = '0' + i / 10;
      fileName[5] = '0' + i % 10;
      if (!SD.exists(fileName)) {                           //If a given filename doesn't exist, it's available
        dataLog = SD.open(fileName, FILE_WRITE);            //Create file with that name
        SDActive = true;                                    //Activate SD logging since file creation was successful
        Serial.println("Logging to: " + String(fileName));  //Tell user which file contains the data for this run of the program
        break;                                              //Exit the for loop now that we have a file
      }
    }
    if (!SDActive) Serial.println("No available file names; clear SD card to enable logging");
  }
  String header = "millis,seconds,blueStamp,cutStamp,autoStamp,openStamp,overrideStamp,cutterOn,commandOverride,ventOpen,servoFeedback,actionCommand";
  if (SDActive) {
    dataLog.println(header);
    dataLog.close();
  }
}


void printSD() {  
  String data = "";

  data = String(millis()/(float)1000, 3) + "," + String(millis()) + ",";     // time keeping
  data += String(blueStamp) + "," + String(cutStamp) + "," + String(autoStamp) + "," + String(openStamp) + "," + String(overrideStamp) + ",";   // time stamps of processes
  data += String(cutterOn) + "," + String(commandOverride) + "," + String(ventOpen) + "," + String(servoFeedback) + ",";    // states of the software
  data += String(actionCommand);    // most recent command received

  Serial.println(data); 

  if (SDActive) {
    dataLog = SD.open(fileName, FILE_WRITE);
    dataLog.println(data);                                //Takes serial monitor data and adds to SD card
    dataLog.close();                                      //Close file afterward to ensure data is saved properly
  } 
}
