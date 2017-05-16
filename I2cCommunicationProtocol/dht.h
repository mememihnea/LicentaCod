/*
 * dht.h
 *
 *  Created on: May 16, 2017
 *      Author: Mihnea
 */
#ifndef DHT_H_
#define DHT_H_

#include <stdint.h>
#include <msp.h>
#include "driverlib.h"

#define DELAY_10us __delay_cycles(19)
#define DELAY_1ms __delay_cycles(2500)
#define DELAY_10ms __delay_cycles(20000)
#define DELAY_40us __delay_cycles(80)

#define MAX_BITS 100
volatile uint32_t bitTime[MAX_BITS];
volatile uint32_t bitCounter;
volatile uint32_t bitTimePrev;
volatile uint32_t currentBitTime;
volatile uint32_t bitTable[MAX_BITS];

volatile uint32_t humidity;
volatile uint32_t temperature;


void configurePorts(void);
void initMCU();
void initTimer();
void setDHTAsInput();
void setDHTAsOutput();
void setDHTHigh();
void setDHTLow();
uint8_t getDHTPin();
void readDHT();
void configureDHT();
void SysTick_Handler(void);
void PORT5_IRQHandler(void);


#endif /* DHT_H_ */
