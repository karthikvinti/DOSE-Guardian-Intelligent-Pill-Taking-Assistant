#include <LPC21XX.h>
#include "delay.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "macros.h"
#include "types.h"
#include "keypad_defines.h"
#include "keypad.h"

// These variables are declared in another file and used here	
extern s32 hour,min,sec,date,month,year,day;

// Variables used to store alarm time (hour, minute, second)
s32 set_t,set_m,set_s;

// String used for displaying leading zeros in time/date formatting
char leading_zeros[] = "0000";

// Function to initialize the RTC module
void RTC_Init()
{
   CCR = RTC_RESET;              // Reset the RTC counter and registers

  #ifndef CPU_LPC2148            // If CPU is NOT LPC2148

   PREINT = PREINT_VAL;          // Load integer part of prescaler value
   PREFRAC = PREFRAC_VAL;        // Load fractional part of prescaler value

   CCR = RTC_ENABLE;             // Enable RTC with external clock

   #else                         // If CPU IS LPC2148

   CCR = RTC_ENABLE|RTC_CLKSRC;  // Enable RTC and select internal clock source

   #endif
}

// Function to read current time from RTC
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
	*hour = HOUR;    // Read hour register and store it
	*minute = MIN;   // Read minute register and store it
	*second = SEC;   // Read second register and store it
}

// Function to display RTC time on LCD in HH:MM:SS format
void DisplayRTCTime(u32 hour, u32 minute, u32 second)
{
		CmdLCD(GOTO_LINE1_POS0);     // Move LCD cursor to line 1, position 0
		CharLCD((hour/10)+48);       // Display tens digit of hour
		CharLCD((hour%10)+48);       // Display units digit of hour
		CharLCD(':');                // Display colon separator
		CharLCD((minute/10)+48);     // Display tens digit of minute
		CharLCD((minute%10)+48);     // Display units digit of minute
		CharLCD(':');                // Display colon separator
		CharLCD((second/10)+48);     // Display tens digit of second
		CharLCD((second%10)+48);     // Display units digit of second

}

// Function to read current date from RTC
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
	*date = DOM;       // Read day of month from RTC and store it
	*month = MONTH;    // Read month from RTC and store it
	*year = YEAR;      // Read year from RTC and store it
}

// Function to display date on LCD in DD/MM/YYYY format
void DisplayRTCDate(u32 date, u32 month, u32 year)
{
		CmdLCD(GOTO_LINE2_POS0);    // Move LCD cursor to line 2, position 0
		CharLCD((date/10)+48);      // Display tens digit of date
		CharLCD((date%10)+48);      // Display units digit of date
		CharLCD('/');               // Display slash separator
		CharLCD((month/10)+48);     // Display tens digit of month
		CharLCD((month%10)+48);     // Display units digit of month
		CharLCD('/');               // Display slash separator
		IntLCD(year);               // Display full year using integer LCD function
}

// Function to read current day of week from RTC
void GetRTCDay(s32 *day)
{
	*day = DOW;      // Read day-of-week value (0–6) from RTC
}

// Function to display day of week on LCD
void DisplayRTCDay(u32 dow)
{
	CmdLCD(GOTO_LINE1_POS0+13);    // Move cursor to end of line 1
	StrLCD(week[dow]);             // Display weekday string (SUN, MON, etc...)
}

