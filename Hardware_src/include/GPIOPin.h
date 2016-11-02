#ifndef GPIOPIN_H
#define GPIOPIN_H
 
#include <Types.h>
#include <Debug.h>
using namespace std;

/**
 * @class GPIOPin
 * @brief A class representing a GPIO pin on the Raspberry Pi.
 * @author jessicamorris
 *
 * Sourced from http://hertaville.com/introduction-to-accessing-the-raspberry-pis-gpio-in-c.html,
 * modified to include my debugging stuff, better follow C++ naming conventions, and added
 * #defines for the magic numbers and other constants.
 */

/**
 * @var string pinNum
 * @brief The pin number of this GPIO pin object.
 */
 
/**
 * @fn int setDirection(string dir)
 * @brief Sets the direction of this GPIO pin.
 * Helper method for setOutput() and setInput().
 *  
 * @param[in] dir The direction of the pin, either GPIO_IN or GPIO_OUT.
 * @return 0 on success, -1 on failure.   
 */
 
/**
 * @fn int exportPin()
 * @brief Export this GPIO pin for use as input/output.
 * @return 0 on success, -1 on failure.  
 */
 
/**
 * @fn int unexportPin()
 * @brief Unexport this GPIO pin for no more use as input/output.
 * @return 0 on success, -1 on failure.  
 */
 
/**
 * @fn int setOutput()
 * @brief Designate this GPIO pin for output.
 * @return 0 on success, -1 on failure.  
 */
 
/**
 * @fn int setInput()
 * @brief Designate this GPIO pin for input.
 * @return 0 on success, -1 on failure.  
 */
 
/**
 * @fn int setValue(string value)
 * @brief Writes the bytes given by \a value to the GPIO pin.
 *  
 * @param[in] value The value to write to the GPIO pin.
 * @return 0 on success, -1 on failure. 
 *  
 * @warning The pin should be configured as input first with \a setInput()
 */
 
/**
 * @fn int getValue(string &value)
 * @brief Reads the bytes on the GPIO pin into \a value.
 *  
 * @param[out] value The value on the GPIO pin.
 * @return 0 on success, -1 on failure. 
 *  
 * @warning The pin should be configured as output first with \a setOutput()
 */

#define GPIO_IN "in"
#define GPIO_OUT "out"
#define GPIO_HIGH "1"
#define GPIO_LOW "0"

class GPIOPin
{
private:
    string pinNum;
    int setDirection(string dir); // Set the direction of this pin
 
public:
    GPIOPin(); // Default constructor, create a GPIOPin for GPIO4
    GPIOPin(string gnum); // Oveloaded constructor, create a GPIOPin for GPIOx
    string getPin(); // Returns the GPIO pin number
    int exportPin(); // Exports GPIO
    int unexportPin(); // Unexport GPIO
    int setOutput(); // Set up this pin as output
    int setInput(); // Set up this pin as input
    int setValue(string value); // Send value on the pin
    int getValue(string &value); // Get value from the pin
};
 
#endif // GPIOPIN_H
