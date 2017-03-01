 /* MSP432 PCM - LPM4.5 No Wakeup
 *
 * Description: In this example, the PCM's ability to go to LPM4.5 and shutdown
 * the device is demonstrated with the PCM DriverLib APIs. The device is setup
 * to interrupt when the switch connected to P1.1 is pressed. When woken up,
 * the device will go into LPM4.5 using the appropriate PCM API.
 */

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>

int main(void)
{
    /* Halting the Watchdog */
    WDT_A_holdTimer();

    /* Configuring P1.1 as an input and enabling interrupts */
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    Interrupt_enableInterrupt(INT_PORT1);

    /* Going to LPM3 and waiting for wake-up*/
    PCM_gotoLPM3();
    //PCM_shutdownDevice(PCM_LPM45);

    /* Execution should shut down */
    while(1);
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_getEnabledInterruptStatus(GPIO_PORT_P1));
}
