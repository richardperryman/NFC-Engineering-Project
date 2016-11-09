/**************************************************************************/
/*! 
    @file     slave_nfc.ino
    @author   Adafruit Industries, with modification by Jessica Morris
    @license  BSD (see license.txt)
    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.
   
    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:
   
    Reads the 4 byte (32 bit) ID of a MiFare Classic card.
    Since the classic cards have only 32 bit identifiers you can stick
  them in a single variable and use that to compare card ID's as a
  number. This doesn't work for ultralight cards that have longer 7
  byte IDs!
   
    Note that you need the baud rate to be 115200 because we need to
  print out the data and read from the card at the same time!
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <DataPacket.h>

// SPI communication pins
#define PN532_SCK  (15)
#define PN532_MOSI (16)
#define PN532_SS   (5)
#define PN532_MISO (14)

// Define breakout with a SPI connection:
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Other stuff
static uint8_t MODULE_ID[] = {'N', 'F', 'C', '\0'};
static bool errorDuringSetup = false;
void flushRemaining();
static uint8_t sendData(uint8_t* data, uint16_t dataLen);

uint8_t* readCard(int uidLength) {
  uint8_t result[] = { 0, 0, 0, 0, 0, 0, 0 };  
  if (uidLength == 4)
  {
    // We probably have a Mifare Classic card ... 
    uint32_t cardid = result[0];
    cardid <<= 8;
    cardid |= result[1];
    cardid <<= 8;
    cardid |= result[2];  
    cardid <<= 8;
    cardid |= result[3]; 
  }
  return result;
}

void setup(void) {
  Serial.begin(115200);
  
  // NFC setup
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    errorDuringSetup = true;
    while (1); // halt
  }
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  // Wait for SETUP packet on serial
  while (!Serial.available())
  {
    delay(100);
  }

  // Read in four bytes - these should be the SETUP packet
  uint8_t setup[4];
  for (int i = 0; i < 4; i++)
  {
    setup[i] = Serial.read();
  }
  flushRemaining();

  DataPacket* received = new DataPacket(setup, 4);
  if (received->getOpcode() != OPCODE_SETUP)
  {
    //panic
    errorDuringSetup = true;
    
  } else {  
    DataPacket *data = new DataPacket(OPCODE_DATA, 1, MODULE_ID, sizeof(MODULE_ID)/sizeof(MODULE_ID[0]));
  }
}


void loop(void) {
  
  
  uint8_t success;
  uint8_t *uid;  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    uid = readCard(uidLength);
  }
}

static void flushRemaining()
{
  while (Serial.available()) Serial.read();
}

static uint8_t sendData(uint8_t* data, uint16_t dataLen)
{
  uint16_t numPackets = 1 + ((dataLen - 1) / MAX_DATA_BYTES);
  bool error = false;

  for (uint16_t i = 0; i < numPackets && !error; i++)
  {
    // Send DATA
    uint8_t* ptr = data + i*508;
    DataPacket* data = new DataPacket(OPCODE_DATA, i+1, ptr, dataLen - 508*i);

    uint8_t byteBuffer[data->getPacketSize()];
    uint16_t numBytes = data->toByteArray(byteBuffer);

    uint16_t j;
    for (j = 0; j < numBytes; j++)
    {
      Serial.write(byteBuffer[i]);
    }

    // Receive ACK
    j = 0;
    while(j++ < 30 && !Serial.available())
    {
      delay(100);
    }

    if (j == 30)
    {
      error = true;
    }
    else {
      uint8_t ack[6];
      
      // Read in three bytes - check for OPCODE_ERROR first
      for (j = 0; j < 4; j++)
      {
        ack[j] = Serial.read();
      }

      if ((packet_opcode_t)((ack[1] << 8) + (ack[2] & 0xFF)) == OPCODE_ACK)
      {
        // Read in the rest of the ACK packet
        for (; j < 6; j++)
        {
          ack[j] = Serial.read();
        }

        uint16_t ackNum = (ack[3] << 8) + (ack[4] & 0xFF);
        
        if (ackNum != i+1)
        {
          error = true;
        }
      }
      else
      {
        // Read in the rest of the bad packet
        error = true;
      }
    }
  }
  
  return error;
}

