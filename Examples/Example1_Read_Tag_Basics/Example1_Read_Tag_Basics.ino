/*
	SparkFun Qwiic RFID board fot the ID-xxLA Modules
	By: Elias Santistevan
	Sparkfun Electronics
	Date: February, 2018
	License: This code is public domain but if you use this and we meet someday, get me a beer! 

	Feel like supporting our work? Buy a board from Sparkfun!
	https://www.sparkfun.com/products/15191

	This example requests an RFID tag's ID when the interrupt pin from the
  SparkFun Qwiic RFID board goes LOW indicating a tag has been scanned. 
*/

#include <Wire.h> 
#include "SparkFun_Qwiic_Rfid.h"

#define RFID_ADDR 0x7D // Default I2C address 

Qwiic_Rfid myRfid(RFID_ADDR);

void setup()
{
  // Begin I-squared-C
	Wire.begin(); 
	Serial.begin(115200); 

  if(!myRfid.begin())
    Serial.println("Could not communicate with Qwiic RFID!"); 
  else
    Serial.println("Ready to scan some tags!"); 
  
  delay(3000);
  String tag = myRfid.getTag();
  Serial.println(tag);

}
void loop()
{
}
