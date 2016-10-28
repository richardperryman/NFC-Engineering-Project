#ifndef SERIAL_H
#define SERIAL_H

#include <string>
using namespace std;

/**
 * @class Serial
 * @brief A class representing a serial port on the Raspberry Pi.
 * @author jessicamorris
 *
 * Sourced from http://stackoverflow.com/questions/27650074/arduino-to-raspberrypi-serial-communication,
 * modified to use a class instead of a "serial" namespace. I'll flesh this out more as I learn
 * more about how it works.
 */

/**
 * @fn int openPort()
 * @brief Opens the port of this Serial object.
 * @return 0 on success, -1 on failure.
 */
 
/**
 * @fn string readData()
 * @brief Reads data in from this serial port's buffer.
 * @return The data as a string.
 * @warning Will return empty strings if the port has not been opened with openPort() first.
 */

class Serial
{
private:
	string portName;
	int serialPort;
public:
	Serial(); // Default constructor, for ACM0
	Serial(string portName);
	int openPort();
	string readData();
};

#endif // SERIAL_H
