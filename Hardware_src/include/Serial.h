#ifndef SERIAL_H
#define SERIAL_H

#include <Types.h>
using namespace std;

/**
 * @class Serial
 * @brief A class representing a serial port on the Raspberry Pi.
 * @author jessicamorris
 *
 * Sourced from https://git.drogon.net?p=wiringPi;a=blob;f=wiringPi/wiringSerial.c
 * and modified for use in C++ (yay classes) instead of C.
 * The flags used in serial port programming come from termios:
 * https://en.wikibooks.org/wiki/Serial_Programming/termios
 * 
 */

/**
 * @fn int openPort()
 * @brief Opens the port of this Serial.
 * @return 0 on success, -1 on failure.
 */
 
/**
 * @fn void closePort()
 * @brief Closes this Serial's port.
 * @warning Does nothing if the port has not been opened with openPort() first.
 */
 
/**
 * @fn void flush()
 * @brief Flushes this Serial's buffer.
 * @warning Does nothing if the port has not been opened with openPort() first.
 */
 
/**
 * @fn int dataAvailable()
 * @brief Checks how many bytes are available to read from this Serial's buffer.
 * @return The number of bytes in the buffer to read, or -1 if there was an issue.
 */
 
/**
 * @fn char readChar()
 * @brief Reads a character in from this serial port's buffer.
 * @return The character read
 * @warning Will return -1 if the port has not been opened with openPort() first.
 */

/**
 * @fn int writeData(string data)
 * @brief Write a string of bytes to this port.
 * 
 */
 
#define SERIAL_BAUD_NFC 115200
// As I discover what other baud rates are useful for, I'll define them here

class Serial
{
private:
	string portName;
	int serialPort;
public:
	Serial(string portName);
	int openPort(int baudRate);
	void closePort();
	void flush();
	int dataAvailable();
    char readChar();
	void writeData(string data);
};

#endif // SERIAL_H
