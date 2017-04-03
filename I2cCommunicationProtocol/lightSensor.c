/*
 * LightSensor.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#include "delayOwn.h"
#include "lightSensor.h"
#include "i2cCommunication.h"
#include "driverlib.h"
#include <msp.h>

/* Standard Includes */
#include <stdint.h>
#include <string.h>

const i2cInitialisationStructure i2cInitStructure =
{
		0x1E,									// Slave Address
		GPIO_PORT_P1,							// Selected Port for i2c Communication
		GPIO_PIN6 + GPIO_PIN7,					// Selected Pin/Pins for i2c Communication
		GPIO_PRIMARY_MODULE_FUNCTION,			// Selected Module Function
		EUSCI_B0_BASE,							// Selected Module
		EUSCI_B_I2C_TRANSMIT_MODE				// Selected Functioning mode
};


void sensorStart(){
	initI2C(&i2cInitStructure);
	ownDelay(300);
	i2cWritewithParam(0x00,0x01); 				// Selecting the ALS 0x00 0x01
	i2cWritewithParam(0x10,0x01); 				// Configuration for ALS: 0~5162 lux (resolution 0.0788 lux/count) 0x10 0x01
}


void sensorStop(){
	initI2C(&i2cInitStructure);
	ownDelay(300);
	i2cWritewithParam(0x00,0x00); 				// Putting the sensor to sleep
}

void sensorReadAndPowerDown(){
	initI2C(&i2cInitStructure);
	i2cWritewithParam(0x00,0x05); 				// Selecting the ALS read once mode 0x00 0x05
	i2cWritewithParam(0x10,0x01); 				// Configuration for ALS: 0~5162 lux (resolution 0.0788 lux/count) 0x10 0x01
}

uint16_t ALSensorReadData(){
	uint8_t lowValue;
	uint8_t highValue;
	uint16_t finalValue = 0x00;

	lowValue=i2cReadwithParam(0x0C);
	highValue=i2cReadwithParam(0x0D);

	/******** Combining the two 8bit registers into one 16bit register *********/

	finalValue = highValue;
	finalValue <<= 8;
	finalValue = finalValue + lowValue;

	return finalValue;
}
