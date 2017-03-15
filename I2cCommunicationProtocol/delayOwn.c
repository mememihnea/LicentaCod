/*
 * delayOwn.c
 *
 *  Created on: Mar 15, 2017
 *      Author: Mihnea
 */

#include "delayOwn.h"

#include "driverlib.h"
#include <msp.h>

/* Standard Includes */
#include <stdint.h>


void ownDelay(int number){
	int i=0;
	while(i<number)
		i++;
}


