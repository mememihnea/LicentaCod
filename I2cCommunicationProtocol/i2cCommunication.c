/*
 * i2c.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#include "i2cCommunication.h"

#include "driverlib.h"
#include <msp.h>

/* Standard Includes */
#include <stdint.h>



/* Application Defines */
//#define SLAVE_ADDRESS_1     0x1E


const eUSCI_I2C_MasterConfig i2cConfig =
{
		EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
		3000000,                                // SMCLK = 3MHz
		EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
		0,                                      // No byte counter threshold
		EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
};


void initI2C(uint8_t slaveAdress){
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
	I2C_setSlaveAddress(EUSCI_B0_BASE,slaveAdress);
	I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
	I2C_enableModule(EUSCI_B0_BASE);
}

void i2cWritewithParam(uint8_t regAddress, uint8_t data){
	I2C_masterSendMultiByteStart(EUSCI_B0_BASE,regAddress);
	I2C_masterSendMultiByteNext(EUSCI_B0_BASE,data);
	I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
}

uint8_t i2cReadwithParam(uint8_t regAddress){
	static uint8_t readValue;

	while(I2C_isBusBusy(EUSCI_B0_BASE));
	while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE)==EUSCI_B_I2C_SENDING_STOP);
	I2C_masterSendSingleByte(EUSCI_B0_BASE,regAddress);
	while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE)==EUSCI_B_I2C_SENDING_STOP);
	readValue=I2C_masterReceiveSingleByte(EUSCI_B0_BASE);

	return readValue;
}
