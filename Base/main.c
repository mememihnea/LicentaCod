#include "msp.h"
/* DriverLib Includes */
#include "driverlib.h"
/* StandardLib Includes */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define LED0 BIT0
#define LED1 BIT6
#define BUTTON BIT4


void setup(){
		/* Halting WDT  */
	    MAP_WDT_A_holdTimer();

	    P1DIR |= (LED0 + LED1); // Set P1.0 to output direction
	    // P1.3 must stay at input
	    P1OUT &= ~(LED0 + LED1); // set P1.0 to 0 (LED OFF)
	    P1IE |= BUTTON; // P1.3 interrupt enabled

	    P1IFG &= ~BUTTON; // P1.3 IFG cleared
	    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
	    MAP_Interrupt_enableInterrupt(INT_PORT1);
	    MAP_Interrupt_enableSleepOnIsrExit();
}



int main(void)
{
	setup();

    while(1)
    {
           MAP_PCM_gotoLPM3();
    }
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);
    P1OUT ^= (LED0 + LED1); // P1.0 = toggle
    P1IFG &= ~BUTTON; // P1.3 IFG cleared
    P1IES ^= BUTTON; // toggle the interrupt edge,
}



