/*
  This is a library for...
  By: Elias Santistevan
  Date: 
  License: This code is public domain but you buy me a beer if you use this and 
  we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
 */

#include "SparkFun_Qwiic_Rfid.h"

Qwiic_Rfid::Qwiic_Rfid(uint8_t address){  _address = address; } // Constructor for I2C


bool Qwiic_Rfid::begin( TwoWire &wirePort )
{
  
  _i2cPort = &wirePort;


  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if( !_ret )
    return true; 
  else 
    return false; 

}

// This function gets the RFID tag from the Qwiic RFID Reader. If there is not
// tag then it will return an empty string. The way this is done is a bit odd.
// When the function is called the tag is retrieved but saved to a global
// struct variable set within the _readTagTime function. The tag is
// saved to a local variable, wiped from the global variable, and then the
// local varaibale is returned. This allows me to get both the tag and the
// associated time the tag was scanned at the same time, while keeping the
// function simple.
String Qwiic_Rfid::getTag(){

  String tempTag; 
  _readTagTime(TAG_AND_TIME_REQUEST); // Load up the global struct variable
  tempTag = rfid.tag; // Assign the tag to our local variable
  rfid.tag = " "; // Clear the global variable
  return tempTag; // Return the local variable. 

}


// This function gets the time the latest RFID tag was scanned from the Qwiic
// RFID reader. If there is no tag then the time that is returned will be zero.
// As mentioned above, the way this is done is not ideal. The information is
// actually retrieved in the above function and is saved to a global struct
// variable. The struct variable "time" is
// saved to a local variable, wiped from the global variable, and then the
// local varaibale is returned. This allows me to get both the tag and the
// associated time the tag was scanned at the same time, while keeping the
// function simple.
uint32_t Qwiic_Rfid::getTagTime(){

  uint32_t tempTime;   
  // Global struct variable is loaded from getTag function.
  tempTime = rfid.time; // Assign the time to the local variable
  rfid.time = 0.0; // Clear the global variable
  return tempTime; // Return the local variable. 

}

// This function gets the time the latest RFID tag was scanned from the Qwiic
// RFID reader. If there is no tag then the time that is returned will be zero.
// As mentioned above, the way this is done is not ideal. The information is
// actually retrieved in the above function and is saved to a global struct
// variable. The struct variable "time" is
// saved to a local variable, wiped from the global variable, and then the
// local varaibale is returned. This allows me to get both the tag and the
// associated time the tag was scanned at the same time, while keeping the
// function simple.
float Qwiic_Rfid::getTagTimePrecise(){

  float tempTime;   
  // Global struct variable is loaded from getTag function.
  tempTime = rfid.time; // Assign the time to the local varaible
  rfid.time = 0.0; // Clear the global variable 
  return tempTime; // Return the local variable

}

// This function gets the number of available tags available on the Qwiic RFID
// Reader. 
uint8_t Qwiic_Rfid::getTagsAvailable(){

  uint8_t numTags; 
  numTags = _i2cPort->available(); // How many bytes are available on the bus. 
  numTags = numTags/TAG_AND_TIME_SIZE; // Divide that by the size of the tag and it's read time. 
  return numTags; 

}

// This function gets all the available tags on teh Qwiic RFID Reader's buffer.
// The buffer on the Qwiic RFID is 20 tags long or 200 bytes. 
String* Qwiic_Rfid::getAllTags(String tagArray[]){
 
  uint8_t numTags; 
  numTags = getTagsAvailable(); // Get number of tags available
  _readAllTagsTimes(numTags); // Load up the global struct variables
  for(int i = 0; i < (sizeof(tagArray)); i++) { 
    tagArray[i] = rfidArray.tag[i]; //Load up passed array with tag
    rfidArray.tag[i] = " "; // Clear global variable
  }

  return tagArray;
}

// This function gets all the times associated with the available tags in the
// buffer. 
float* Qwiic_Rfid::getAllTimes(float timeArray[]){
 
  for(uint8_t i = 0; i < (sizeof(timeArray)/4); i++){
    timeArray[i] = rfidArray.time[i]; //Load up passed array with time
    rfidArray.time[i] = 0.0; // Clear global variable
  }

  return timeArray; 
}

// This function changes the I-squared-C address of the Qwiic RFID. The address
// is written to the memory location in EEPROM that determines its address.
bool Qwiic_Rfid::changeAddress(uint8_t newAddress) 
{

  if (newAddress < 0x07 || newAddress > 0x78) // Range of legal addresses
        return; 

  _i2cPort->beginTransmission( _address);  
  _i2cPort->write(ADDRESS_LOCATION);  
  _i2cPort->write(newAddress);  

  if(!Wire.endTransmission())
    return false; 
  else
    return true; 
}

// This function handles the I-squared-C transaction to get the RFID tag and
// time from the Qwiic RFID Reader. What comes in from the RFID reader is a
// number that was converted from a string to it's direct numerical
// representation which then needs to be translated back. 
void Qwiic_Rfid::_readTagTime(uint8_t _numofReads)
{

  char _tempTag[TAG_SIZE]; 
  uint32_t _tempTime; 

  _i2cPort->requestFrom(_address, _numofReads); 
  _tempTag[0] = char[](Wire.read()); 
  _tempTag[1] = char[](Wire.read());
  _tempTag[2] = char[](Wire.read());
  _tempTag[3] = char[](Wire.read());
  _tempTag[4] = char[](Wire.read());
  _tempTag[5] = char[](Wire.read());
  _tempTag[6] = '\0';
  Serial.println(_tempTag[1]);
  Serial.println(_tempTag[2]);
  Serial.println(_tempTag[3]);
  Serial.println(_tempTag[4]);
  Serial.println(_tempTag[5]);
  rfid.tag  = _tempTag; 
  
  _tempTime  = uint32_t(Wire.read() << 24); 
  _tempTime |= uint32_t(Wire.read() << 16); 
  _tempTime |= uint32_t(Wire.read() << 8); 
  _tempTime |= uint32_t(Wire.read()); 
  rfid.time = _tempTime/1000; // Convert to seconds
  

}

// Similar to the function above but with a larger scope, this function gets
// all the available RFID tags and their scan times from the Qwiic RFID reader. 
void Qwiic_Rfid::_readAllTagsTimes(uint8_t _numofReads)
{

  char _tempTag[TAG_SIZE]; 
  uint32_t _tempTime; 

  for(uint8_t i = 0; i < _numofReads; i++)
  {
    _i2cPort->requestFrom(_address, _numofReads); 
    _tempTag[0] = char(Wire.read());
    _tempTag[1] = char(Wire.read());
    _tempTag[2] = char(Wire.read());
    _tempTag[3] = char(Wire.read());
    _tempTag[4] = char(Wire.read());
    _tempTag[5] = char(Wire.read());
    _tempTag[6] = '\0';
    rfidArray.tag[i] = _tempTag; 
    
    _tempTime = uint32_t(Wire.read() << 24); 
    _tempTime |= uint32_t(Wire.read() << 16); 
    _tempTime |= uint32_t(Wire.read() << 8); 
    _tempTime |= uint32_t(Wire.read()); 
    rfidArray.time[i] = _tempTime/1000; // Convert to seconds
  } 

}
