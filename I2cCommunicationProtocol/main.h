/*
 * main.h
 *
 *  Created on: Mar 20, 2017
 *      Author: Mihnea
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <msp.h>
#include "driverlib.h"



const uartInitialisationStructure WiFiUARTInitStructure =
{
		GPIO_PORT_P3,							// Selected Port for UART Communication
		GPIO_PIN2 + GPIO_PIN3,					// Selected Pin/Pins for UART Communication
		GPIO_PRIMARY_MODULE_FUNCTION,			// Selected Module Function
		CS_DCO_FREQUENCY_12,					// Selected Clock Source
		EUSCI_A2_BASE							// Selected UART Module
};

const uartInitialisationStructure pcUARTInitStructure =
{
		GPIO_PORT_P1,							// Selected Port for UART Communication
		GPIO_PIN1 + GPIO_PIN2 + GPIO_PIN3,		// Selected Pin/Pins for UART Communication
		GPIO_PRIMARY_MODULE_FUNCTION,			// Selected Module Function
		CS_DCO_FREQUENCY_12,					// Selected Clock Source
		EUSCI_A0_BASE							// Selected UART Module
};


uint16_t readValue;

int main(void);

#endif /* MAIN_H_ */
