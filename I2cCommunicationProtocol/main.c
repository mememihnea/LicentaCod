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

int main(void)
{
	WDT_A_holdTimer();


	initUART();
	rtcConfig();
	sensorStart();
	//interruptFlag=0;

	readValue = ALSensorReadData();
	sendInt(readValue);
	//ownDelay(100000);

	while (1)
	{
		if(interruptFlag==1)
		{
			sendString("Outside Interrupt");
			readValue = ALSensorReadData();
			sendInt(readValue);
			//ownDelay(100000);
		}
		else
			PCM_gotoLPM3();
		interruptFlag=0;

	}
}



