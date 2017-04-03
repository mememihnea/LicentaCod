/*
 * realTimeClock.c
 *
 *  Created on: Mar 15, 2017
 *      Author: Mihnea
 */

#include "realTimeClock.h"

#include "driverlib.h"
#include <msp.h>
#include "lightSensor.h"
#include "i2cCommunication.h"
#include "uartCommunication.h"


/* Standard Includes */
#include <stdint.h>

const RTC_C_Calendar currentTime =
{
		0x00,						// Second
		0x03,						// Minute
		0x22,						// Hour
		0x12,						// Day
		0x11,						// Month
		0x1955						// Year
};

void rtcConfig(){
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	CS_setExternalClockSourceFrequency(32000,48000000);
	CS_startLFXT(false);
	RTC_C_initCalendar(&currentTime, RTC_C_FORMAT_BCD);
	RTC_C_setCalendarEvent(RTC_C_CALENDAREVENT_MINUTECHANGE);
	RTC_C_definePrescaleEvent(RTC_C_PRESCALE_0,RTC_C_PSEVENTDIVIDER_128);
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
	static volatile RTC_C_Calendar newTime;


	status = RTC_C_getEnabledInterruptStatus();
	RTC_C_clearInterruptFlag(status);

	if (status & RTC_C_TIME_EVENT_INTERRUPT)
	{
		/* Interrupts every minute - Set breakpoint here */


		sendString("Inside Interrupt");
		interruptFlag=1;
		newTime = RTC_C_getCalendarTime();
	}
}

