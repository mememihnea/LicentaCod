/*
     *  Description: This example demonstrates the use case where the slave needs
     *  to initiate a transfer from the master. This is often done in sensor
     *  applications and other applications where the slave has to progressively
     *  send data to the master when there is new data (perhaps from a sensor) to
     *  be sent. A GPIO port is used to wake-up the master and initiate the
     *  master to read data from the slave. In this case, the first byte of the
     *  master read will be the number of bytes (including the length byte) that
     *  the master should read. The GPIO pin is set as an input with a pull-up
     *  enabled. This is the MASTER code.
     *
     *  ACLK = n/a, MCLK = HSMCLK = SMCLK = BRCLK = default DCO = ~3.0MHz
     *
     *                                /|\  /|\
     *                MSP432P401   10k  10k      MSP432P401
     *                   slave         |    |         master
     *             -----------------   |    |   -----------------
     *            |     P1.6/UCB0SDA|<-|----+->|P1.6/UCB0SDA     |
     *            |                 |  |       |                 |
     *            |                 |  |       |                 |
     *            |     P1.7/UCB0SCL|<-+------>|P1.7/UCB0SCL     |
     *            |                 |          |                 |
     *            |        P5.5/GPIO|<-------->|P5.5/GPIO        |
     *            |                 |          |                 |
     *
     * Author: Timothy Logan
     ******************************************************************************/
    /* DriverLib Includes */
    #include "driverlib.h"

    /* Standard Includes */
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>

    /* Slave Address */
    #define SLAVE_ADDRESS_1     0x64

	/* Variables */
	int i;
    volatile uint8_t RXData[4];
    volatile uint32_t xferIndex;
    volatile uint32_t numOfRecBytes;

    /* I2C Master Configuration Parameter */
    const eUSCI_I2C_MasterConfig i2cConfig =
    {
            EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
            3000000,                                // SMCLK = 3MHz
            EUSCI_B_I2C_SET_DATA_RATE_100KBPS,      // Desired I2C Clock of 400khz
            0,                                      // No byte counter threshold
            EUSCI_B_I2C_NO_AUTO_STOP                // No Autostop
    };

    /* UART Configuration Parameter*/
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

    void setup(){
    	 	 	 /* Disabling the Watchdog  */
    	        WDT_A_holdTimer();
    	        xferIndex = 0;

    	        				/**************** UART *************/

    	        /* Selecting P1.2 and P1.3 in UART mode */
    	        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
    	        	            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    	        /* Setting DCO to 12MHz */
    	        CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    	        /* Configuring UART Module */
    	        UART_initModule(EUSCI_A0_BASE, &uartConfig);

    	        /* Enable UART module */
    	        UART_enableModule(EUSCI_A0_BASE);

    							/**************** I2c and GPIO *************/

    	        /* Select Port 1 for I2C - Set Pin 6, 7 to input Primary Module Function,
    	         *   (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL). and setting P5.5 for input mode
    	         *   with pull-up enabled
    	         */
    	        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
    	                GPIO_PIN6 + GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
    	        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);

    	        /* Initializing I2C Master to SMCLK at 100khz with no autostop */
    	        I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
    	        I2C_setSlaveAddress(EUSCI_B0_BASE,SLAVE_ADDRESS_1);

    	        /* Set in receive mode */
    	        I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_RECEIVE_MODE);
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
    	volatile uint8_t test=0;
    	    	setup();
    	    	while(1){
    	    		while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE==EUSCI_B_I2C_SENDING_STOP))
    	    		I2C_masterSendSingleByte(EUSCI_B0_BASE,0x1B);
    	    		while(MAP_I2C_masterIsStopSent(EUSCI_B0_BASE==EUSCI_B_I2C_SENDING_STOP))
    	    		test=I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
    	    		sendInt(test);
    	    	}
     }


