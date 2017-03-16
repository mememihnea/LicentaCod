/*
 * realTimeClock.c
 *
 *  Created on: Mar 15, 2017
 *      Author: Mihnea
 */

#include "realTimeClock.h"
#include "i2cCommunication.h"
#include "uartCommunication.h"
#include "lightSensor.h"
#include "delayOwn.h"

#include "driverlib.h"
#include <msp.h>

/* Standard Includes */
#include <stdint.h>

const RTC_C_Calendar currentTime =
{
		0x00,
		0x03,
		0x22,
		0x12,
		0x11,
		0x1955
};

void rtcConfig(){
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	CS_setExternalClockSourceFrequency(32000,48000000);
	/* Starting LFXT in non-bypass mode without a timeout. */
	CS_startLFXT(false);
	RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BCD);
	RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);
	//RTC_C->PS1CTL = RTC_C_PS1CTL_RT1IP__128 | RTC_C_PS1CTL_RT1PSIE;		// set divider to 128
	/* Enable interrupt for RTC Ready Status, which asserts when the RTC
	 * Calendar registers are ready to read.
	 * Also, enable interrupts for the Calendar alarm and Calendar event. */
	RTC_C_clearInterruptFlag(RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT | RTC_C_CLOCK_ALARM_INTERRUPT);
	RTC_C_enableInterrupt(RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT	| RTC_C_CLOCK_ALARM_INTERRUPT);
	RTC_C_startClock();
	Interrupt_enableInterrupt(INT_RTC_C);
	Interrupt_enableSleepOnIsrExit();
	Interrupt_enableMaster();
}

void RTC_C_IRQHandler(void)
{
	uint32_t status;
	uint16_t readValue;

	status = RTC_C_getEnabledInterruptStatus();
	RTC_C_clearInterruptFlag(status);

	if (status & RTC_C_TIME_EVENT_INTERRUPT)
	{
		/* Interrupts every minute - Set breakpoint here */
		readValue = ALSensorReadData();
		sendInt(readValue);
		ownDelay(100000);

		newTime = MAP_RTC_C_getCalendarTime();

		// Clear the pre-scalar timer interrupt flag
		//RTC_C->PS1CTL &= ~(RTC_C_PS1CTL_RT1PSIFG);
	}
}

