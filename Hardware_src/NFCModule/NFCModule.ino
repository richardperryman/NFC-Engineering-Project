#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532Interface.h>
#include <PN532.h>
#include <AuthenticationModule.h>
#include <Types.h>

// SPI communication pins
#define PN532_SCK  (15)
#define PN532_MOSI (16)
#define PN532_SS   (5)
#define PN532_MISO (14)

#define MAX_TOKEN_LENGTH 255

class NFCModule : public AuthenticationModule {
private:
  PN532_SPI* pn532spi;
  PN532* nfc;
public:
  NFCModule()
  : AuthenticationModule("nfc", 4)
  {
    pn532spi = new PN532_SPI(SPI, PN532_SS);
    nfc = new PN532(*pn532spi);
  }

  bool startup() {
    bool error = false;

    Serial.begin(115200);
    nfc->begin();

    if (!nfc->getFirmwareVersion()) {
      error = true;
    } else {
      nfc->SAMConfig();
      error = AuthenticationModule::startup();
    }

    if (error) {
      EncodedPacket* errorPacket = new EncodedPacket(OPCODE_ERROR, ERROR_SETUP);
      AuthenticationModule::sendPacket(errorPacket);
      delete(errorPacket);
    }

    return error;
  }

  uint16_t getData(uint8_t* buff, uint16_t bufferLen)
  {
    uint16_t bytesWritten = 0;
    bool success = nfc->inListPassiveTarget();
    
    if (success) {
      uint8_t initMsg[] = { 0x00, /* CLA */
                            0xA4, /* INS */
                            0x04, /* P1  */
                            0x00, /* P2  */
                            0x08, /* Length of AID  */
                            0xF7, 0xA5, 0x39, 0x75, 0x8A, 0xB0, 0x00, 0x7F, /* AID defined on Android App */
                            0xF4  /* Le  */ };

      for (; bytesWritten < bufferLen && success;) {
        uint8_t data[MAX_TOKEN_LENGTH];
        uint8_t dataLen = MAX_TOKEN_LENGTH;

        if (success = nfc->inDataExchange(initMsg, sizeof(initMsg), data, &dataLen)) {
          uint16_t j = 0;
          for (; j < dataLen && (bytesWritten + j) < bufferLen; j++) {
            buff[bytesWritten + j] = data[j];
          }

          bytesWritten += j;
        }
      }
    }

    return bytesWritten;
  }  
};

NFCModule nfcModule;
bool error = false;

void setup() {
  error = nfcModule.startup();
}

void loop() {
  if (error) {
    delay(10000);
  } else {
    // 512 byte buffer for data (a little overkill for NFC, but it's fine)
    uint16_t dataLen = 512;
    uint8_t data[dataLen];

    if ((dataLen = nfcModule.getData(data, dataLen)) > 0) {
      nfcModule.sendData(data, dataLen);
    }

    delay(300);
  }
}

