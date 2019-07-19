#ifndef _SPARKFUN_QWIIC_RFID_H_
#define _SPARKFUN_QWIIC_RFID_H_

#include <Wire.h>
#include <Arduino.h>

#define DEFAULT_ADDR         0x7D
#define ALTERNATE_ADDR       0x7C
#define ADDRESS_LOCATION     0xC7 

#define TAG_AND_TIME_REQUEST 10
#define MAX_TAG_STORAGE      20
#define BYTES_IN_BUFFER      4

struct rfidData {

  String tag; 
  uint32_t time; 

};

class Qwiic_Rfid
{  
  public:

    Qwiic_Rfid(uint8_t address); // I-squared-C constructor

    bool begin(TwoWire &wirePort = Wire); // begin function

    // This function gets the RFID tag from the Qwiic RFID Reader. If there is not
    // tag then it will return an empty string. When the function is called the tag 
    // is retrieved but saved to a global struct variable set within the _readTagTime 
    // function. The tag is saved to a local variable, wiped from the global variable, and then the
    // local varaibale is returned. This allows me to get both the tag and the
    // associated time the tag was scanned at the same time, while keeping the
    // function simple.
    String getTag();

    // This function gets the time in seconds of the latest RFID tag was scanned from the Qwiic
    // RFID reader. If there is no tag then the time that is returned will be zero.
    // The information is received in the call to getTag() above, there is no time
    // without the variable, and stored in the data member time of the struct
    // rfidData. 
    int32_t getReqTime();

    // This function gets the time in seconds of the latest RFID tag was scanned from the Qwiic
    // RFID reader. If there is no tag then the time that is returned will be zero.
    // The information is received in the call to getTag() above, there is no time
    // without the variable, and stored in the data member time of the struct
    // rfidData. 
    float getPrecReqTime();

    // This function clear the buffer from the Qwiic RFID Reader by reading them
    // but not storing them.
    void clearTags();

    // This function gets all the available tags on teh Qwiic RFID Reader's buffer.
    // The buffer on the Qwiic RFID holds 20 tags and their scan time. Not knowing
    // how many are available until the i2c buffer is read, the parameter is 
    // a full 20 element array.
    void getAllTags(String tagArray[MAX_TAG_STORAGE]);

    // This function gets all the available scan times associated with the scanned RFID tags.
    // The buffer on the Qwiic RFID holds 20 tags and their scan time. Not knowing
    // how many are available until the I-squared-C buffer is read, the parameter is 
    // a full 20 element array. A note on the time. The time is not the time of day
    // when the tag was scanned but actually the time between when the tag was
    // scanned and when it was read from the I-squared-C bus. 
    void getAllTimes(int32_t timeArray[MAX_TAG_STORAGE]);

    // This function gets all the available scan times associated with the scanned RFID tags.
    // The buffer on the Qwiic RFID holds 20 tags and their scan time. Not knowing
    // how many are available until the I-squared-C buffer is read, the parameter is 
    // a full 20 element array. A note on the time. The time is not the time of day
    // when the tag was scanned but actually the time between when the tag was
    // scanned and when it was read from the I-squared-C bus. 
    void getAllPrecTimes(float timeArray[MAX_TAG_STORAGE]);

    // This function changes the I-squared-C address of the Qwiic RFID. The address
    // is written to the memory location in EEPROM that determines its address.
    bool changeAddress(uint8_t newAddress);

  private:

    uint8_t _address;

    rfidData _libRfid;  
    rfidData _libRfidArray[MAX_TAG_STORAGE];  

    // This function handles the I-squared-C transaction to get the RFID tag and
    // time from the Qwiic RFID Reader. What comes in from the RFID reader is a
    // number that was converted from a string to it's direct numerical
    // representation which then needs to be translated back. 
    void _readTagTime(uint8_t _numofReads);

    // This function differs
    // from the one above by populating an array of 20 elements that drains the
    // entire available rfid buffer on the Qwiif RFID Reader. Similar to the
    // function above it handles the I-squared-C transaction to get the RFID tags and
    // time from the Qwiic RFID Reader. What comes in from the RFID reader is a
    // number that was converted from a string to it's direct numerical
    // representation which is then converted back to its' original state. 
    void _readAllTagsTimes(uint8_t _numofReads);

    TwoWire *_i2cPort;
};
#endif