// Function to display menu and handle user selection using keypad
void menu()
{
	   u8 choice;                    // Variable to store key pressed by user
	   InitLCD();                    // Re-initialize LCD
	   WriteToCGRAM();               // Load custom characters into LCD CGRAM
	   d:CmdLCD(CLEAR_LCD);          // Label 'd' used for re-displaying menu, clear LCD
	   CmdLCD(GOTO_LINE1_POS0);      // Move cursor to line 1, position 0
	   StrLCD("1.E.Time  3.Exit");   // Display menu options on first line
	   CmdLCD(GOTO_LINE2_POS0);      // Move cursor to line 2, position 0
	   StrLCD("2.Set_Med_Time");     // Display second menu option
	
	   while(1)                                       // Infinite loop to wait for user input
		 {
			  choice = get_key();                         // Read key pressed from keypad
			  if(choice != '\0')                          // Check if a valid key is pressed
			  {
				   switch(choice)                           // Select action based on key pressed
					 {
						 case '1' : edit_time();                // Call function to edit time
						            CmdLCD(CLEAR_LCD);          // Clear LCD after editing
						            return;                     // Exit menu function
						 
						 case '2' : set_med_time();             // Call function to set medicine time
                                    CmdLCD(CLEAR_LCD);          // Clear LCD
						            return;                     // Exit menu function
						 
						 case '3' : CmdLCD(CLEAR_LCD);          // Clear LCD
									StrLCD("Time Updated!");    // Display Time Updated message
									delay_ms(500);              // Delay to show message
						            CmdLCD(CLEAR_LCD);	        // Clear LCD
							        return;                     // Exit menu function
						 
						 default : CmdLCD(CLEAR_LCD);           // Clear LCD for error message
								   CmdLCD(GOTO_LINE1_POS0);     // Move cursor to line 1
								   StrLCD("This key");          // Display error text
								   CmdLCD(GOTO_LINE2_POS0);     // Move cursor to line 2
								   StrLCD("is not used");       // Display error tex
						           delay_ms(800);               // Delay so user can read message
						           goto d;                      // Go back to menu display
					 }
				 }
		 } 
}

// Function to display edit menu for time, date, and day
void edit_time()
{
	  s8 choice;                  // Variable to store keypad input
	  k:CmdLCD(CLEAR_LCD);        // Label 'k' used to return to edit menu, clear LCD
	  delay_ms(10);               // Small delay for LCD stability
	  CmdLCD(GOTO_LINE1_POS0);    // Move cursor to line 1, position 0
	  StrLCD("1.Time   2.Date");  // Display options for time and date
	  CmdLCD(GOTO_LINE2_POS0);    // Move cursor to line 2, position 0
	  StrLCD("3.Day    4.Back");  // Display options for day and back
	  
      while(1)              // Infinite loop to wait for user input
	  {	
		   choice = get_key();  // Read key pressed from keypad
	       if(choice != '\0')   // Check if a valid key is pressed
		   { 
	           switch(choice)   // Perform action based on key pressed
			   {
					 case '1' : set_time();     // Call function to set time
					 CmdLCD(CLEAR_LCD);        // Clear LCD after operation
			         return;                   // Exit edit_time function
							 
					 case '2' : set_date();   // Call function to set date
					 CmdLCD(CLEAR_LCD);       // Clear LCD after operation
					 return;                 // Exit edit_time function
							 
                     case '3' : set_day();   // Call function to set day
					 CmdLCD(CLEAR_LCD);      // Clear LCD after operation
					 return;                 // Exit edit_time function
							 
					 case '4' : CmdLCD(CLEAR_LCD);   // Clear LCD
					 StrLCD("Updated!");  // Display Updated message
					 delay_ms(500);       // Delay to show message
					 return;              // Go back to previous menu
							 
					 default : CmdLCD(CLEAR_LCD); // Clear LCD for error message
					 CmdLCD(GOTO_LINE1_POS0);   // Move cursor to first line
					 StrLCD("This key");        // Display error text
					 CmdLCD(GOTO_LINE2_POS0);   // Move cursor to second line
					 StrLCD("is not used");     // Display error text
				     delay_ms(500);             // Delay so user can read message
					 goto k;                    // Go back to edit menu
               }
		 }
	  }
}

