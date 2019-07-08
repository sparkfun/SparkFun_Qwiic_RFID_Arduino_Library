/*
  This example code demonstrates how to change the address of the Qwiic RFID
  Card Reader to one of your choosing. There is a set range of available
  addresses from 0x07 to 0x78, so make sure your chosen address falls within
  this range. The next thing to note is that when you change the address you'll
  need to call an instance of the Qwiic_Rfid class that includes your new
  address: "Qwiic_Rfid myRfid(YOUR_NEW_ADDRESS_HERE);" so that the new address
  is fed to all the available functions. Finally if for some reason you've
  forgotten your new address. No big deal, load up the I2C scanner example code
  and find out where your product's address is at. 
  By: Elias Santistevan
  SparkFun Electronics
  Date: July 2019

  License: This code is public domain but you buy me a beer if you use 
	this and we meet someday (Beerware license).
*/

#include <Wire.h>
#include "SparkFun_Qwiic_Rfid.h"


// Close the onboard jumper if you want to access the alternate addresses. 
#define RFID_DEFAULT_ADDRESS 0x7D // Alternate jumper address 0x7C


// After changing the address you'll need to apply that address to a new
// instance of the Qwiic_Rfid class: "Qwiic_Rfid myRfid(YOUR_NEW_ADDRESS_HERE)".
Qwiic_Rfid myRfid(RFID_DEFAULT_ADDRESS); // Change to Single Relay Address if using Quad Relay

byte newAddress = 0x09; // Change this to your desired addresss. 

void setup()
{
  Wire.begin(); 
  Serial.begin(115200);
  
  // Let's see.....
  if(myRfid.begin())
    Serial.println("Ready to scan some tags");
  else
    Serial.println("Check connections to Qwiic RFID Reader.");

  // There is a not so limited, but still limited range of
  // addresses that are available to you 0x07 -> 0x78.
  if(myRfid.changeAddress(newAddress)) // Put the address you want here. 
    Serial.println("Address changed successfully."); 
  else
    Serial.println("Address change failed...");
  
  delay(2000);

  
}

void loop()
{
}
