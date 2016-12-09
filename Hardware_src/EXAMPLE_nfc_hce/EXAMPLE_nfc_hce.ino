#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532Interface.h>
#include <PN532.h>

PN532_SPI pn532spi(SPI, 5);
PN532 nfc(pn532spi);

#define SENTINEL_BYTE 0x00
#define MAX_TOKEN_LENGTH 255

void setup()
{    
    Serial.begin(115200);
    Serial.println("-------Peer to Peer HCE--------");
    
    nfc.begin();
    
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (! versiondata) {
      Serial.print("Didn't find PN53x board");
      while (1); // halt
    }
    
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
    Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
    Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
    
    // Set the max number of retry attempts to read from a card
    // This prevents us from waiting forever for a card, which is
    // the default behaviour of the PN532.
    //nfc.setPassiveActivationRetries(0xFF);
    
    // configure board to read RFID tags
    nfc.SAMConfig();
}

void loop()
{
  bool success;
  
  Serial.println("Waiting for an ISO14443A card");
  
  // set shield to inListPassiveTarget
  success = nfc.inListPassiveTarget();

  if(success) {

    //bool dataCompletelyReceived = false;
    Serial.println("Found something!");

      uint8_t initMsg[] = { 0x00, /* CLA */
                               0xA4, /* INS */
                               0x04, /* P1  */
                               0x00, /* P2  */
                               0x08, /* Length of AID  */
                               0xF7, 0xA5, 0x39, 0x75, 0x8A, 0xB0, 0x00, 0x7F, /* AID defined on Android App */
                               0xF4  /* Le  */ };
      
      uint8_t data[MAX_TOKEN_LENGTH];
      uint8_t dataLen = MAX_TOKEN_LENGTH;

      success = nfc.inDataExchange(initMsg, sizeof(initMsg), data, &dataLen);

      if (success) {
        Serial.print("Data length: "); Serial.println(dataLen);
        nfc.PrintHexChar(data, dataLen);

        uint8_t temp[0xF4];
        dataLen = MAX_TOKEN_LENGTH;

        success = nfc.inDataExchange(initMsg, sizeof(initMsg), temp, &dataLen);

        if (success) {
          Serial.print("Second data length: "); Serial.println(dataLen);
          nfc.PrintHexChar(temp, dataLen);

          uint8_t finalMsg[] = { 0x00, /* CLA */
                               0xA4, /* INS */
                               0x04, /* P1  */
                               0x00, /* P2  */
                               0x08, /* Length of AID  */
                               0xF7, 0xA5, 0x39, 0x75, 0x8A, 0xB0, 0x00, 0x7F, /* AID defined on Android App */
                               0x00  /* Le  */ };

          uint8_t endMsg[MAX_TOKEN_LENGTH];
          dataLen = MAX_TOKEN_LENGTH;
          success = nfc.inDataExchange(finalMsg, sizeof(finalMsg), endMsg, &dataLen);

          if (success) {
            Serial.print("Final data length (should be nil): "); Serial.println(dataLen);
            nfc.PrintHexChar(endMsg, dataLen);
          }
        }
        /*
        uint8_t tries = 0;

        while(!dataCompletelyReceived && tries < 5) {
          // Build response
          uint8_t verifiedDataLen = dataLen + 1;
          uint8_t* verifiedData = new uint8_t[verifiedDataLen];
          verifiedData[0] = SENTINEL_BYTE;
          for (uint16_t i = 0; i < dataLen; i++) {
            verifiedData[i+1] = data[i];
          }
          dataLen = MAX_TOKEN_LENGTH;

          Serial.print("MY RESPONSE: "); Serial.println(verifiedDataLen);
          nfc.PrintHexChar(verifiedData, verifiedDataLen);

          success = nfc.inDataExchange(verifiedData, verifiedDataLen, data, &dataLen);

          if (success) {
            Serial.print("Response length: "); Serial.println(dataLen);
            nfc.PrintHexChar(data, dataLen);

            if (dataLen == 1 && data[0] == SENTINEL_BYTE) {
              dataCompletelyReceived = true;
            }
            delay(300);
          } else {
            Serial.println("Broken connection?"); 
            tries++;
          }

          delete(verifiedData);
        }*/
        
      } else {
        Serial.println("Failed sending SELECT AID"); 
      } 
  }
      
  else {
    Serial.println("Didn't find anything!");
  }

  delay(1000);
}

void printResponse(uint8_t *response, uint8_t responseLength) {
  
   String respBuffer;

    for (int i = 0; i < responseLength; i++) {
      
      if (response[i] < 0x10) 
        respBuffer = respBuffer + "0"; //Adds leading zeros if hex value is smaller than 0x10
      
      respBuffer = respBuffer + String(response[i], HEX) + " ";                        
    }

    Serial.print("response: "); Serial.println(respBuffer);
}

void setupNFC() {
 
  nfc.begin();
    
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig(); 
}