// Function to set RTC time using keypad
int set_time(void)
{
	  int h = HOUR,m = MIN,s = SEC,i=1;   // Local copy of hour, minute, second and field selector
	  char key;                           // Variable to store pressed key
	  c:CmdLCD(CLEAR_LCD);                // Label 'c' to return here, clear LCD
	  CmdLCD(GOTO_LINE1_POS0);            // Move cursor to line 1, position 0
	  CharLCD('<');                       // Display left arrow symbol
	  CmdLCD(GOTO_LINE1_POS0+15);         // Move cursor to end of line 1
	  CharLCD('>');                       // Display right arrow symbol
	  CmdLCD(GOTO_LINE2_POS0+3);          // Move cursor to line 2, position 3
	  CharLCD(1);                         // Display Down arrow which is custom character (from CGRAM)
	  CmdLCD(GOTO_LINE2_POS0+12);         // Move cursor to line 2, position 12
	  CharLCD(0);                         // Display Up arrow which is custom character (from CGRAM)
	  while(1)                            // Infinite loop for time editing
	  {
	     CmdLCD(GOTO_LINE1_POS0+4);       // Move cursor to time display position
	     CharLCD((h/10)+48);              // Display tens digit of hour
		 CharLCD((h%10)+48);              // Display units digit of hour
		 CharLCD(':');                    // Display colon separator
	     CharLCD((m/10)+48);              // Display tens digit of minute
	     CharLCD((m%10)+48);              // Display units digit of minute
	     CharLCD(':');                    // Display colon separator
	     CharLCD((s/10)+48);              // Display tens digit of second
	     CharLCD((s%10)+48);              // Display units digit of second
	      
		 if(i==1)                         // If hour field is selected
		 {
		     CmdLCD(GOTO_LINE1_POS0+4);     // Move cursor to hour position
		     StrLCD("  ");	                // Blink hour field
		 }
			 
		 else if(i==2)                    // If minute field is selected
		 {
			  CmdLCD(GOTO_LINE1_POS0+7);    // Move cursor to minute position
		      StrLCD("  ");                 // Blink minute field
		 }
			 
		 else if(i==3)                    // If second field is selected
		 {
			 CmdLCD(GOTO_LINE1_POS0+10);   // Move cursor to second position
		     StrLCD("  ");                 // Blink second field
		 }
       
         key = get_key();              // Read key pressed from keypad
		 if(key != '\0')               // Check if a valid key is pressed
		 {
			 switch(key)                // Perform action based on key
			 {
					case 'D' : if(i == 1)  // Hour selected
							   {
								  if((h>=0)&&(h<=22))  // Hour range within 0 t0 23
								  {
										h++;    // Increase hour
								  }
								  else{
										h=0;   // Reset hour
								  }
								}
								else if(i == 2)   // Minute selected
								{
									if((m>=0)&&(m<=58))  // Minutes range in 0 to 59
								    {
										 m++;   // Increase minute
									}
								 else{
										m = 0;  // Reset minute
									}
								}
								else if(i == 3)   // Seconds selected
								{
									if((s>=0)&&(s<=58))  //Seconds range in 0 to 59
									{
										s++;     // Increase second
									}
									else{
										s = 0;   // Reset second
										}
								 }
								 break;           // Exit switch case
														
							   case 'C' : if(i == 1)       // Hour selected
								{
									if(h==0)
									{
										h=23;       // Wrap hour
									}
									else{
										 h--;      // Decrease hour
									} 
								 }
								 else if(i == 2)  // Minute selected
								 {
										if(m==0)
										{
										   m=59;        // Wrap minute
										}
										else{
											m--;       // Decrease minute
										} 
								  }
								  else if(i == 3)   // Second selected
								  {
										if(s==0)
										{
											s=59;        // Wrap second
										}
									    else{
											s--;       // Decrease second
											} 
								  }
								  break;       // Exit switch case
														
							     case 'A' : if(i == 3)   // Move selection forward
									        { 
												i = 1;   // Go back to hour
											}
											else 
											{
												i++;     // Move to next field
											}
											break;       // Exit switch case
														
			   				     case 'B' : if(i == 1)   // Move selection backward
									        {
												 i = 3;   // Go to second field
											}
											else{
													i--;     // Move to previous field
												}
												break;       // Exit switch case
														
							     case '=' :                // Save updated time
								            HOUR = h;      // Update RTC hour
											MIN = m;       // Update RTC minute
											SEC = s;       // Update RTC second
											CmdLCD(CLEAR_LCD);        // Clear LCD
											StrLCD("Time Updated!");  // Confirmation message
											delay_ms(500);            // Delay to show message
											return 0;                 // Exit function
														
								  default :                           // If invalid key is pressed
									        CmdLCD(CLEAR_LCD);        // Clear LCD
											CmdLCD(GOTO_LINE1_POS0);  // Move cursor to first line
											StrLCD("This key");       // Display error message
											CmdLCD(GOTO_LINE2_POS0);  // Move cursor to second line
											StrLCD("is not used");    // Display error message
											delay_ms(500);            // Delay to show message
											goto c;                   // Return to time edit screen
						 }
					}
		}
}

