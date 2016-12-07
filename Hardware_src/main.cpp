
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <chrono>
#include <thread>
#include <sys/stat.h>
#include <Types.h>
#include <Serial.h>
#include <GPIOPin.h>
#include <DecodedPacket.h>
#include <EncodedPacket.h>
#include <ServerConnection.h>
#include <Debug.h>

// To do:
// Read configuration file at start up (server URL, lock ID)


// For unlocking:
// Pi sends tokens to server
// Server responds w/ "hey I'm ready to send yes/no"
// Pi generates one-time-use public key (/dev/urandom or something, 256-bits) and sends to server
// Server encrypts yes/no with one-time-use key and sends it back to the Pi

// For QR:
// User takes photo of code (the code is the lock ID), sends code to server
// Server forwards user info to Pi (server is a authentication module)

static uint32_t LOCK_ID;
static std::string SERVER_URL;

static GPIOPin* GREEN;
static GPIOPin* BLUE;
static GPIOPin* RED;
// static GPIOPin* RELAY_SIGNAL;

static void maintenanceLoop();
static void pollingLoop(std::vector<Serial> modules);
static int8_t readConfiguration(std::string filepath);
static int8_t verifyModules(std::vector<Serial>* modules);
static uint16_t getToken(Serial port, uint8_t* destination);

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
    
    delete(GREEN);
    delete(BLUE);
    delete(RED);
}

void accessGranted()
{
    BLUE->setValue(GPIO_LOW);
    GREEN->setValue(GPIO_HIGH);
    // RELAY_SIGNAL->setValue(GPIO_HIGH);
    
    sleep(3); // Disengage the lock for three seconds
    
    BLUE->setValue(GPIO_HIGH);
    GREEN->setValue(GPIO_LOW);
    // RELAY_SIGNAL->setValue(GPIO_LOW);
}

void accessDenied()
{
    BLUE->setValue(GPIO_LOW);
    RED->setValue(GPIO_HIGH);
    
    sleep(1); // Show red for one second
    
    BLUE->setValue(GPIO_HIGH);
    RED->setValue(GPIO_LOW);
}

int main()
{
    uint8_t status = 0;
    std::string filepath = "/not/real/rightnow.cfg";
    
    std::vector<std::string> moduleIDs;
    std::vector<uint8_t*> tokens;
    uint8_t testToken[255];
    testToken[0] = 0x11;
    testToken[1] = 0x22;
    uint8_t testToken2[255];
    //moduleIDs.push_back("NFC");
    //tokens.push_back(testToken);
    moduleIDs.push_back("Password");
    testToken2[0] = 'p';
    testToken2[1] = 'a';
    testToken2[2] = 's';
    testToken2[3] = 's';
    testToken2[4] = 'w';
    testToken2[5] = 'o';
    testToken2[6] = 'r';
    testToken2[7] = 'd';
    tokens.push_back(testToken2);
    uint32_t lockID = 0x1;
    
    printf("Generating server connection\n");
    ServerConnection* conn = new ServerConnection("http://sbacs.48tdba4fch.us-west-2.elasticbeanstalk.com");
    printf("Checking for signs of life\n");
    conn->openConnection();
    if (0 == conn->verifyConnection())
    {
        printf("Server online, performing query\n");
        conn->requestAccess(lockID, moduleIDs, tokens);
    } else {
        printf("Server not online\n");
    }
    
    conn->closeConnection();
    delete(conn);
    
    /*setUpLEDs();
    status = readConfiguration(filepath);
    
    if (status != 0) {
        DEBUG_LOG(CRITICAL, __FUNCTION__, "Failure to read configuration from %s", filepath);
        maintenanceLoop();
    } else {
        ServerConnection* conn = new ServerConnection(SERVER_URL);
        //ServerConnection* conn = new ServerConnection("google.com");
        conn->openConnection();
        status = conn->verifyConnection();
        
        if (status != 0) {
            DEBUG_LOG(CRITICAL, __FUNCTION__, "Cannot reach server.");
            conn->closeConnection();
            maintenanceLoop();
        } else {
            status = verifyModules(&modules);
            
            if (status != 0) {
                DEBUG_LOG(CRITICAL, __FUNCTION__, "Error retrieving authentication modules.");
            } else if (modules.size() == 0) {
                DEBUG_LOG(CRITICAL, __FUNCTION__, "No authentication modules found!");
                conn->closeConnection();
                maintenanceLoop();
            } else {
                pollingLoop(modules);
            }
        }
    }
    
    teardown();*/
    return 0;
}

