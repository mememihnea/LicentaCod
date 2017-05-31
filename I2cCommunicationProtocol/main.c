/* DriverLib Includes */
#include "driverlib.h"
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Own Libraries Includes */
#include "i2cCommunication.h"
#include "lightSensor.h"
#include "uartCommunication.h"
#include "delayOwn.h"
#include "realTimeCLock.h"
#include "main.h"
#include "temp_humidSensor.h"

int main(void)
{
	WDT_A_holdTimer();

	initUART(&pcUARTInitStructure);
	initUART(&WiFiUARTInitStructure);
	//rtcConfig();
	lightSensorStart();
	//configureDHT();
	configurePorts();

	while (1)
	{
		//PCM_gotoLPM3();

		//sendString(EUSCI_A2_BASE,'Salut');
//		UART_transmitData(EUSCI_A2_BASE, 'a'+1);
//		__delay_cycles(10000);

//		if(interruptFlag==1)
//		{
			readValue = lightSensorReadData();
			sendInt(EUSCI_A2_BASE, readValue);
			__delay_cycles(10000000);

//			readDHT();
//			sendInt(humidity/10);
//			sendInt(temperature/10);
//
//			interruptFlag=0;
//		}
//		else
//		{
//			PCM_gotoLPM3();
//		}
	}
}