// Function to set RTC date using keypad
int set_date(void)
{
	  int d = DOM,m = MONTH,y = 2025,i=1;  // Local copies of date, month, year and field selector
	  char key;                            // Variable to store keypad input
	  b:CmdLCD(CLEAR_LCD);                 // Label 'b' to return here, clear LCD
	  CmdLCD(GOTO_LINE1_POS0);             // Move cursor to line 1, position 0	
	  CharLCD('<');                        // Display left arrow
	  CmdLCD(GOTO_LINE1_POS0+15);          // Move cursor to end of line 1
	  CharLCD('>');                        // Display right arrow
	  CmdLCD(GOTO_LINE2_POS0+3);           // Move cursor to line 2, position 3
	  CharLCD(1);                          // Display Down arrow which is custom character (from CGRAM)
	  CmdLCD(GOTO_LINE2_POS0+12);          // Move cursor to line 2, position 12
	  CharLCD(0);                          // Display Up arrow which is custom character (from CGRAM)
	  while(1)                             // Infinite loop for date editing
	  {
	     CmdLCD(GOTO_LINE1_POS0+3	);       // Move cursor to date display position
	     CharLCD((d/10)+48);               // Display tens digit of date
		 CharLCD((d%10)+48);               // Display units digit of date
		 CharLCD('/');                     // Display date separator
		 CharLCD((m/10)+48);               // Display tens digit of month
		 CharLCD((m%10)+48);               // Display units digit of month
		 CharLCD('/');                     // Display date separator
		 add_leading_zeros(count_digits(y));   // Add leading zeros before year
		 IntLCD(y);                        // Display year value
	      
		 if(i==1)                       // If date field is selected
		 {
			 CmdLCD(GOTO_LINE1_POS0+3);   // Move cursor to date field
		     StrLCD("  ");	              // Blink date field
		 }
			 
		 else if(i==2)                  // If month field is selected
		 {
			  CmdLCD(GOTO_LINE1_POS0+6);  // Move cursor to month field
		      StrLCD("  ");               // Blink month field
		 }
			 
		 else if(i==3)                  // If year field is selected
		 {
			  CmdLCD(GOTO_LINE1_POS0+9);  // Move cursor to year field
		      StrLCD("    ");             // Blink year field
		 }
       
          key = get_key();      // Read key from keypad
			    if(key != '\0')       // Check if a valid key is pressed
				{
					 switch(key)        // Perform action based on key
					 {
						 case 'D' :                          // Increment selected field
									if(i == 1)               // Date selected
								    {
										if((d>=1)&&(d<=30))
										{
											 d++;                 // Increase date
										}
										else{
											 d=1;                 // Reset date
										}
									 }
									 else if(i == 2)          // Month selected
									 {
										 if((m>=1)&&(m<=11))
										 {
											 m++;               // Increase month
										 }
										 else{
											 m = 1;             // Reset month
										 }
									  }
									  else if(i == 3)          // Year selected
									  {
											if((y>=0)&&(y<=4095))
											{
												 y++;               // Increase year
											}
											else{
												 y = 0;             // Reset year
											}
									  }
									  break;                  // Exit switch case
														
							   case 'C' :                         // Decrement selected field
									      if(i == 1)              // Date selected
								          {
											 if(d==1)
											 {
												d=31;               // Wrap date
											 }
										     else{
												  d--;              // Decrease date
											  } 
										   }
										   else if(i == 2)          // Month selected
								           {
												if(m==1)              
												{
													m=12;               // Wrap month
												}
												else{
													m--;              // Decrease month
												} 
											}
											else if(i == 3)          // Year selected
								            {
												if(y==0)
												{
													y=4095;             // Wrap year
												}
												else{
													y--;              // Decrease year
												} 
											 }
											 break;                // Exit switch case
														
							   case 'A' :                       // Move selection forward
									      if(i == 3)
									      {
											  i = 1;            // Move back to date
										  }
										  else 
										  {
											   i++;              // Move to next field
										  }
										  break;
														
			   				 case 'B' :                      // Move selection backward
									    if(i == 1)
									    {
											i = 3;           // Move to year
										}
										else{
											i--;             // Move to previous field
										}
										break;              // Exit switch case
														
							   case '=' :                     // Save date into RTC
									      DOM = d;            // Update day of month
										  MONTH = m;          // Update month
										  YEAR = y;           // Update year
										  CmdLCD(CLEAR_LCD);  // Clear LCD
										  StrLCD("Date Updated!");  // Confirmation message
										  delay_ms(500);        // Delay to show message
										  return 0;             // Exit function
														
								 default :                           // Invalid key pressed
									        CmdLCD(CLEAR_LCD);        // Clear LCD
											CmdLCD(GOTO_LINE1_POS0);
											StrLCD("This key");       // Display error message
											CmdLCD(GOTO_LINE2_POS0);
											StrLCD("is not used");    // Display error message
											delay_ms(500);            // Delay for readability
											goto b;                   // Return to date edit screen
						 }
					}
		}
}

