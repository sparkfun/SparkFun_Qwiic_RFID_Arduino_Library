/*

This example code demonstrates how to get every RFID Card off of the Qwiic
RFID reader. The Qwiic RFID Reader can hold up to 20 cards and their scan times at
a time. If more than 20 are read then the first card is overwritten. If you
expect this to be the case then getting the scan time will help to to see
when the RFID card was scanned. A brief note on time. This time is not the
time when the RFID card was scanned but the time between when the RFID card
was scanned and when it was requested by you the user. There are two time
functions available for large reads: getAllTimes() and getAllPrecTimes(),
both return time in seconds but the second option gives you a time with two
decimal point precision(0.00).  

By: Elias Santistevan
Sparkfun Electronics
Date: December, 2023
License: This code is public domain but if you use this and we meet someday, get me a beer! 

Feel like supporting our work? Buy a board from Sparkfun!
https://www.sparkfun.com/products/15191

*/

#include <Wire.h> 
#include "SparkFun_Qwiic_Rfid.h"

//#define RFID_ADDR 0x7C // Old Alternate I2C Address
//#define RFID_ADDR 0x7D // Old Default I2C Address
//#define RFID_ADDR 0x14 // Alternate I2C address 
#define RFID_ADDR 0x13 // Default I2C address 

#define MAX_RFID_STORAGE 20

String allTags[MAX_RFID_STORAGE]; 
float allTimes[MAX_RFID_STORAGE];
int serialInput;

Qwiic_Rfid myRfid(RFID_ADDR);

void setup()
{
    Wire.begin(); 
    Serial.begin(115200); 
    Serial.println("Example 3 - Get Tags and Their Scan Times");

    if(!myRfid.begin()){
        Serial.println("Could not communicate with Qwiic RFID, check you have the correct address!"); 
        while(1)
    }

    Serial.println("Ready to scan some tags!"); 

    // Using the Serial Terminal to initiate retrieval of the RFID cards and
    // their scan times.. 
    Serial.println("Enter '1' into ther Serial Terminal to retrieve all tags that have been scanned."); 
}
void loop()
{
    
    if (Serial.available() > 0) {

        serialInput = Serial.read();
        if (serialInput == 49){      // "1" on your keyboard in Ascii is 49. 
            // Fill the given tag and time arrays. 
            myRfid.getAllTags(allTags);
            myRfid.getAllPrecTimes(allTimes);

            for( int i = 0; i < MAX_RFID_STORAGE; i++) {
                Serial.print("RFID Tag: "); 
                Serial.print(allTags[i]);
                Serial.print(" Scan Time: ");
                Serial.println(allTimes[i]);
            }

        }  
    }

}
