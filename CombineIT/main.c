#include "msp.h"
/* DriverLib Includes */
#include "driverlib.h"
/* StandardLib Includes */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */


/* Variables */
int i;
volatile bool stateChange=false;


/* UART Configuration */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,               // ACLK Clock SOurce
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1
        16000,                                  // 16000 tick period
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_SKIP_CLEAR                      // Clear value
};

void setup(){
		/* Halting WDT  */
	    WDT_A_holdTimer();

	    /* Selecting P1.2 and P1.3 in UART mode */
	    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
	            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	    /* Setting P1.0 as LED Output and P1.1 as input switch */
	    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
	    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
	    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
	    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

	    /* Setting up Timer_A to be sourced from ACLK and for ACLK to be sourced from
	      * the 128Khz REFO. Since the frequency of MCLK will be changed when we go
	      * into LF mode, we want to make our LED blink look consistent.
	      */
	     CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
	     Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
	     Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,
	             TIMER_A_CAPTURECOMPARE_REGISTER_0);


	    /* Setting DCO to 12MHz */
	    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

	    /* Configuring UART Module */
	    UART_initModule(EUSCI_A0_BASE, &uartConfig);

	    /* Enable UART module */
	    UART_enableModule(EUSCI_A0_BASE);

	    /* Enabling interrupts */
	    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	    Interrupt_enableInterrupt(INT_EUSCIA0);
	    Interrupt_enableInterrupt(INT_PORT1);
	    Interrupt_enableSleepOnIsrExit();
	    Interrupt_enableMaster();
	    Interrupt_disableSleepOnIsrExit();
	    SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
	    Interrupt_enableMaster();
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

void sendString(char string[]){
	for(i=0;i<strlen(string);i++)
	{
		UART_transmitData(EUSCI_A0_BASE, string[i]);
	}
}

void sendInt(int number){
	char *buffer;
	char str[10];
	buffer=intToString(number,str);
	sendString(buffer);
}

int main(void)
{
	setup();
	while(1)
	{
		if (stateChange)
		        {
		            MAP_Interrupt_disableMaster();

		            stateChange = false;

		            /* Re-enabling port pin interrupt */
		            GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
		            Interrupt_enableInterrupt(INT_PORT1);
		            Interrupt_enableMaster();

		            /* Change to new power state */
		            PCM_setPowerState(PCM_LPM3);

		        }
	}
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
    }

}

/* PORT1 ISR - Modifies the Power State*/
void PORT1_IRQHandler(void)
{
	uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
	 if (status & GPIO_PIN1)
	    {
		 //PCM_setPowerState(PCM_AM_LDO_VCORE0);
		 sendInt(PCM_getPowerState());
		 printf("%" PRIu8 "\n",PCM_getPowerState());
		 P1->OUT ^= BIT0;              // Toggle LED
	    }
}

/* TimerA0 ISR */
void TA0_0_IRQHandler(void)
{
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);

    stateChange = true;
    P1->OUT ^= BIT0;
    Timer_A_stopTimer(TIMER_A0_BASE);
    Interrupt_disableInterrupt(INT_TA0_0);
}