// Function to set the day of week using keypad
int set_day(void)
{
	   int day = DOW,i=1;             // Store current RTC day and selection flag
	   char key;                      // Variable to store keypad input
	   s32 count=0;                   // Variable to track selected day (0–6)
	   CmdLCD(CLEAR_LCD);             // Clear LCD screen
	   CmdLCD(GOTO_LINE1_POS0);       // Move cursor to line 1, position 0
	   CharLCD('<');                  // Display left arrow
	   CmdLCD(GOTO_LINE1_POS0+15);    // Move cursor to end of line 1
	   CharLCD('>');                  // Display right arrow
	   CmdLCD(GOTO_LINE2_POS0+3);     // Move cursor to line 2, position 3
	   CharLCD(1);                    // Display custom character
	   CmdLCD(GOTO_LINE2_POS0+12);    // Move cursor to line 2, position 12
	   CharLCD(0);                    // Display custom character
	   while(1)
		 {
			 CmdLCD(GOTO_LINE1_POS0+6);   // Move cursor to day display position
			 StrLCD(week[count]);         // Display day name from array
			 if(i==1)                     // If selection is active
		   {
		      CmdLCD(GOTO_LINE1_POS0+6); // Move cursor to day position
		      StrLCD("   ");            // Blink day field
		   }
			 key = get_key();             // Read key from keypad
			 if(key != '\0')              // Check if key is pressed
			 { 
					switch(key)               // Perform action based on key
					{
						case 'D' :               // Increment day
							       if(count==6)
						           {
										count=0;  // Wrap to Sunday
								   }
								   else{
										 count++;    // Move to next day
									}
								    break;        // Exit switch case
											 
						 case 'C' :               // Decrement day
							        if(count==0)
						            {
										count=6;  // Wrap to Saturday
									}
									else{
										count--;    // Move to previous day
									}
                                    break;	       // Exit switch case
 
                         case '=' :              // Save selected day
							        DOW = count;             // Update RTC day register
								    CmdLCD(CLEAR_LCD);       // Clear LCD
									StrLCD("Day Updated!");  // Show confirmation
									delay_ms(500);           // Delay to display message
								    return 0;		            // Exit function
											 
					}
			 }
			 
		 }
}

