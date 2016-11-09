
#include <fstream>
#include <iostream>
#include <GPIOPin.h>

GPIOPin::GPIOPin()
{
    this->pinNum = "4"; // GPIO4 is default because reasons
}

GPIOPin::GPIOPin(std::string gnum)
{
	this->pinNum = gnum;
}
 
std::string GPIOPin::getPin()
{
    return this->pinNum;
}

int GPIOPin::exportPin()
{
	std::string export_path = "/sys/class/gpio/export";
	std::ofstream exportgpio(export_path.c_str()); // Open export file
	
	if (!exportgpio)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to export GPIO"+this->pinNum);
		return -1;
	}
	
	exportgpio << this->pinNum;
	exportgpio.close();
	return 0;
}

int GPIOPin::unexportPin()
{
	std::string unexport_path = "/sys/class/gpio/unexport";
	std::ofstream unexportgpio(unexport_path.c_str()); // Open unexport file
	
	if (!unexportgpio)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to unexport GPIO"+this->pinNum);
		return -1;
	}
	
	unexportgpio << this->pinNum;
	unexportgpio.close();
	return 0;
}

int GPIOPin::setDirection(std::string dir)
{
	std::string setdir_path = "/sys/class/gpio/gpio" + this->pinNum + "/direction";
	std::ofstream setdirgpio(setdir_path.c_str()); // Open direction file for this pin
	
	if (!setdirgpio)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to set direction of GPIO"+this->pinNum);
	}
	
	setdirgpio << dir;
	setdirgpio.close();
	return 0;
}

int GPIOPin::setOutput()
{
	return setDirection(GPIO_OUT);
}

int GPIOPin::setInput()
{
	return setDirection(GPIO_IN);
}

int GPIOPin::setValue(std::string value)
{
	std::string setval_path = "/sys/class/gpio/gpio" + this->pinNum + "/value";
	std::ofstream setvalgpio(setval_path.c_str()); // Open value file for this pin
	
	if (!setvalgpio)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to set the value of GPIO"+this->pinNum);
		return -1;
	}
	
	setvalgpio << value;
	setvalgpio.close();
	return 0;
}

int GPIOPin::getValue(std::string &value)
{
	std::string getval_path = "/sys/class/gpio/gpio" + this->pinNum + "/value";
	std::ifstream getvalgpio(getval_path.c_str()); // Open value file for this pin
	
	if (!getvalgpio)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to get value of GPIO"+this->pinNum);
		return -1;
	}
	
	getvalgpio >> value;
	if (value != GPIO_LOW)
		value = GPIO_HIGH;
	else
		value = GPIO_LOW;
	
	getvalgpio.close();
	return 0;
}
