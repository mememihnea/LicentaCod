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



const i2cInitialisationStructure i2cInitStructure =
{
		0x1E,									// Slave Address
		GPIO_PORT_P1,							// Selected Port for i2c Communication
		GPIO_PIN6 + GPIO_PIN7,					// Selected Pin/Pins for i2c Communication
		GPIO_PRIMARY_MODULE_FUNCTION,			// Selected Module Function
		EUSCI_B0_BASE,							// Selected Module
		EUSCI_B_I2C_TRANSMIT_MODE
};



int main(void)
{
	WDT_A_holdTimer();


	initI2C(&i2cInitStructure);
	initUART();

	ALSensorConfigure();

	while (1)
	{
		PCM_gotoLPM3();
	}
}




