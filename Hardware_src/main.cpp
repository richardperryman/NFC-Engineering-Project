
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <Types.h>
#include <Serial.h>
#include <GPIOPin.h>
#include <DataPacket.h>

// To do:
// Read configuration file at start up (server URL, lock ID)


// For unlocking:
// Pi sends tokens to server
// Server responds w/ "hey I'm ready to send yes/no"
// Pi generates one-time-use public key (/dev/urandom or something, 256-bits) and sends to server
// Server encrypts yes/no with one-time-use key and sends it back to the Pi

// For QR:
// User takes photo of code (the code is the lock ID), sends to server
// Server forwards user info to Pi (server is a authentication module)

static uint32_t LOCK_ID;
static GPIOPin* GREEN;
static GPIOPin* BLUE;
static GPIOPin* RED;
// static GPIOPin* RELAY_SIGNAL;

static void maintenanceLoop();
static uint8_t readConfiguration(std::string filepath);
static void setUpLEDs();
static void teardown();
static std::vector<Serial> verifyModules();
static uint8_t getToken(Serial port, uint8_t* destination);

int main()
{
    uint8_t status = 0;
    std::string filepath = "/not/real/rightnow.cfg";
    
    std::vector<Serial> modules;
    
    setUpLEDs();
    status = readConfiguration(filepath);
    
    if (status != 0)
    {
        DEBUG_LOG(CRITICAL, __FUNCTION__, "Failure to read configuration from " + filepath);
        maintenanceLoop();
    }
    
    modules = verifyModules();
    
    if (modules.size() == 0)
    {
        DEBUG_LOG(CRITICAL, __FUNCTION__, "No authentication modules found!");
        maintenanceLoop();
    }
    
    BLUE->setValue(GPIO_HIGH);   
    while(true)
    {
        // Poll each device
        for (uint8_t i = 0; i < modules.size(); i++)
        {
            Serial module = modules.at(i);
            
            //
        }
        
        usleep(300);
    }

    teardown();
    
	return 0;
}

static uint8_t readConfiguration(std::string filepath)
{
    LOCK_ID = 0xDEADBEEF;
    
	return 0;
}

static void maintenanceLoop()
{
    while(true)
    {
        RED->setValue(GPIO_HIGH);
        sleep(1);
        RED->setValue(GPIO_LOW);
        sleep(1);
    }
}

static void setUpLEDs()
{
	GREEN = new GPIOPin("2");
	BLUE = new GPIOPin("3");
	RED = new GPIOPin("4");
	
	GREEN->exportPin();
	BLUE->exportPin();
	RED->exportPin();
	
	sleep(1);
	
	GREEN->setOutput();
	BLUE->setOutput();
	RED->setOutput();
	
	sleep(1);
}

static void teardown()
{
	GREEN->unexportPin();
	BLUE->unexportPin();
	RED->unexportPin();	

	sleep(1);
}

static std::vector<Serial> verifyModules()
{
    uint8_t i = 0;
    uint8_t result;
    const std::string path = "/dev/ttyACM";
    std::string fullpath;
    struct stat buffer;
        
    std::vector<std::string> devices; // Not every ACMx device is a module
    std::vector<Serial> modules;
        
    // Get all device paths on /dev/ttyACM*
    do
    {
        fullpath = path + std::to_string(i);
        result = stat(fullpath.c_str(), &buffer);
        
        if (result == 0)
        {
            printf("Found device on %s\n", fullpath.c_str());
            devices.push_back(fullpath);
        }
        i++;
    } while (result == 0);
    
	// Get String names from modules
	DataPacket* setup = new DataPacket(OPCODE_SETUP);
    
    for (i = 0; i < devices.size(); i++)
    {
        Serial *usb = new Serial(devices.at(i));
        usb->openPort(SERIAL_BAUD_NFC);

        usb->sendPacket(*setup);
        usb->flush();

        while (usb->dataAvailable() < 0)
        {
            // Block for response
        }
        
        DataPacket* response = usb->receivePacket();
        if (response == nullptr)
        {
            DEBUG_LOG(WARNING, __FUNCTION__, "Device on " + devices.at(i) + " is not a valid authentication module.");
        }
        else
        {
            uint8_t bytes[512];
            response->getData(bytes);
            printf("Found module with ID: %s\n", bytes);
            
            modules.push_back(*usb);
        }
    }
	// Package up lock ID, modules, fire this off to the server to verify
	
	
	// Return happy or sad
	return modules;
}

// Currently will just print stuff
// todo: more robust error handling, result should be stored to a temporary buffer first, then copied
static uint8_t getToken(Serial port, uint8_t* destination)
{
	DataPacket* receivedPacket = port.receivePacket();
    
    if (receivedPacket == nullptr)
    {
        DEBUG_LOG(ERROR, __FUNCTION__, "Received an invalid packet.");
        return -1;
    }
    else
    {
        if (receivedPacket->getOpcode() != OPCODE_REQ)
        {
            DEBUG_LOG(ERROR, __FUNCTION__, "Received request packet with bad opcode.");
            return -1;
        }
        else
        {
            uint8_t packetData[MAX_DATA_BYTES];
            uint16_t numBytes = receivedPacket->getData(packetData);
            
            if (numBytes != 2)
            {
                DEBUG_LOG(ERROR, __FUNCTION__, "Received request packet with bad data.");
                return -1;
            }
            else
            {
                uint16_t numDataPackets = ((packetData[1] << 8) + (packetData[2] & 0xFF));
                uint16_t currentDataPacket = 0x0000;
                
                bool error = false;
                for (; currentDataPacket < numDataPackets + 1 && !error; currentDataPacket++)
                {
                    // Send ACK
                    DataPacket *ack = new DataPacket(OPCODE_ACK, currentDataPacket);
                    port.sendPacket(*ack);
                    
                    // Receive DATA
                    if (currentDataPacket < numDataPackets)
                    {
                        if (!port.dataAvailable())
                        {
                            DEBUG_LOG(ERROR, __FUNCTION__, "Data packet timed out.");
                            return -1;
                        }
                        else
                        {
                            DataPacket *data = port.receivePacket();
                            
                            if (data->getOpcode() != OPCODE_DATA)
                            {
                                DEBUG_LOG(ERROR, __FUNCTION__, "Received data packet with bad opcode.");
                                return -1;
                            }
                            else
                            {
                                uint8_t dataBuffer[MAX_DATA_BYTES];
                                uint16_t bytesReceived = data->getData(dataBuffer);
                                
                                DEBUG_LOG(INFO, __FUNCTION__, "Received data packet of length " + std::to_string(bytesReceived));
                                
                                for (int i = 0; i < bytesReceived; i++) {
                                    printf("0x%02X ", dataBuffer[i]);
                                    if (i%127 == 0) printf("\n");
                                }
                                printf("\n");
                            }
                        }
                    }                    
                }
            }
        }
    }
    
	return 0;
}
