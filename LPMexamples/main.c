
/* MSP432 PCM - Going to LPM3
 *
 * Description: In this very simple example, the use of the PCM API to go to
 * LPM3 is demonstrated. The device is configured for GPIO interrupt
 * (to wake the device up when the user presses the button on P1.1) and then
 * the device is put into LPM3 using the PCM_gotoLPM3 function. Furthermore,
 * the device is configured in a manner to minimize power consumption and
 * achieve datasheet specifications with regard to power consumption.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |            P1.1  |<--Toggle Switch
 *            |                  |
 *            |                  |
 *
 * Author: Timothy Logan
 ******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#define BUTTON
int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P1.0 as output and P1.1 (switch) as input */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring P1.1 as an input and enabling interrupts */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableSleepOnIsrExit();



    /* Going to LPM3 */
    while (1)
    {
        /* Note that while this examples just goes to LPM3, LPM4 is essentially
            just LPM3 with WDT_A/RTC_C disabled. For convenience, the user can
            use the MAP_PCM_gotoLPM4() function if they want the API to handle
            the disabling of these components */
        MAP_PCM_gotoLPM3();
    }
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    }

}

