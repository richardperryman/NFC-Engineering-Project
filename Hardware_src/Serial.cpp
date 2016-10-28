
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <Debug.h>
#include <Serial.h>

using namespace std;

Serial::Serial()
{
	this->portName = "ACM0";
	this->serialPort = -1;
}

Serial::Serial(string portName)
{
	this->portName = portName;
	this->serialPort = -1;
}

int Serial::openPort()
{
	serialPort = open(this->portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

	if (serialPort == -1)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Failed to open port " + this->portName + ".");
		return -1;
	}
    
	struct termios portOptions;

	fcntl(serialPort, F_SETFL, FNDELAY);

	// Fetch the current port settings
	tcgetattr(serialPort, &portOptions);

	// Flush the port's buffers
	tcflush(serialPort, TCIOFLUSH);

	// Set the input and output baud rates
	// This will be 115,200 to match the NFC module's.
	cfsetispeed(&portOptions, B115200);
	cfsetospeed(&portOptions, B115200);

	// c_cflag contains a few important things- CLOCAL and CREAD, to prevent
	// this program from "owning" the port and to enable receipt of data.
	// Also, it holds the settings for number of data bits, parity, stop bits,
	// and hardware flow control. 
	portOptions.c_cflag |= CLOCAL;
	portOptions.c_cflag |= CREAD;
	// Set up the frame information.
	portOptions.c_cflag &= ~PARENB;
	portOptions.c_cflag &= ~CSTOPB;
	portOptions.c_cflag &= ~CSIZE;
	portOptions.c_cflag |= CS8;

	// Push options to port
	tcsetattr(serialPort, TCSANOW, &portOptions);

	// Flush the buffer one more time.
	tcflush(serialPort, TCIOFLUSH);
	
	return 0;
}

string Serial::readData()
{
	if (serialPort == -1)
	{
		DEBUG_LOG(WARNING, __FUNCTION__, "Port " + this->portName + " has not been opened.");
		return "";
	}
	
    char buffer[1];
    int bytesRead = 0;
    string returnString;

    do
    {
        bytesRead = read(this->serialPort, buffer, sizeof(buffer));

        if(bytesRead > 0)
            returnString.append(buffer, bytesRead);

    } while (bytesRead == sizeof(buffer));

    return returnString;
}
