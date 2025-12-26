#include"types.h"

// Function to generate delay in microseconds
void delay_us(u32 tdly)
{
	 tdly=tdly*12;      // Convert microseconds into loop count (based on CPU clock)
	 while(tdly--);     // Empty loop to waste time and create delay
}

// Function to generate delay in milliseconds
void delay_ms(u32 tdly)
{
	  tdly=tdly*12000;   // Convert milliseconds into loop count
	  while(tdly--);     // Empty loop to generate millisecond delay
}

// Function to generate delay in seconds
void delay_s(u32 tdly)
{
	  tdly=tdly*12000000;  // Convert seconds into loop count
	  while(tdly--);       // Empty loop to generate second delay
}
