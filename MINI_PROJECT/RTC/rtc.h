#include "types.h"

void RTC_Init(void);

//#define CPU_LPC2148

extern char week[][4];

void GetRTCTimeInfo(s32 *, s32 *, s32 *);
void DisplayRTCTime(u32,u32,u32);
void GetRTCDateInfo(s32 *,s32 *,s32 *);
void DisplayRTCDate(u32,u32,u32);
void GetRTCDay(s32 *);
void DisplayRTCDay(u32);

void menu(void);
void edit_time(void);
int set_time(void);
int set_date(void);
int set_day(void);
void set_med_time(void);

void match_time(void);

s32 count_digits(int year_count);
void add_leading_zeros(int zeros);
