#include <LPC21XX.h>
#include "rtc.h"
#include "rtc_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "types.h"
#include "macros.h"
#include "keypad.h"
#include "keypad_defines.h"

/* Array to store weekday names (SUN to SAT)*/			
s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

/* Variables to store RTC time and date values*/
s32 hour,min,sec,date,month,year,day; 

/* Alarm lock flag (prevents repeated alarm triggering)*/
u8 alarm_lock = 0;

/*Alarm time variables declared in another file*/
extern s32 set_t,set_m,set_s;

int main()
{
	 IODIR1 |= (1<<BUZ)|(1<<LED);      // Configure buzzer and LED pins as OUTPUT
	
	 IODIR0 &= ~(1 << 16)|(1 << 17);   // Configure switches connected to P0.16 and P0.17 as INPUT
	 
	 IODIR1 |= (0xf)<<R0;              // Configure keypad row pins as OUTPUT

   RTC_Init();                       // Initialize RTC module
   InitLCD();                        // Initialize LCD
	
   StrLCD("RTC_CLOCK...");           // Display startup message on LCD
	 delay_ms(500);                    // Time Delay for 500ms
	 CmdLCD(CLEAR_LCD);                // Clear LCD screen
	 HOUR = 10,MIN = 30;               // Set initial RTC time (10:30)
	 DOM = 26,MONTH = 7,YEAR = 2025;   // Set initial RTC date (26/07/2025)
   while(1)
   {
		  GetRTCTimeInfo(&hour,&min,&sec);     // Read current time from RTC
		  DisplayRTCTime(hour,min,sec);        // Display time on LCD
			GetRTCDateInfo(&date,&month,&year);  // Read current date from RTC
			DisplayRTCDate(date,month,year);     // Display date on LCD
			GetRTCDay(&day);                     // Read current day (0–6)
			DisplayRTCDay(day);                  // Display day on LCD
		  
		  if(!(READBIT(IOPIN0,16)))            // Check if menu switch (P0.16) is pressed
			{
				 while(!(READBIT(IOPIN0,16)));     // Wait until switch is released (debounce)
			   menu();                           // Enter menu mode
			}			
			
      if((set_t == HOUR)&&(set_m == MIN)&&(alarm_lock == 0))  // Check if current time matches alarm time
			{
			      match_time();       // Trigger alarm
				    alarm_lock = 1;     // Lock alarm to avoid repeated ringing
			}
			
			if(MIN != set_m)   //Check current minute matches to set minute
			{
				alarm_lock = 0;  // Reset alarm lock when minute changes
			}
   }
}
