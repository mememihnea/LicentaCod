/*
 * client.c
 *
 *  Created on: Apr 25, 2017
 *      Author: Mihnea
 */

#include "server.h"
#include "uartCommunication.h"
#include "driverlib.h"
#include <msp.h>
#include <stdbool.h>

/* Standard Includes */
#include <stdint.h>

void ESP_Config(){
	initUART();

}

void serverSendDataToClient(){

}

char sendData(char command[], uint8_t timeout, bool debug){
	char output[] = "";


	return output;
}
