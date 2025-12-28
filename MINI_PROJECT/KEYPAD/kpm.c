#include <LPC21XX.h>
#include "keypad.h"
#include "delay.h"
#include "types.h"
#include "macros.h" 
#include "keypad_defines.h"

/*
 * Function Name : get_key
 * Description   : Scans a 4x4 matrix keypad and returns the pressed key.
 * Return Value  : 
 *      - ASCII value of the pressed key
 *      - '\0' if no key is pressed
*/


s8 get_key()                                      // Returns a char for the pressed key, or '\0' if none
{
        s32 row,col;                              // Returns a char for the pressed key, or '\0' if none
	
        u8 key[4][4] ={                           // Key mapping for Proteus: rows x cols -> character
						 {'1', '2', '3', 'A'},      // Row 0: 1 2 3 A
						 {'4', '5', '6', 'B'},      // Row 1: 4 5 6 B
						 {'7', '8', '9', 'C'},      // Row 2: 7 8 9 C
						 {'*', '0', '=', 'D'}       // Row 3: * 0 = D
					  };
				
        for(row = 0; row < ROWS; row++)           // Scan each row one by one
		{
             WRITENIBBLE(IOSET1,R0,0xF);          // Drive all row lines high (set R0..R3 = 1)
             CLRBIT(IOCLR1,(R0 + row));           // Pull the current row low (activate this row)
             for(col = 0; col < COLS; col++)      // Scan all columns for this active row 	 	
			 {
                  if(!(IOPIN1 & (1 << (C0 + col))))               // Check if column line is pulled low (key press detected)
				  {
                         delay_us(50);                            // Debounce: short delay to filter bouncing
                         if(!(IOPIN1 & (1 << (C0 + col))))        // Confirm key is still pressed after debounce
						 {
                           while(!(IOPIN1 & (1 << (C0 + col))));  // Wait here until key is released (blocking)
                           WRITENIBBLE(IOSET1,R0,0xF);            // Restore rows to high before exiting
                           return key[row][col];                  // Return the mapped character for (row, col)
                         }
                   }
              }
        }
        return '\0';                               //If no key is pressed, return NULL character
}

