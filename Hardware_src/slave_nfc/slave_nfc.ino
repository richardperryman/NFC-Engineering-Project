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

#include <DecodedPacket.h>
#include <EncodedPacket.h>
#include <Types.h>

// n.b. On the microcontroller's end, it will only ever receive SETUPs, ACKS, and ERRORs with 2-byte block numbers or codes
// All of these packets are 6 bytes long
#define MAX_PACKET_BYTES 6

// SPI communication pins
#define PN532_SCK  (15)
#define PN532_MOSI (16)
#define PN532_SS   (5)
#define PN532_MISO (14)
#define LED (18)

// Define breakout with a SPI connection:
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Other stuff
static uint8_t MODULE_ID[] = {'N', 'F', 'C', 0x00};
static bool errorDuringSetup = false;
void flushRemaining();
static uint8_t sendData(uint8_t* data, uint16_t dataLen);
static DecodedPacket* receivePacket();
static void sendPacket(EncodedPacket packet);
static void flashLED();

uint16_t readCard(uint8_t* destination)
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
  uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint16_t writeIndex = 0; // The index writing to the destination

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (!success) {
    return 0;
  } else {
    if (uidLength == 4)
    {
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    // Start with block 4 (the first block of sector 1) since sector 0
    // contains the manufacturer data and it's probably better just
    // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    
      if (success)
      {
        uint8_t data[16];

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
    
        if (success)
        {
          // Record the data
          for (uint16_t i = 0; i < 16; i++) {
            destination[writeIndex++] = data[i];
          }
      
          // Wait a bit before reading the card again
          delay(500);
          return writeIndex;
        }
        else
        {
          // Problem reading
          return 0;
        }
      }
      else
      {
        // Problem reading
        return 0;
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
    
      // Try to read the first general-purpose user page (#4)
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        Serial.println("");
    
        // Wait a bit before reading the card again
        delay(500);
      }
      else
      {
        // Problem reading
        return 0;
      }
    }
  }
}

/*void loop(void) {
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                      // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
   
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    // Start with block 4 (the first block of sector 1) since sector 0
    // contains the manufacturer data and it's probably better just
    // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
    
      if (success)
      {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
 
        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
    
        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");
      
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
    
      // Try to read the first general-purpose user page (#4)
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        Serial.println("");
    
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        Serial.println("Ooops ... unable to read the requested page!?");
      }
    }
  }
}*/

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  // NFC setup
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    errorDuringSetup = true;
  }
  
  // configure board to read RFID tags
  nfc.SAMConfig();

  // Read in SETUP packet
  DecodedPacket* received = receivePacket();
  
  if (received->getOpcode() != OPCODE_SETUP)
  {
    // panic
    errorDuringSetup = true;
  } else {
    EncodedPacket* response = new EncodedPacket(OPCODE_DATA, 1, MODULE_ID, sizeof(MODULE_ID)/sizeof(MODULE_ID[0]));
    sendPacket(*response);

    // Receive ACK
    DecodedPacket* ackPacket = receivePacket();
    if (ackPacket->getOpcode() != OPCODE_ACK)
    {
      //errorDuringSetup = true;
    }

    delete(response);
  }

  delete(received);
}

void loop() {
  if (errorDuringSetup)
  {
    delay(10000); // delay ad infinitum
  }
  else {
    // Wait for something to read
    uint8_t buffer[512];
    uint16_t numBytes;
    if ((numBytes = readCard(buffer)) > 0) {
      flashLED();
      sendData(buffer, numBytes); 
    }
  }
}

static void flushRemaining()
{
  while (Serial.available()) Serial.read();
}

static uint8_t sendData(uint8_t* data, uint16_t dataLen)
{
  uint16_t numPackets = 1 + ((dataLen - 1) / MAX_DECODED_BYTES);
  bool error = false;

  Serial.print("Received data: ");
  for (int i = 0; i < dataLen; i++) {
    Serial.print(data[i], HEX); Serial.print(" ");
  }
  Serial.println();

  // On i = 0, send the REQUEST packet, after that send DATA packets
  for (uint16_t i = 0; i < numPackets + 1 && !error; i++)
  {
    if (i > 0x0000)
    {
      // Send DATA
      uint8_t* ptr = data + ((i-1)*508);
      EncodedPacket* data = new EncodedPacket(OPCODE_DATA, i, ptr, dataLen - 508*(i-1));
      sendPacket(*data);
      delete(data);
    } else {
      // Send REQ
      EncodedPacket* req = new EncodedPacket(OPCODE_REQUEST, numPackets);
      sendPacket(*req);
      delete(req);
    }

    // Receive ACK
    DecodedPacket* ack = receivePacket();
    if (ack->getOpcode() != OPCODE_ACK)
    {
      // panic
      error = true;
    } else {
      // check block number here I guess
    }
    delete(ack);
  }
  
  return error;
}

static DecodedPacket* receivePacket()
{
  // TODO: Add a timeout to this
  uint8_t packetBytes[MAX_PACKET_BYTES];
  uint8_t readChar;
  
  while (!Serial.available()) {
    delay(100); // Block until available
  }

  // Read in PACKET_FLAG
  do {
    readChar = Serial.read();
  } while (readChar != PACKET_FLAG);

  // Store 8 bytes in buffer
  uint16_t i = 0;
  do {
    packetBytes[i++] = readChar;
    readChar = Serial.read();
  } while(i < MAX_PACKET_BYTES-1 && readChar != PACKET_FLAG);
  packetBytes[i++] = readChar;
  flushRemaining();

  // Return the packet
  return new DecodedPacket(packetBytes, i);
}

static void sendPacket(EncodedPacket packet)
{
  uint8_t byteBuffer[packet.getPacketSize()];
  uint16_t numBytes = 0;

  numBytes = packet.getPacketBytes(byteBuffer);

  Serial.write(byteBuffer, numBytes);
  Serial.flush();
}

static void flashLED() {
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

