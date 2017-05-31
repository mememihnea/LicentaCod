/* DriverLib Includes */
#include "driverlib.h"
#include "msp.h"

/* Standard Includes */
#include <stdint.h>

/* Own Libraries Includes */
#include "temp_humidSensor.h"


void configurePorts(void)
{
	//MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, PIN_ALL8);
	//MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P1, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, PIN_ALL8);
	//MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P3, PIN_ALL8);
	//MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P4, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, PIN_ALL8);
	//MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P5, PIN_ALL8);
//	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P6, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P7, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P7, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P8, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P8, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P9, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P9, PIN_ALL8);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P10, PIN_ALL8);
	MAP_GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P10, PIN_ALL8);
}

void initMCU()
{
	WDT_A_holdTimer();

	/* CLOCK Initialization*/

	/* Configuring pins for peripheral/crystal usage and LED for output */
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
			GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_CS_setExternalClockSourceFrequency(32768,48000000);

	/* Starting HFXT in non-bypass mode without a timeout. Before we start
	 * we have to change VCORE to 1 to support the 48MHz frequency */
	MAP_PCM_setCoreVoltageLevel(PCM_AM_LDO_VCORE1);//PCM_AM_LDO_VCORE1//PCM_VCORE1
	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	MAP_CS_startHFXT(false);
	//MAP_CS_startLFXT(false);
	/* Initializing MCLK to HFXT (effectively 48MHz) */
	MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
#ifdef USE_SYSTICK
	/* Systick Initialization*/
	/* Configuring SysTick to trigger at 24000000 (MCLK is 48MHz so this will
	 * make it toggle every 0.5s) */
	MAP_SysTick_enableModule();
	MAP_SysTick_setPeriod(24000000);
	MAP_SysTick_enableInterrupt();
#endif
}

void initTimer()
{
	MAP_Timer32_initModule(TIMER32_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
			TIMER32_FREE_RUN_MODE);
	MAP_Timer32_setCount(TIMER32_BASE,0xffffffff);
	MAP_Timer32_startTimer(TIMER32_BASE, false);
}

void setDHTAsInput()
{
	MAP_GPIO_setAsInputPin(GPIO_PORT_P5, GPIO_PIN5);
	//MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN5);
	MAP_GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN5);
}
void setDHTAsOutput()
{
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN5);
}
void setDHTHigh()
{
	GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN5);
}
void setDHTLow()
{
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN5);
}

uint8_t getDHTPin()
{
	return GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5);
}
void readDHT()
{
	humidity = 0;
	temperature = 0;
	setDHTAsOutput();
	setDHTLow();
	DELAY_10ms;
	setDHTHigh();
	DELAY_40us;
	bitCounter = 0;
	setDHTAsInput();
	__delay_cycles(200000);
	__delay_cycles(200000);
	__delay_cycles(200000);
	__delay_cycles(200000);
}

void configureDHT()
{
	initMCU();
	initTimer();
	Interrupt_enableInterrupt(INT_PORT5);
	Interrupt_enableMaster();
}

void PORT5_IRQHandler(void)
{

	uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5,status);

	if (GPIO_PIN5 & status)
	{
		currentBitTime = MAP_Timer32_getValue(TIMER32_BASE);
		bitTime[bitCounter] = bitTimePrev - currentBitTime;
		if(bitTime[bitCounter] > 500)
		{
			bitTable[bitCounter] = 1;
			if(bitCounter < 18 && bitCounter > 1)
			{
				humidity |= 1<<(17-bitCounter);
			}
			if(bitCounter < 33 && bitCounter > 16)
			{
				temperature |= 1<<(33-bitCounter);
			}

		}

		if(bitCounter++ > MAX_BITS)
			bitCounter = 0;

		bitTimePrev = currentBitTime;
	}
}
void SysTick_Handler(void)
{
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN5);
}
