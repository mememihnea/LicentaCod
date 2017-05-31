/*
 * realTimeClock.h
 *
 *  Created on: Mar 15, 2017
 *      Author: Mihnea
 */

#ifndef REALTIMECLOCK_H_
#define REALTIMECLOCK_H_

#include <stdint.h>
#include <msp.h>
#include "driverlib.h"


/* Statics */
//volatile bool interruptFlag=0;

void rtcConfig();
void RTC_C_IRQHandler(void);

#endif /* REALTIMECLOCK_H_ */
