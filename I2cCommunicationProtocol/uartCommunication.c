/*
 * uart.c
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#include "uartCommunication.h"

#include "driverlib.h"
#include <msp.h>
/* Standard Includes */
#include <stdint.h>
#include <string.h>

volatile int i;

const eUSCI_UART_Config uartConfig =
{
		EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
		6,                                     // BRDIV = 78
		8,                                       // UCxBRF = 2
		20,                                       // UCxBRS = 0
		EUSCI_A_UART_NO_PARITY,                  // No Parity
		EUSCI_A_UART_LSB_FIRST,                  // LSB First
		EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
		EUSCI_A_UART_MODE,                       // UART mode
		EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

void initUART(const uartInitialisationStructure *structure){
	GPIO_setAsPeripheralModuleFunctionInputPin(structure->portSelect, structure->pinSelect, structure->moduleFunctionSelect);
	CS_setDCOCenteredFrequency(structure->clockSelect);
	UART_initModule(structure->uartModuleSelect, &uartConfig);
	UART_enableModule(structure->uartModuleSelect);
}

void reverse(char s[])
{
	int c,i,j;

	for(i = 0,j = strlen(s)-1; i < j; i++,j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


char *intToString(int number, char string[])
{
	int i, sign;

	if ((sign = number) < 0)  /* record sign */
		number = -number;          /* make n positive */
	i = 0;

	do {       /* generate digits in reverse order */
		string[i++] = number % 10 + '0';   /* get next digit */
	} while ((number /= 10) > 0);     /* delete it */

	if (sign < 0)
		string[i++] = '-';
	string[i] = '\0';
	reverse(string);
	return string;
}

void sendString(uint32_t module ,char string[]){
	for(i=0; i<strlen(string); i++){
		UART_transmitData(module, string[i]);
		__delay_cycles(10000);
	}
	UART_transmitData(module, ' ');
}

void sendInt(uint32_t module, int number){
	char *buffer;
	char str[10];
	buffer=intToString(number,str);
	sendString(module, buffer);
}

