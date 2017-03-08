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
    	    		    		    	        3000000,                                // SMCLK = 3MHz
    	    		    		    	        EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 100khz
    	    		    		    	        0,                                      // No byte counter threshold
    	    		    		    	        EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
    	    		    		    	    };

        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
        I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
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

    void ownDelay(int number){
    	int i=0;
    	while(i<number)
    		i++;
    }


    void i2cWritewithParam(uint8_t regAddress, uint8_t data){
    	//while(I2C_isBusBusy(EUSCI_B0_BASE)==EUSCI_B_I2C_BUS_BUSY);

    	I2C_masterSendMultiByteStart(EUSCI_B0_BASE,regAddress);
    	I2C_masterSendMultiByteNext(EUSCI_B0_BASE,data);
  		I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
    }

    uint8_t i2cReadwithParam(uint8_t regAddress){

    	static uint8_t readValue;

    	while(I2C_isBusBusy(EUSCI_B0_BASE));
    	while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE)==EUSCI_B_I2C_SENDING_STOP);
    	I2C_masterSendSingleByte(EUSCI_B0_BASE,regAddress);
    	while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE)==EUSCI_B_I2C_SENDING_STOP);
    	readValue=I2C_masterReceiveSingleByte(EUSCI_B0_BASE);

    	return readValue;
    }

    int main(void)
    {
        WDT_A_holdTimer();

        uint8_t readValue;

        initI2C();
        initUART();

        i2cWritewithParam(0,1); // Selecting the ALS 0x00 0x01
        while (1)
        {
        	readValue=i2cReadwithParam(0x0C);

        	sendInt(readValue);
        	sendString(" ");
        	ownDelay(1000);
        }
    }
