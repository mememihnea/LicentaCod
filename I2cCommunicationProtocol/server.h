/*
 * client.h
 *
 *  Created on: Apr 25, 2017
 *      Author: Mihnea
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdbool.h>
#include <stdint.h>
#include <msp.h>
#include "driverlib.h"

#define DEBUG true					//Prints AT responses to serial terminal, set to False if you do not want to print

void ESP_Config(); 					// This function configures the ESP module as Access Point
void serverSendDataToClient();		// This function sends the data from the MSP to the Python Client
void sendData(char command[], uint8_t timeout, bool debug);					// This function sends data from ESP

#endif /* SERVER_H_ */
