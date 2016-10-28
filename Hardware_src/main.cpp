
#include <iostream>
#include <unistd.h>
#include <Serial.h>
#include <GPIOPin.h>


// For now this will just blink the LEDs
// n.b. Because the Pi is slow, it needs small sleeps during the pin setup
int main()
{
	GPIOPin* green = new GPIOPin("2");
	GPIOPin* blue = new GPIOPin("3");
	GPIOPin* red = new GPIOPin("4");
	
	green->exportPin();
	blue->exportPin();
	red->exportPin();
	
	sleep(1); // DO NOT REMOVE
	
	green->setOutput();
	blue->setOutput();
	red->setOutput();
	
	sleep(1); // DO NOT REMOVE
	
	cout << "Turning on LEDs!" << endl;
	
	green->setValue("1");
	sleep(3);
	green->setValue("0");
	blue->setValue("1");
	sleep(3);
	blue->setValue("0");
	red->setValue("1");
	sleep(3);
	red->setValue("0");
	
	cout << "Done" << endl;
	
	green->unexportPin();
	blue->unexportPin();
	red->unexportPin();
	
	return 0;
}
