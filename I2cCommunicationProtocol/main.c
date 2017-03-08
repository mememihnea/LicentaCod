/* DriverLib Includes */
#include "driverlib.h"
#include "msp.h"
/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
/* Own Libraries Includes */
#include "i2c.h"
#include "uart.h"
#include "lightSensor.h"



void ownDelay(int number){
	int i=0;
	while(i<number)
		i++;
}



int main(void)
{
	WDT_A_holdTimer();

	uint8_t readValue;

	initI2C();
	initUART();

	ALSensorConfigure();
	while (1)
	{
		readValue = ALSensorReadData();
		sendInt(readValue);

		ownDelay(1000);
	}
}
