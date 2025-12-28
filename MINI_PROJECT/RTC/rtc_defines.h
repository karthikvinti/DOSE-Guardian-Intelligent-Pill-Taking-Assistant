#ifndef RTC_DEFINES_H          // Prevents multiple inclusion of this header file
#define RTC_DEFINES_H

/*---------------- System Clock Configuration ----------------*/

// External oscillator frequency (12 MHz crystal)	
#define FOSC 12000000        // Frequency of main oscillator in Hz

// CPU clock frequency derived using PLL (CCLK = 5 × FOSC)
#define CCLK (5*FOSC)        // Core clock frequency in Hz

// Peripheral clock frequency (PCLK = CCLK / 4)
#define PCLK (CCLK/4)        // Clock supplied to peripherals like RTC

/*---------------- RTC Prescaler Values ----------------*/
#define PREINT_VAL (int)((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

/*---------------- RTC CCR (Clock Control Register) Bits ----------------*/
#define RTC_ENABLE  1<<0
#define RTC_RESET   1<<1
#define RTC_CLKSRC  1<<4

#define SW1  16        // Input switch 1 pin number
#define SW2  17        // Input switch 2 pin number
#define BUZ  25        // Output pin for buzzer control
#define LED1 26        // Output pin for LED indication
#define LED2 27        // Output pin for LED indication 

#endif                 // End of RTC_DEFINES_H