static int8_t readConfiguration(std::string filepath)
{
    LOCK_ID = 0xDEADBEEF;
    SERVER_URL = "http://sbacs.48tdba4fch.us-west-2.elasticbeanstalk.com";
    
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

static void pollingLoop(std::vector<Serial> modules)
{
    BLUE->setValue(GPIO_HIGH);   
    while(true) // TODO: Switch this loop to check for a kill file so I can quit gracefully
    {
        std::vector<uint8_t[255]> tokens;
        
        // Poll each device
        for (uint8_t i = 0; i < modules.size(); i++)
        {
            Serial module = modules.at(i);
            
            if (module.dataAvailable())
            {
                DEBUG_LOG(INFO, __FUNCTION__, "Receiving auth token from module...");
                uint8_t test[255];
                getToken(module, test);
                
                //DEBUG_LOG(INFO, __FUNCTION__, "Token as string: %s\n", test);
                std::cout << "Token as string: " << test << "\n";
                
                
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }   
}

static int8_t verifyModules(std::vector<Serial>* modules)
{
    uint8_t i = 0;
    
    std::vector<std::string> device_paths; // Not every ACMx device is a module

    // Get all device paths on /dev/ttyACM*
    FILE * f = popen( "find /dev -name ttyACM*", "r" );
    if ( f == 0 ) {
        DEBUG_LOG(CRITICAL, __FUNCTION__, "Could not get device listing from /dev.");
        return -1;
    }
    
    const int BUFSIZE = 260; // 255 + 5 chars for "/dev/"
    char buf[ BUFSIZE ];
    
    while( fgets( buf, BUFSIZE,  f ) ) {
        std::string* path= new std::string(buf);
        device_paths.push_back(path->substr(0, path->length()-1));
        DEBUG_LOG(INFO, __FUNCTION__, "Found device on %s", (path->substr(0, path->length()-1)).c_str());
    }
    pclose( f );
    
    if (device_paths.size() == 0)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "No device paths found.");
        return -1;
    }
    
	// Get String names from modules
	EncodedPacket* setup = new EncodedPacket(OPCODE_SETUP, 0x0000);
    
    for (i = 0; i < device_paths.size(); i++)
    {
        Serial *usb = new Serial(device_paths.at(i));
        usb->openPort(SERIAL_BAUD_NFC);

        usb->sendPacket(*setup);
        usb->flush();

        DecodedPacket* response = usb->receivePacket();
        if (response == nullptr)
        {
            DEBUG_LOG(WARNING, __FUNCTION__, "Device on %s is not a valid authentication module.", device_paths.at(i).c_str());
        }
        else
        {
            uint8_t responseData[response->getDataSize()];
            response->getData(responseData);
            
            DEBUG_LOG(INFO, __FUNCTION__, "Found module with ID: %s", responseData);
            modules->push_back(*usb);
            
            // Send ACK back
            EncodedPacket* ack = new EncodedPacket(OPCODE_ACK, response->getBlockNumber());
            usb->sendPacket(*ack);
            delete(ack);
        }
    }
    
    delete(setup);
	
	return 0;
}

// todo: more robust error handling, result should be stored to a temporary buffer first, then copied
static uint16_t getToken(Serial port, uint8_t* destination)
{
    // Receive REQUEST
	DecodedPacket* receivedPacket = port.receivePacket();
    
    if (receivedPacket == nullptr)
    {
        DEBUG_LOG(ERROR, __FUNCTION__, "Received an invalid packet.");
        return -1;
    }
    else
    {
        if (receivedPacket->getOpcode() != OPCODE_REQUEST)
        {
            DEBUG_LOG(ERROR, __FUNCTION__, "Received request packet with bad opcode.");
            return -1;
        }
        else
        {
            uint16_t numDataPackets = receivedPacket->getBlockNumber();
            uint16_t currentDataPacket = 0x0000;
            
            DEBUG_LOG(INFO, __FUNCTION__, "Expecting %d data packets.", numDataPackets);
            
            bool error = false;
            for (; currentDataPacket < numDataPackets + 1 && !error; currentDataPacket++)
            {
                // Send ACK
                EncodedPacket *ack = new EncodedPacket(OPCODE_ACK, currentDataPacket);
                port.sendPacket(*ack);
                
                // Receive DATA
                if (currentDataPacket < numDataPackets)
                {
                    DecodedPacket *data = port.receivePacket();
                        
                    if (data == nullptr)
                    {
                        DEBUG_LOG(ERROR, __FUNCTION__, "Data packet timed out.");
                        return -1;
                    }
                        
                    if (data->getOpcode() != OPCODE_DATA)
                    {
                        DEBUG_LOG(ERROR, __FUNCTION__, "Received data packet with bad opcode.");
                        return -1;
                    }
                    else
                    {
                        uint8_t dataBuffer[data->getDataSize()];
                        uint16_t bytesReceived = data->getData(dataBuffer);
                        
                        DEBUG_LOG(INFO, __FUNCTION__, "Received data packet of length %d", bytesReceived);
                        
                        printf("Received packet bytes: [ ");
                        for (int i = 0; i < bytesReceived; i++) {
                            destination[i] = dataBuffer[i];
                            printf("0x%02X ", dataBuffer[i]);
                            if (i > 0 && i%127 == 0) printf("\n");
                        }
                        printf("]\n");
                    }
                }
            }
        }
    }
    
	return 0;
}