// Function to set medicine reminder time
void set_med_time()
{
	  s32 i=1;   // Field selector (hour, minute, second)
	  s8 key;    // Variable to store keypad input
	  set_t = HOUR,set_m = MIN,set_s = SEC;   // Copy current RTC time
	  a:CmdLCD(CLEAR_LCD);                    // Label 'a' to refresh screen, clear LCD
	  CmdLCD(GOTO_LINE1_POS0);                // Move cursor to line 1, position 0
	  CharLCD('<');                           // Display left arrow
	  CmdLCD(GOTO_LINE1_POS0+15);             // Move cursor to end of line 1
	  CharLCD('>');                           // Display right arrow
	  CmdLCD(GOTO_LINE2_POS0+3);              // Move cursor to line 2, position 3
	  CharLCD(1);                             // Display custom character
	  CmdLCD(GOTO_LINE2_POS0+12);             // Move cursor to line 2, position 12
	  CharLCD(0);                             // Display custom character
      while(1)
	  {
	     CmdLCD(GOTO_LINE1_POS0+4);           // Move cursor to time display position
	     CharLCD((set_t/10)+48);              // Display tens digit of hour
		 CharLCD((set_t%10)+48);              // Display units digit of hour
		 CharLCD(':');                        // Display colon
		 CharLCD((set_m/10)+48);              // Display tens digit of minute
		 CharLCD((set_m%10)+48);              // Display units digit of minute
		 CharLCD(':');                        // Display colon
		 CharLCD((set_s/10)+48);              // Display tens digit of second
		 CharLCD((set_s%10)+48);              // Display units digit of second
	       
		 if(i==1)                      // If hour field selected
		 {
			 CmdLCD(GOTO_LINE1_POS0+4);
		     StrLCD("  ");	              // Blink hour field
		 }
			  
		 else if(i==2)                  // If minute field selected
		 {
			  CmdLCD(GOTO_LINE1_POS0+7);
		      StrLCD("  ");               // Blink minute field
		 }
			 
		 else if(i==3)                   // If second field selected
		 {
			 CmdLCD(GOTO_LINE1_POS0+10);
		     StrLCD("  ");                // Blink second field
		 }
			 
		  key = get_key();              // Read key from keypad
		  if(key != '\0')             // Check if valid key is pressed
		  {
				switch(key)              // Perform action based on key
				{
					 case 'D' : if(i == 1)  // If hour field is selected
								{
									if((set_t>=0)&&(set_t<=22))  // Check hour range (0–23)
									{
										set_t++;                   // Increase hour by 1
									}
									else{
										set_t=0;                   // If limit crossed, reset hour to 0
									}
								}
								else if(i == 2)                // If minute field is selected
								{
									 if((set_m>=0)&&(set_m<=58))  // Check minute range (0–59)
									 {
										 set_m++;                  // Increase minute by 1
									 }
								 else{
									   set_m = 0;                // If limit crossed, reset minute to 0
									}
								}
							    else if(i == 3)                // If second field is selected
								{
									if((set_s>=0)&&(set_s<=58)) // Check second range (0–59)
								    {
										 set_s++;                 // Increase second by 1
									}
								    else{
										 set_s = 0;               // If limit crossed, reset second to 0
									}
								 }
								 break;                         // Exit switch case
														
							   case 'C' : if(i == 1)             // If hour field is selected
								          {
											  if(set_t==0)         // If hour is already 0
											  {
													set_t=23;         // Wrap around to 23
											  }
											  else{
												    set_t--;        // Decrease hour by 1
											  } 
										   }
										   else if(i == 2)        // If minute field is selected
								           {
												if(set_m==0)         // If minute is already 0
												{
													set_m=59;         // Wrap around to 59
												}
												else{
												    set_m--;        // Decrease minute by 1
												} 
											}
											else if(i == 3)        // If second field is selected
								            {
												if(set_s==0)         // If second is already 0
												{
													set_s=59;         // Wrap around to 59
												}
												else{
													set_s--;        // Decrease second by 1
												} 
											 }
											 break;                 // Exit switch case
														
							   case 'A' : if(i == 3)            // If currently at second field
									      {
											i = 1;            // Move back to hour field
										  }
										  else 
										  {
											 i++;              // Move to next field
										  }    
										  break;                // Exit switch case
														
			   				  case 'B' : if(i == 1)           // If currently at hour field
									     {
											i = 3;           // Move to second field
										 }
										 else{
											i--;             // Move to previous field
										 }
										 break;
														
							  case '=' : CmdLCD(CLEAR_LCD);           // Clear LCD screen
										 CmdLCD(GOTO_LINE1_POS0+4);   // Move cursor to middle of line 1
										 StrLCD("MEDICINE");          // Display text "MEDICINE"
										 CmdLCD(GOTO_LINE2_POS0+4);   // Move cursor to middle of line 2
										 StrLCD("TIME SET");          // Display confirmation message
										 delay_ms(700);               // Delay so user can read message
										 IOCLR1 = (1<<LED1)|(1<<LED2); // Clearing the two LED's
								         return ;                     // Exit function after saving time
														
							  default : CmdLCD(CLEAR_LCD);            // Clear LCD screen
										CmdLCD(GOTO_LINE1_POS0);      // Move cursor to line 1
										StrLCD("This key");           // Display error message
										CmdLCD(GOTO_LINE2_POS0);      // Move cursor to line 2
										StrLCD("is not used");        // Display error message
									    delay_ms(500);                // Delay for readability
										goto a;                       // Go back to medicine time setting screen
						 }
					}
		 }			 
}

