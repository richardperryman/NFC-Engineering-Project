
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
	
	green->setValue(GPIO_HIGH);
	sleep(1);
	green->setValue(GPIO_LOW);
	blue->setValue(GPIO_HIGH);
	sleep(1);
	blue->setValue(GPIO_LOW);
	red->setValue(GPIO_HIGH);
	sleep(1);
	red->setValue(GPIO_LOW);
	
	cout << "Done" << endl;
	
	green->unexportPin();
	blue->unexportPin();
	red->unexportPin();
    
    Serial* usb = new Serial("/dev/ttyACM0");
    usb->openPort(SERIAL_BAUD_NFC);
    
    cout << usb->readChar();
    
    usb->closePort();
	
	return 0;
}
