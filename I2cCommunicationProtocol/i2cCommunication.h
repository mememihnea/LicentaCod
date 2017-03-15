/*
 * i2c.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#ifndef I2CCOMMUNICATION_H_
#define I2CCOMMUNICATION_H_

#include <stdint.h>
#include <msp.h>
#include "driverlib.h"

typedef struct {
	uint8_t slaveAdress;
	uint8_t portSelect;
	uint16_t pinSelect;
	uint8_t moduleFunctionSelect;
	uint32_t moduleInstanceSelect;
	uint8_t functioningModeSelect;
}i2cInitialisationStructure;


void initI2C(const i2cInitialisationStructure *structure); // Configuration of i2c protocol
void i2cWritewithParam(uint8_t regAddress, uint8_t data); // This function WRITES data to the regAddress
uint8_t i2cReadwithParam(uint8_t regAddress); // This function READS data from the regAddress

#endif /* I2CCOMMUNICATION_H_ */
