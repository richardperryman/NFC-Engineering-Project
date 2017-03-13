#include <Keypad.h>

#include <Types.h>
#include <DecodedPacket.h>
#include <Packet.h>
#include <AuthenticationModule.h>
#include <EncodedPacket.h>

static const byte ROWS = 4;
static const byte COLS = 3;
static char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
static byte rowPins[ROWS] = {8, 7, 6, 5};
static byte colPins[COLS] = {4, 3, 2};

static const int LEDPIN = 13;

class PINModule : public AuthenticationModule {
  private:
    Keypad* keypad;
    
  public:
    PINModule()
    : AuthenticationModule("PIN", 4)
    {
      keypad = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
    }

    bool startup() {
      pinMode(LEDPIN, OUTPUT);
      Serial.begin(115200);
      return AuthenticationModule::startup();
    }

    uint16_t getData(uint8_t* buff, uint16_t bufferLen)
    {
      uint16_t bytesWritten = 0;
      char key;
      if ((key = keypad->getKey()) != NO_KEY)
      {
        int timeout = 500; // 5-second timeout after key press
        buff[bytesWritten++] = key;
        digitalWrite(LEDPIN, HIGH);

        while (timeout > 0 && (key = keypad->getKey()) != '#')
        {
          if ((key != NO_KEY) && (key != '*')) {
            if (bytesWritten < bufferLen)
            {
              buff[bytesWritten++] = key;
            }
            timeout = 500;
          } else {
            delay(10);
            timeout--;
          }
        }
      }

      digitalWrite(LEDPIN, LOW);
      return bytesWritten;
    }
};

PINModule pinModule;
bool error = false;

void setup() {
  error = pinModule.startup();
}

void loop() {
  if (error) {
    delay(10000);
  } else {
    uint16_t dataLen = 8; // Arbitrarily designated 8 numbers as maximum
    uint8_t data[dataLen];

    if ((dataLen = pinModule.getData(data, dataLen)) > 0) {
      // Blink LED to indicate data being sent
      digitalWrite(LEDPIN, HIGH);
      delay(200);
      digitalWrite(LEDPIN, LOW);
      delay(200);
      digitalWrite(LEDPIN, HIGH);
      delay(200);
      digitalWrite(LEDPIN, LOW);
      pinModule.sendData(data, dataLen);
    }

    delay(300);
  }
}
