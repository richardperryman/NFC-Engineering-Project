
#include <fstream>
#include <iostream>
#include <GPIOPin.h>
 
using namespace std;

GPIOPin::GPIOPin()
{
    this->pinNum = "4"; // GPIO4 is default because reasons
}

GPIOPin::GPIOPin(string gnum)
{
	this->pinNum = gnum;
}
 
string GPIOPin::getPin()
{
    return this->pinNum;
}

int GPIOPin::exportPin()
{
	string export_path = "/sys/class/gpio/export";
	ofstream exportgpio(export_path.c_str()); // Open export file
	
	if (exportgpio < 0)
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
	string unexport_path = "/sys/class/gpio/unexport";
	ofstream unexportgpio(unexport_path.c_str()); // Open unexport file
	
	if (unexportgpio < 0)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to unexport GPIO"+this->pinNum);
		return -1;
	}
	
	unexportgpio << this->pinNum;
	unexportgpio.close();
	return 0;
}

int GPIOPin::setDirection(string dir)
{
	string setdir_path = "/sys/class/gpio/gpio" + this->pinNum + "/direction";
	ofstream setdirgpio(setdir_path.c_str()); // Open direction file for this pin
	
	if (setdirgpio < 0)
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

int GPIOPin::setValue(string value)
{
	string setval_path = "/sys/class/gpio/gpio" + this->pinNum + "/value";
	ofstream setvalgpio(setval_path.c_str()); // Open value file for this pin
	
	if (setvalgpio < 0)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to set the value of GPIO"+this->pinNum);
		return -1;
	}
	
	setvalgpio << value;
	setvalgpio.close();
	return 0;
}

int GPIOPin::getValue(string &value)
{
	string getval_path = "/sys/class/gpio/gpio" + this->pinNum + "/value";
	ifstream getvalgpio(getval_path.c_str()); // Open value file for this pin
	
	if (getvalgpio < 0)
	{
		DEBUG_LOG(CRITICAL, __FUNCTION__, "Unable to get value of GPIO"+this->pinNum);
		return -1;
	}
	
	getvalgpio >> value;
	if (value != "0")
		value = "1";
	else
		value = "0";
	
	getvalgpio.close();
	return 0;
}
