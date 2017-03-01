/* DriverLib Includes */
    #include "driverlib.h"
	#include "msp.h"
    /* Standard Includes */
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>

	/* Application Defines */
	#define SLAVE_ADDRESS_1     0x1E

	volatile int i;


    void initI2C(){
    	const eUSCI_I2C_MasterConfig i2cConfig =
    	    {
    	        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    	        48000000,                                // SMCLK = 3MHz
    	        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
    	        0,                                      // No byte counter threshold
    	        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
    	    };

        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
        I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
        I2C_initSlave(EUSCI_B0_BASE,SLAVE_ADDRESS_1, EUSCI_B_I2C_OWN_ADDRESS_OFFSET0, EUSCI_B_I2C_OWN_ADDRESS_ENABLE);
        I2C_setSlaveAddress(EUSCI_B0_BASE,SLAVE_ADDRESS_1);
        I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
        I2C_enableModule(EUSCI_B0_BASE);


    }

    void initUART(){
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

    		    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    		    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    		    UART_initModule(EUSCI_A0_BASE, &uartConfig);
    		    UART_enableModule(EUSCI_A0_BASE);
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


    void i2cWrite(){
    	I2C_masterSendMultiByteStart(EUSCI_B0_BASE,0x00);
    	I2C_masterSendMultiByteNext(EUSCI_B0_BASE,0x01);
    	I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
    }

    int i2cRead(){
    	int retVal;
    	I2C_slavePutData(EUSCI_B0_BASE,0x0C);
    	retVal=I2C_slaveGetData(EUSCI_B0_BASE);
    	return retVal;
    }

    void i2cRead2(){
    	I2C_masterSendSingleByte(EUSCI_B0_BASE,0x0C);
    }

    int main(void)
    {
        WDT_A_holdTimer();
        uint8_t test;

        initI2C();
        initUART();

        while (1)
        {

        	i2cWrite();
     		//i2cRead2();
        }
    }
