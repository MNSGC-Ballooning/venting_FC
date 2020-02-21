// Script for flight computer of the balloon venting system

#include <Servo.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define FEEDBACK_PIN A0
#define CUTTER_PIN 5
#define AUTO_LED 6
#define COMMAND_LED 7
#define chipSelect 8
#define PWM_PIN 9

#define BLUE_INTERVAL 100
#define SEQUENCE_INTERVAL 1000
#define CUT_INTERVAL 45000
#define AUTO_INTERVAL 300000
#define OPEN_INTERVAL 60000
#define OVERRIDE_INTERVAL 600000

// timers
unsigned long blueStamp = 0;
unsigned long cutStamp = 0;
unsigned long sequenceStamp = 0;
unsigned long autoStamp = 0;
unsigned long openStamp = 0;
unsigned long overrideStamp = 0;

// SD
File dataLog;                     //File object for datalogging
char fileName[] = "Vent00.csv";   //Template for file name to save data
bool SDActive = false;            //Used to check for SD card before attempting to log data

// Servo
Servo ventServo;
unsigned int serialByte;
float servoFeedback;

// Bluetooth
SoftwareSerial ventBlue(2,3);
int incoming = 0;
String actionCommand;

boolean cutterOn = false;
boolean ventOpen = false;
boolean commandOverride = false;


void setup() {
  Serial.begin(9600);           // initialize serial monitor

  ventBlue.begin(9600);         // initialize bluetooth moduel
  
  ventServo.attach(PWM_PIN);    // initialze servo

  setupSD();                    // initialize SD

  pinMode(AUTO_LED,OUTPUT);     // initialize LEDs
  pinMode(COMMAND_LED,OUTPUT); 
  
  pinMode(CUTTER_PIN,OUTPUT);   // initialize digital line to H-Driver

  closeVent();                  // ensure that the vent is closed at the beginning of the flight
  
}

void loop() {

  // read data from the bluetooth ever 100ms
  if (millis() - blueStamp > BLUE_INTERVAL) { 
    blueStamp = millis();
    
    if (ventBlue.available() > 0) {
      delay(50);
      actionCommand = "";
      actionCommand = getCommand();
      action(actionCommand);
    }
  }

  // run all other functions every second to ensure bluetooth rx buffer has enough time to fill
  if (millis() - sequenceStamp > SEQUENCE_INTERVAL) {
    servoFeedback = analogRead(FEEDBACK_PIN);
    
    sequenceStamp = millis();

    printSD();    // print data to the SD card

    // check to see if the resistor cutter should turned off
    if (millis() - cutStamp > CUT_INTERVAL && cutterOn) {
      cutResisitorOff();
    }

    // check to see if the vent should be automatically opened
    if (millis() - autoStamp > AUTO_INTERVAL && !ventOpen) {
      autoStamp = millis();
      openVent();
    }

    // check to see if the vent should be closed
    if (millis() - openStamp > OPEN_INTERVAL && ventOpen) {
      if (!commandOverride) {
        closeVent();
      }
      // if currently in an override state, wait longer to close the vent (unless a new command is sent)
      else if(millis() - overrideStamp > OVERRIDE_INTERVAL) {
        closeVent();
      }
    }
  }
}
