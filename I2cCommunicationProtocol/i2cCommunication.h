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

void initI2C(uint8_t slaveAdress); // Configuration of i2c protocol
void i2cWritewithParam(uint8_t regAddress, uint8_t data); // This function WRITES data to the regAddress
uint8_t i2cReadwithParam(uint8_t regAddress); // This function READS data from the regAddress

#endif /* I2CCOMMUNICATION_H_ */