// Function to display required leading zeros before a number on LCD
void add_leading_zeros(int zeros)
{
	 CmdLCD(GOTO_LINE1_POS0+9);      // Move LCD cursor to year display position
	 StrLCD(leading_zeros + zeros);  // Display remaining zeros from the string	
}

// Function to count number of digits in a given number (year)
s32 count_digits(int year_count)
{
	  s32 count = 0;      // Variable to store digit count
	  while(year_count)   // Loop until number becomes zero
	  {
		  count++;          // Increase digit count
		  year_count /= 10; // Remove last digit
	  }
	  return count;       // Return total number of digits
}

// Function that triggers alarm and checks whether medicine is taken
void match_time()
{
    s32 count = 200;     // Counter used to limit waiting time
	u8 flag = 0;         // Flag to indicate whether medicine was taken
    CmdLCD(CLEAR_LCD);   // Clear LCD screen
    CmdLCD(GOTO_LINE1_POS0+4);  // Move cursor to center of first line
    StrLCD("TIME FOR");         // Display alert message
	CmdLCD(GOTO_LINE2_POS0+4);  // Move cursor to center of second line
	StrLCD("MEDICINE");         // Display alert message
    delay_ms(700);              // Delay so user can read message
    IOSET1 = 1<<BUZ;            // Turn ON buzzer to alert user
    while(count)                // Loop until counter becomes zero
    {
       delay_ms(100);           // Wait 100 ms between checks
       if(!(READBIT(IOPIN0,17)))          // Check if medicine confirmation switch is pressed
       {
           while(!(READBIT(IOPIN0,17)));  // Wait until switch is released
           IOCLR1 = 1<<BUZ;      		  // Turn OFF buzzer
		   IOSET1 = 1<<LED2;              // Turn ON Green LED to indicate medicine taken
           CmdLCD(CLEAR_LCD);             // Clear LCD screen
           CmdLCD(GOTO_LINE1_POS0);       // Move cursor to start of first line
           StrLCD("MEDCINE TAKEN");       // Display confirmation message
           delay_ms(1000);                // Delay to show message
		   CmdLCD(CLEAR_LCD);             // Clear LCD after message
		   flag = 1;                      // Set flag to indicate medicine was taken
		   break;                         // Exit the while loop
        }
        count--;                           // Decrease counter value
     }
	   if(flag == 0)                         // If medicine was NOT taken within time
	   {
           IOCLR1 = 1<<BUZ;                   // Turn OFF buzzer
           CmdLCD(CLEAR_LCD);                 // Clear LCD screen
      	   CmdLCD(GOTO_LINE1_POS0);           // Move cursor to first line
           StrLCD("MEDICINE");                // Display warning message
           CmdLCD(GOTO_LINE2_POS0);           // Move cursor to second line
           StrLCD("NOT TAKEN");               // Display warning message
           delay_ms(1000);                    // Delay to show warning
           CmdLCD(CLEAR_LCD);                 // Clear LCD screen

           IOSET1 = 1<<LED1;                   // Turn ON Red LED to indicate missed medicine
		}
}


