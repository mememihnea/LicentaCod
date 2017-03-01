#include "msp.h"
#include "driverlib.h"

int main(void)
{
    /* Halting WDT  */
	MAP_WDT_A_holdTimer();

    //while(1);
    	//MAP_PCM_gotoLPM0();
}
