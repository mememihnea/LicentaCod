/*
 * LightSensor.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */


#include "lightSensor.h"
#include "driverlib.h"
#include <msp.h>
/* Standard Includes */
#include <stdint.h>
#include <string.h>
#include "i2cCommunication.h"


void ALSensorConfigure(){
	i2cWritewithParam(0x00,0x01); // Selecting the ALS 0x00 0x01
	i2cWritewithParam(0x10,0x01); // Configuration for ALS: 0~5162 lux (resolution 0.0788 lux/count) 0x10 0x01
}

uint16_t ALSensorReadData(){
	uint8_t lowValue;
	uint8_t highValue;
	uint16_t finalValue = 0x00;
	uint16_t ALSdata;

	lowValue=i2cReadwithParam(0x0C);
	highValue=i2cReadwithParam(0x0D);

	/******** Combining the two 8bit registers into one 16bit register *********/

	finalValue = highValue;
	finalValue <<= 8;
	finalValue = finalValue + lowValue;
	/******** Ambiental Light Value Formula ***********/

	//ALSdata = finalValue * finalValue * 0.0788;

	return finalValue;
}
