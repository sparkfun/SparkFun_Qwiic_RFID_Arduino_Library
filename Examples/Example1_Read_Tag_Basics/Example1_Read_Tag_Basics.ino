/*
	This example gets the latest tag scanned  and it's associated time from the 
  Qwiic RFID Reader when the user enters "1" into the Serial Terminal. The
  "scan" time is not the time of day the RFID card was scanned but rather the
  time between when the card was scanned and when you the user requested the
  RFID tag from the Qwiic RFID Reader. 

	By: Elias Santistevan
	Sparkfun Electronics
	Date: July, 2019
	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15191

*/

#include <Wire.h> 
#include "SparkFun_Qwiic_Rfid.h"

#define RFID_ADDR 0x7D // Default I2C address 

Qwiic_Rfid myRfid(RFID_ADDR);

String tag; 
float scanTime;
int serialInput; 

void setup()
{
  // Begin I-squared-C
	Wire.begin(); 
	Serial.begin(115200); 

  if(myRfid.begin())
    Serial.println("Ready to scan some tags!"); 
  else
    Serial.println("Could not communicate with Qwiic RFID!"); 

  // Want to clear tags sitting on the Qwiic RFID card?
  //myRfid.clearTags();
}

void loop()
{
  if (Serial.available() > 0){

    serialInput = Serial.read(); 
    if (serialInput == 49){   // "1" on your keyboard is 49 in ASCII

      tag = myRfid.getTag();
      Serial.print("Tag ID: ");
      Serial.print(tag);
      scanTime = myRfid.getPrecReqTime(); 
      // If this time is too precise try: 
      // long time = myRfid.getReqTime(); 
      Serial.print(" Scan Time: ");
      Serial.println(scanTime);

    }

  }
}
