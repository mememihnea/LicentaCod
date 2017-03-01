#include "msp.h"

/* DriverLib Include */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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

void Setup()
{
	WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

	/* Setting P1.0 as LED Output and P1.1 as input switch */
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
	Interrupt_enableInterrupt(INT_PORT1);
	Interrupt_enableMaster();

}


void main(void)
{
	Setup();
	
	Interrupt_disableSleepOnIsrExit();
	while(1)
	{
		PCM_setPowerState(PCM_LPM3); // nu iasa din LPM3 ; Exit LPM3 Interrupt Service Routine		BIC #CPUOFF+SCG1+SCG0,0(SP) ; Exit LPM3 on RETI
		//pagina 40/644 din family user guide
	}
}


void PORT1_IRQHandler(void)
{
	uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
	PCM_setPowerState(PCM_AM_LDO_VCORE0);
	//P1->OUT ^= BIT0;              // Toggle LED

}
