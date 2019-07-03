#ifndef _SPARKFUN_QWIIC_RFID_H_
#define _SPARKFUN_QWIIC_RFID_H_

#include <Wire.h>
#include <Arduino.h>

#define DEFAULT_ADDR         0x7D
#define ALTERNATE_ADDR       0x7C
#define ADDRESS_LOCATION     0xC7 

#define TAG_REQUEST          6
#define TAG_SIZE             7 // Tag + null character
#define TAG_AND_TIME_SIZE    10 
#define TAG_AND_TIME_REQUEST 10

struct rfidData {

  String tag; 
  float time; 

};

// The Qwiic RFID holds at most 20 tags with their times. 
struct rfidDataArray {

  String tag[20]; 
  float time[20]; 

};

class Qwiic_Rfid
{  
  public:

    Qwiic_Rfid(uint8_t address); // I2C Constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

    // This function gets the RFID tag from the Qwiic RFID Reader. If there is not
    // tag then it will return an empty string. The way this is done is a bit odd.
    // When the function is called the tag is retrieved but saved to a global
    // struct variable set within the _readTagTime function. The tag is
    // saved to a local variable, wiped from the global variable, and then the
    // local varaibale is returned. This allows me to get both the tag and the
    // associated time the tag was scanned at the same time, while keeping the
    // function simple.
    String getTag();

    // This function gets the time the latest RFID tag was scanned from the Qwiic
    // RFID reader. If there is no tag then the time that is returned will be zero.
    // As mentioned above, the way this is done is not ideal. The information is
    // actually retrieved in the above function and is saved to a global struct
    // variable. The struct variable "time" is
    // saved to a local variable, wiped from the global variable, and then the
    // local varaibale is returned. This allows me to get both the tag and the
    // associated time the tag was scanned at the same time, while keeping the
    // function simple.
    uint32_t getTagTime();

    // This function gets the time the latest RFID tag was scanned from the Qwiic
    // RFID reader. If there is no tag then the time that is returned will be zero.
    // As mentioned above, the way this is done is not ideal. The information is
    // actually retrieved in the above function and is saved to a global struct
    // variable. The struct variable "time" is
    // saved to a local variable, wiped from the global variable, and then the
    // local varaibale is returned. This allows me to get both the tag and the
    // associated time the tag was scanned at the same time, while keeping the
    // function simple.
    float getTagTimePrecise();
      
    // This function gets the number of available tags available on the Qwiic RFID
    // Reader. 
    uint8_t getTagsAvailable();

    // This function gets all the available tags on teh Qwiic RFID Reader's buffer.
    // The buffer on the Qwiic RFID is 20 tags long or 200 bytes. 
    StringgetAllTags(String tagArray[]);

    // This function gets all the times associated with the available tags in the
    // buffer. 
    floatgetAllTimes(float timeArray[]);

    // This function changes the I-squared-C address of the Qwiic RFID. The address
    // is written to the memory location in EEPROM that determines its address.
    bool changeAddress(uint8_t newAddress);

  private:

    uint8_t _address;
    rfidData rfid;
    rfidDataArray rfidArray;

    // This function handles the I-squared-C transaction to get the RFID tag and
    // time from the Qwiic RFID Reader. What comes in from the RFID reader is a
    // number that was converted from a string to it's direct numerical
    // representation which then needs to be translated back. 
    void _readTagTime(uint8_t _numofReads);

    // Similar to the function above but with a larger scope, this function gets
    // all the available RFID tags and their scan times from the Qwiic RFID reader. 
    void _readAllTagsTimes(uint8_t _numofReads);

    TwoWire *_i2cPort;
};
#endif
