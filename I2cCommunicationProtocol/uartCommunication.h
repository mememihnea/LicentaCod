/*
 * uart.h
 *
 *  Created on: Mar 8, 2017
 *      Author: Mihnea
 */

#ifndef UARTCOMMUNICATION_H_
#define UARTCOMMUNICATION_H_

#include <stdint.h>
#include <msp.h>
#include "driverlib.h"

void initUART(); //  Configuration of the UART protocol
void reverse(char s[]);
char *intToString(int number, char string[]);
void sendString(char string[]); // This function sends a String through the UART protocol
void sendInt(int number); // This function sends an Int through the UART protocol

#endif /* UARTCOMMUNICATION_H_ */
