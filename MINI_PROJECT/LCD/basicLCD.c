#include <LPC21XX.h>
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "macros.h"

// Function to initialize the LCD
void InitLCD(void)
{
	  // Configure LCD data pins and control pins (RS, RW, EN) as OUTPUT
	  IODIR0 |= (0xff<<LCD_data)|(1<<RS)|(1<<RW)|(1<<EN);
	  delay_ms(20);    // Wait for LCD power-on stabilization
	  CmdLCD(0x30);    // LCD wake-up command (first time)
	  delay_ms(8);     // Delay required by LCD
	  CmdLCD(0x30);    // LCD wake-up command (second time)
	  delay_ms(1);     // Small delay
	  CmdLCD(0x30);    // LCD wake-up command (third time)
	  delay_ms(1);     // Small delay
	  CmdLCD(0x38);    // Set LCD to 8-bit mode, 2 lines, 5x7 font
	  CmdLCD(0x08);    // Display OFF
	  CmdLCD(0x01);    // Clear LCD display
	  CmdLCD(0x06);    // Entry mode: cursor moves right
	  CmdLCD(0x0C);    // Display ON, cursor OFF
}

// Function to send data or command to LCD
void DispLCD(u8 val)
{
	    IOCLR0 = 1<<RW;                   // Set RW = 0 (write operation)
	    WRITEBYTE(IOPIN0,LCD_data,val);   // Place data/command on LCD data pins
	    IOSET0 = 1<<EN;                   // Enable LCD (EN = 1)
	    delay_ms(2);                      // Wait for LCD to read data
	    IOCLR0 =1<<EN;                    // Disable LCD (EN = 0)
	    delay_us(2);                      // Small delay for LCD processing 
} 

// Function to send a command to LCD
void CmdLCD(u8 cmd)
{
	     IOCLR0 = 1<<RS;     // RS = 0 (command mode)
	     DispLCD(cmd);       // Send command to LCD
	     delay_ms(2);        // Delay to allow command execution
}

// Function to send a single character to LCD
void CharLCD(s8 data)
{
	     IOSET0 = 1<<RS;     // RS = 1 (data mode)
	     DispLCD(data);      // Send character to LCD
	     delay_ms(2);        // Delay for LCD to display character
} 

// Function to display a string on LCD
void StrLCD(s8 *ptr)
{
	while(*ptr!='\0')        // Loop until null character is reached
	{
		 CharLCD(*ptr);        // Display current character
		 ptr++;                // Move to next character
	} 
}

// Function to display an integer number on LCD
void IntLCD(s32 num)
{
	  s8 a[10];              // Array to store digits
	  int i=0;               // Index variable
	  if(num==0)             // Check if number is zero
		{
			CharLCD(0+48);       // Display character '0'
		}
		else{
			if(num<0)            // Check if number is negative
			{
				  CharLCD('-');    // Display minus sign
				  num=-num;        // Convert number to positive
			}
			while(num)           // Extract digits one by one
			{
				  a[i++]=num%10+48; // Convert digit to ASCII and store
				  num=num/10;      // Remove last digit
			}
			--i;                  // Adjust index to last stored digit
			for(;i>=0;i--)        // Print digits in reverse order
			{ 
				   CharLCD(a[i]);   // Display digit on LCD
			}
		}
}

// Function to write custom characters into LCD CGRAM
void WriteToCGRAM(void)
{
	   s8 i;  // Loop variable
     s8 a[] = {0x04,0x0E,0x15,0x04,0x04,0x04,0x04,0x04};   // Custom character pattern 1
		 s8 b[] = {0x04,0x04,0x04,0x04,0x15,0x0E,0x04,0x04};   // Custom character pattern 2
		 CmdLCD(0x40);         // Set CGRAM address to beginning
		 for(i=0;i<8;i++)      // Write first custom character
		    CharLCD(a[i]);
		 for(i=0;i<8;i++)      // Write second custom character
		    CharLCD(b[i]);
}

// Function to display floating-point number on LCD
void FloatLCD(f32 fN,u32 nDP)
{
	  u32 n;                 // Variable to store integer part
	  s32 i;                 // Loop counter
	  if(fN<0.0)             // Check if number is negative
		{
			  CharLCD('-');      // Display minus sign
			  fN = -fN;          // Convert to positive
		}
		n=fN;                  // Extract integer part
		IntLCD(n);             // Display integer part
		CharLCD('.');          // Display decimal point
		for(i=0;i<nDP;i++)     // Display decimal places
		{
			   fN = (fN-n)*10;   // Shift decimal digits
			   n=fN;             // Extract next digit
			   CharLCD(n+48);    // Convert digit to ASCII and display
		}
}
