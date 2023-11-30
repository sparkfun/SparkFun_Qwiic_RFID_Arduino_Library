/*
  This library is for the SparkFun Qwiic RFID Card Reader. It provides functions
  to retrieve RFID Card IDs that have been scanned by the Qwiic RFID Reader and
  their times. By: Elias Santistevan Date: July 2019 License: This code is
  public domain but you buy me a beer if you use this and we meet someday
  (Beerware license).

  Feel like supporting our work? Buy one from SparkFun Electronics!
  https://www.sparkfun.com/products/15191
 */

#include "SparkFun_Qwiic_Rfid.h"

Qwiic_Rfid::Qwiic_Rfid(uint8_t address) {
  _address = address;
} // Constructor for I2C

bool Qwiic_Rfid::begin(TwoWire &wirePort) // Begin, but not Wire.begin() =).
{

  _i2cPort = &wirePort;

  // Check for successful response: "O".
  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if (!_ret)
    return true;
  else
    return false;
}

String Qwiic_Rfid::getTag() {

  // Call the read command that will fill the global struct variable: rfidData
  _readTagTime(TAG_AND_TIME_REQUEST);

  String tempTag = _libRfid.tag; // Assign the tag to our local variable
  _libRfid.tag = "";             // Clear the global variable
  return tempTag;                // Return the local variable.
}

int32_t Qwiic_Rfid::getReqTime() {

  // Global struct variable is loaded from getTag function. There is no time
  // without a tag scan.
  int32_t tempTime = _libRfid.time; // Assign the time to the local variable
  _libRfid.time = 0;                // Clear the global variable
  return tempTime / 1000;           // Return the local variable in seconds.
}

float Qwiic_Rfid::getPrecReqTime() {

  // Global struct variable is loaded from getTag function. There is no time
  // without a tag scan.
  float tempTime =
      float(_libRfid.time) / 1000; // Assign the time to the local variable
  _libRfid.time = 0;               // Clear the global variable
  return tempTime;                 // Return the local variable in seconds.
}

void Qwiic_Rfid::clearTags() { _readAllTagsTimes(MAX_TAG_STORAGE); }

void Qwiic_Rfid::getAllTags(String tagArray[MAX_TAG_STORAGE]) {

  // Load up the global struct variables
  _readAllTagsTimes(MAX_TAG_STORAGE);

  for (uint8_t i = 0; i < MAX_TAG_STORAGE; i++) {
    tagArray[i] = _libRfidArray[i].tag; // Load up passed array with tag
    _libRfidArray[i].tag = "";          // Clear global variable
  }
}

void Qwiic_Rfid::getAllTimes(int32_t timeArray[MAX_TAG_STORAGE]) {

  for (uint8_t i = 0; i < MAX_TAG_STORAGE; i++) {
    timeArray[i] = (_libRfidArray[i].time /
                    1000);     // Load up passed array with time in seconds
    _libRfidArray[i].time = 0; // Clear global variable
  }
}

void Qwiic_Rfid::getAllPrecTimes(float timeArray[MAX_TAG_STORAGE]) {

  for (uint8_t i = 0; i < MAX_TAG_STORAGE; i++) {
    timeArray[i] =
        _libRfidArray[i].time; // Load up passed array with time in seconds
    timeArray[i] = timeArray[i] / 1000;
    _libRfidArray[i].time = 0; // Clear global variable
  }
}

bool Qwiic_Rfid::changeAddress(uint8_t newAddress) {

  if (newAddress < 0x07 || newAddress > 0x78) // Range of legal addresses
    return false;

  _i2cPort->beginTransmission(_address);
  _i2cPort->write(ADDRESS_LOCATION);
  _i2cPort->write(newAddress);

  if (!_i2cPort->endTransmission())
    return true;
  else
    return false;
}

void Qwiic_Rfid::_readTagTime(uint8_t _numofReads) {

  String _tempTag;
  int32_t _tempTime;

  _i2cPort->requestFrom(_address, static_cast<uint8_t>(_numofReads));

  // What is read from the buffer is immediately converted to a string and
  // cocatenated onto the temporary variable.
  _tempTag = String(_i2cPort->read());
  _tempTag += String(_i2cPort->read());
  _tempTag += String(_i2cPort->read());
  _tempTag += String(_i2cPort->read());
  _tempTag += String(_i2cPort->read());
  _tempTag += String(_i2cPort->read());

  // The tag is copied to the tag data member of the rfidData struct.
  _libRfid.tag = _tempTag;

  // Bring in the time.
  if (_libRfid.tag == "000000") { // Blank tag.

    // Time is zero if there is not a tag.
    _tempTime = 0;

    // Clear the buffer of the four bytes that would hold a time if there
    // was a time to read.
    _i2cPort->read();
    _i2cPort->read();
    _i2cPort->read();
    _i2cPort->read();
  }

  else {
    _tempTime = int32_t(_i2cPort->read() << 24);
    _tempTime |= int32_t(_i2cPort->read() << 16);
    _tempTime |= int32_t(_i2cPort->read() << 8);
    _tempTime |= int32_t(_i2cPort->read());
  }

  // Time is copied to the time data member of the rfidData struct.
  _libRfid.time = _tempTime; // Time in milliseconds
}

void Qwiic_Rfid::_readAllTagsTimes(uint8_t _numofReads) {

  String _tempTag;
  int32_t _tempTime;

  for (uint8_t i = 0; i < _numofReads; i++) {
    //
    // What is read from the buffer is immediately converted to a string and
    // cocatenated onto the temporary variable.
    _i2cPort->requestFrom(_address, static_cast<uint8_t>(TAG_AND_TIME_REQUEST));
    _tempTag = String(_i2cPort->read());
    _tempTag += String(_i2cPort->read());
    _tempTag += String(_i2cPort->read());
    _tempTag += String(_i2cPort->read());
    _tempTag += String(_i2cPort->read());
    _tempTag += String(_i2cPort->read());

    // The tag is copied to the tag data member of the rfidData struct.
    _libRfidArray[i].tag = _tempTag;

    // Bring in the time but only if there is a tag.....
    if (_libRfidArray[i].tag == "000000") { // Blank tag.

      // Time is zero since there is not tag.
      _tempTime = 0;

      // Clear the buffer...
      _i2cPort->read();
      _i2cPort->read();
      _i2cPort->read();
      _i2cPort->read();
    }

    else {
      // If there is tag bring in the time.
      _tempTime = int32_t(_i2cPort->read() << 24);
      _tempTime |= int32_t(_i2cPort->read() << 16);
      _tempTime |= int32_t(_i2cPort->read() << 8);
      _tempTime |= int32_t(_i2cPort->read());
    }

    // Time is copied to the time data member of the rfidData struct.
    _libRfidArray[i].time = _tempTime; // Convert to seconds
  }
}
