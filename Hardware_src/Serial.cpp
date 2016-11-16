
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <chrono>
#include <thread>
#include <Debug.h>
#include <Serial.h>

#define DELAY 100

Serial::Serial(std::string portName)
{
    this->portName = portName;
    this->serialPort = -1;
}

int Serial::openPort(int baudRate)
{
    struct termios portOptions;
    speed_t baud;
    int status;
    
    switch(baudRate)
    {
        case 50: baud = B50;     break;
        case 75: baud = B75 ;    break;
        case 110: baud = B110;    break;
        case 134: baud = B134;    break;
        case 150: baud = B150;    break;
        case 200: baud = B200;    break;
        case 300: baud = B300;    break;
        case 600: baud = B600;    break;
        case 1200: baud = B1200;   break;
        case 1800: baud = B1800;   break;
        case 2400: baud = B2400;   break;
        case 4800: baud = B4800;   break;
        case 9600: baud = B9600;   break;
        case 19200: baud = B19200;  break;
        case 38400: baud = B38400;  break;
        case 57600: baud = B57600;  break;
        case SERIAL_BAUD_NFC: baud = B115200; break;
        case 230400: baud = B230400; break;
   
        default:
            DEBUG_LOG(ERROR, __FUNCTION__, "Invalid baud rate passed");
            return -1;
    }
    
    // Flags:
    // O_RDWR = open for reading and writing
    // O_NOCTTY = this process "own" the port
    // O_NDELAY = use non-blocking I/O
    // open returns a file descriptor which will be stored as an int
    serialPort = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);

    if (serialPort == -1)
    {
        DEBUG_LOG(CRITICAL, __FUNCTION__, "Failed to open port %s.", portName.c_str());
        return -1;
    }
    
    // Set file status (read/write) of the port
    fcntl(serialPort, F_SETFL, O_RDWR);

    // Fetch the current port settings
    tcgetattr(serialPort, &portOptions);

    // Set the input and output baud rates
    cfsetispeed(&portOptions, baud);
    cfsetospeed(&portOptions, baud);

    // Ignore CD signal (modem control) and enable this port as receiver
    portOptions.c_cflag |= (CLOCAL | CREAD);
    
    // Set up the frame information.
    portOptions.c_cflag &= ~PARENB; // No parity checking
    portOptions.c_cflag &= ~CSTOPB; // ?
    portOptions.c_cflag &= ~CSIZE; // Clear current char size mask
    portOptions.c_cflag |= CS8; // Force 8-bit input
    portOptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non-canonical mode, echo off, ?, signal chars off
    portOptions.c_oflag &= ~OPOST; // No local output processing
    
    portOptions.c_cc[VMIN] = 0; // Minimum number of bytes to return from read() is 0
    portOptions.c_cc[VTIME] = 50; // Wait up to 5 seconds for a character

    // Flush and push options to port
    tcsetattr(serialPort, TCSANOW | TCSAFLUSH, &portOptions);
    
    // Get current port status
    ioctl(serialPort, TIOCMGET, &status);
    
    // Data Terminal Ready, Request To Send
    status |= (TIOCM_DTR | TIOCM_RTS);
    
    // Send the new port status
    ioctl(serialPort, TIOCMSET, &status);
    
    usleep(10000);
    
    // This next bit deals with race conditions
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(serialPort, &rfd);
    timeval tv = { 0 };
    select(serialPort+1, &rfd, 0, 0, &tv);
    //if (!FD_ISSET(serialPort, &rfd)) {
    
    return 0;
}

void Serial::closePort()
{
    close(serialPort);
    serialPort = -1;
}

void Serial::flush()
{
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return;
    }
    
    int result = tcdrain(serialPort);
    if (result != 0)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "tcdrain returned error code %d", result);
    }
}

void Serial::flushRemaining()
{
    while (this->dataAvailable())
    {
        readChar();
    }
}

int Serial::dataAvailable()
{
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return -1;
    }
    
    int result;

    if (ioctl(serialPort, FIONREAD, &result) < 0)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "ioctl to get number of bytes buffered in %s failed.", portName.c_str());
        result = -1;
    }
    
    return result;
}

uint8_t Serial::blockForData(uint8_t maxSeconds)
{
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return -1;
    }
    
    uint16_t numDelays = maxSeconds * 10;
    
    for (uint16_t i = 0; i < numDelays; i++)
    {
        if (dataAvailable())
        {
            return 0;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
        }
    }
    
    return -1;
}

uint8_t Serial::readChar()
{
    uint8_t result;
    
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return -1;
    }
    
    if (read(serialPort, &result, 1) != 1)
    {
        return -1;
    }
    
    return result;
}

DecodedPacket* Serial::receivePacket()
{
    uint8_t buffer;
    uint8_t temp[MAX_PACKET_SIZE];
    
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return nullptr;
    }
    
    // Block 3 seconds for data - return nullptr if no data
    if (blockForData(3) != 0)
    {
        return nullptr;
    }
    
    do {
        buffer = readChar();
    } while (buffer != PACKET_FLAG);
    
    uint16_t i = 0;
    do {
        temp[i++] = buffer;
        buffer = readChar();
    } while (buffer != PACKET_FLAG && buffer != -1);
    
    temp[i++] = buffer;
    flushRemaining();
    
    return new DecodedPacket(temp, i);
}

void Serial::writeChar(uint8_t character)
{
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return;
    }
    
    write(serialPort, &character, 1);   
}

void Serial::sendPacket(EncodedPacket packet)
{
    if (serialPort == -1)
    {
        DEBUG_LOG(WARNING, __FUNCTION__, "Port %s has not been opened.", portName.c_str());
        return;
    }
    
    uint16_t numBytes = packet.getPacketSize();
    uint8_t packetBytes[numBytes];
    packet.getPacketBytes(packetBytes);
    
    write(serialPort, packetBytes, numBytes);
    flush();
}
