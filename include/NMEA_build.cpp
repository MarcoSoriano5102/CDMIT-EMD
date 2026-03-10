/**************************************************************************/
/*!
  @file NMEA_build.cpp

  This is the Adafruit GPS library - the ultimate GPS library
  for the ultimate GPS module!

  Tested and works great with the Adafruit Ultimate GPS module
  using MTK33x9 chipset
      ------> http://www.adafruit.com/products/746
  Pick one up today at the Adafruit electronics shop
  and help support open source hardware & software! -ada

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  @author Limor Fried/Ladyada  for Adafruit Industries.

  @copyright BSD license, check license.txt for more information
  All text above must be included in any redistribution
*/
/**************************************************************************/

#include "AdafruitGPS.hpp"

/**************************************************************************/
/*!
    @brief Add *CS where CS is the two character hex checksum for all but
    the first character in the string. The checksum is the result of an
    exclusive or of all the characters in the string. Also useful if you
    are creating new PMTK strings for controlling a GPS module and need a
    checksum added.
    @param buff Pointer to the string, which must be long enough
    @return none
*/
/**************************************************************************/
void AdafruitGPS::addCheckSum(char *buff) {
  char cs = 0;
  int i = 1;
  while (buff[i]) {
    cs ^= buff[i];
    i++;
  }

  // Calculate the needed buffer size: original length + 3 (*XX) + 1 (null
  // terminator)
  int neededSize = strlen(buff) + 4;
  char *tempBuffer = (char *)malloc(neededSize);

  if (tempBuffer != NULL) {
    // Use snprintf to safely format the string with the checksum
    snprintf(tempBuffer, neededSize, "%s*%02X", buff, cs);

    // Copy the formatted string back to the original buffer
    // Note: Make sure the original buffer is large enough to hold the new
    // string.
    strcpy(buff, tempBuffer);

    // Free the allocated memory to avoid memory leaks
    free(tempBuffer);
  }
}