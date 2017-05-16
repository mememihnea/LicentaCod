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
#include "dht.h"

int main(void)
{
	WDT_A_holdTimer();

	configureDHT();
	//configurePorts();
	//rtcConfig();
	//sensorStart();
	//interruptFlag = 1;
	while (1)
	{
		humidity &= 0xffff;
		temperature &= 0xffff;
		readDHT();
		__delay_cycles(200000);
		__delay_cycles(200000);
		__delay_cycles(200000);
		__delay_cycles(200000);

//		if(interruptFlag==1)
//		{
//			readValue = sensorReadAndPowerDown();
//			sendInt(readValue);
//			interruptFlag=0;
//		}
//		else
//		{
//			PCM_gotoLPM3();
//			//PCM_shutdownDevice(PCM_LPM35_VCORE0);
//			//PCM_shutdownDevice(PCM_LPM35_VCORE0);
//		}
	}
	//initUART(&WiFiUARTInitStructure);
	//initUartPC();
	//rtcConfig();
	//sensorStart();
	//interruptFlag=0;

	//readValue = ALSensorReadData();
	//sendInt(readValue);
	//ownDelay(100000);

	//				if(interruptFlag==1)
	//				{
	//					readValue = ALSensorReadData();
	//					sendInt(readValue);
	//				}
	//				else
	//					PCM_gotoLPM3();
	//				interruptFlag=0;


	//wifiConnect();
}



