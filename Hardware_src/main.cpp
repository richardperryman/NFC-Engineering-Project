
#include <iostream>
#include <unistd.h>
#include <Types.h>
#include <Serial.h>
#include <GPIOPin.h>

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

// For now this will just blink the LEDs
// n.b. Because the Pi is slow, it needs small sleeps during the pin setup

static uint32_t LOCK_ID;
static GPIOPin* GREEN;
static GPIOPin* BLUE;
static GPIOPin* RED;
static GPIOPin* RELAY_SIGNAL;

int main()
{

	return 0;
}

static uint8_t readConfiguration(string filepath)
{
	return 0;
}

static void setUpLEDs()
{
	GREEN = new GPIOPin("2");
	BLUE = new GPIOPin("3");
	RED = new GPIOPin("4");
	
	GREEN->exportPin();
	BLUE->exportPin();
	RED->exportPin();
	
	usleep(100);
	
	GREEN->setOutput();
	BLUE->setOutput();
	RED->setOutput();
	
	usleep(100);
}

static void teardown()
{
	GREEN->unexportPin();
	BLUE->unexportPin();
	RED->unexportPin();	

	usleep(100);
}

static uint8_t verifyModules()
{
	// Poll "/dev/ttyACM";
	
	// Get String names from modules
	
	// Package up lock ID, modules, fire this off to the server to verify
	
	
	// Return happy or sad
	return 0;
}

static uint8_t* getToken()
{
	// Open up Serial
	
	// Read token from Serial
	
	return 0;
}
