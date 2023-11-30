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
Date: December, 2023

License: This code is public domain but you buy me a beer if you use 
this and we meet someday (Beerware license).
*/

#include <Wire.h>
#include "SparkFun_Qwiic_Rfid.h"


//#define RFID_ADDR 0x7C // Old Alternate I2C Address
//#define RFID_ADDR 0x7D // Old Default I2C Address
//#define RFID_ADDR 0x14 // Alternate I2C address 
#define RFID_ADDR 0x13 // Default I2C address 


// After changing the address you'll need to apply that address to a new
// instance of the Qwiic_Rfid class: "Qwiic_Rfid myRfid(YOUR_NEW_ADDRESS_HERE)".
Qwiic_Rfid myRfid(RFID_ADDR); // Change to Single Relay Address if using Quad Relay

byte newI2CAddress = 0x09; // Change this to your desired addresss. 

void setup()
{
    Wire.begin(); 
    Serial.begin(115200); 
    Serial.println("Example 4 - Change RFID I2C Address"); 

    if(!myRfid.begin()){
        Serial.println("Could not communicate with Qwiic RFID, check you have the correct address!"); 
        while(1)
    }

    Serial.println("Ready to scan some tags!"); 

    // There is a not so limited, but still limited range of
    // addresses that are available to you 0x07 -> 0x78.
    if(!myRfid.changeAddress(newI2CAddress)) // Put the address you want here. 
        Serial.println("Address change failed...");

    Serial.println("Address change successful!");

    while(1);

  
}

void loop()
{
}
