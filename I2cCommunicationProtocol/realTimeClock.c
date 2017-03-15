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

/* Statics */
 static volatile RTC_C_Calendar newTime;

/* Time is November 12th 1955 10:03:00 PM */
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
	/* Configuring pins for peripheral/crystal usage and LED for output */
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

	/* Setting the external clock frequency. This API is optional, but will
	 * come in handy if the user ever wants to use the getMCLK/getACLK/etc
	 * functions
	 */
	CS_setExternalClockSourceFrequency(32000,48000000);

	/* Starting LFXT in non-bypass mode without a timeout. */
	CS_startLFXT(false);
	/* Initializing RTC with current time as described in time in
	 * definitions section */
	RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BCD);
	/* Specify an interrupt to assert every minute */
	RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);

	/* Enable interrupt for RTC Ready Status, which asserts when the RTC
	 * Calendar registers are ready to read.
	 * Also, enable interrupts for the Calendar alarm and Calendar event. */
	RTC_C_clearInterruptFlag(
			RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
			| RTC_C_CLOCK_ALARM_INTERRUPT);
	RTC_C_enableInterrupt(
			RTC_C_CLOCK_READ_READY_INTERRUPT | RTC_C_TIME_EVENT_INTERRUPT
			| RTC_C_CLOCK_ALARM_INTERRUPT);

	/* Start RTC Clock */
	RTC_C_startClock();

	/* Enable interrupts and go to sleep. */
	Interrupt_enableInterrupt(INT_RTC_C);
	Interrupt_enableSleepOnIsrExit();
	Interrupt_enableMaster();
}

/* RTC ISR */
void RTC_C_IRQHandler(void)
{
    uint32_t status;
    uint16_t readValue;


    status = MAP_RTC_C_getEnabledInterruptStatus();
    MAP_RTC_C_clearInterruptFlag(status);

    if (status & RTC_C_CLOCK_READ_READY_INTERRUPT)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }

    if (status & RTC_C_TIME_EVENT_INTERRUPT)
    {
        /* Interrupts every minute - Set breakpoint here */
    	readValue = ALSensorReadData();
    	sendInt(readValue);
    	ownDelay(100000);

        newTime = MAP_RTC_C_getCalendarTime();
    }
}

