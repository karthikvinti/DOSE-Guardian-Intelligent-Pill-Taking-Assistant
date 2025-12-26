#include "types.h"

void InitLCD(void);
void CmdLCD(u8);
void CharLCD(s8);
void StrLCD(s8 *);
void IntLCD(s32);
void WriteToCGRAM(void);
void FloatLCD(f32,u32);
void charge(void);
