/* DriverLib Includes */
#include "driverlib.h"
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "i2cCommunication.h"
/* Own Libraries Includes */
#include "lightSensor.h"
#include "uartCommunication.h"

void ownDelay(int number){
	int i=0;
	while(i<number)
		i++;
}



int main(void)
{
	WDT_A_holdTimer();

	uint16_t readValue;

	initI2C(&i2cInitStructure);
	initUART();

	ALSensorConfigure();

	while (1)
	{
		readValue = ALSensorReadData();
		sendInt(readValue);
		ownDelay(100000);
	}
}
