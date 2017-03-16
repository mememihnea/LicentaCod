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


int main(void)
{
	WDT_A_holdTimer();

	initUART();
	rtcConfig();
	ALSensorConfigure();

	while (1)
	{
		PCM_gotoLPM3();
	}
}




