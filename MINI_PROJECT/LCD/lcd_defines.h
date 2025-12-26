#define LCD_data 0        // Starting bit position of LCD data pins (D0–D7)
#define RS 8              // Register Select pin connected to P0.8
#define RW 10             // Read/Write control pin connected to P0.10
#define EN 9              // Enable pin connected to P0.9
#define SW 12             // Switch input pin connected to P0.12

#define GOTO_LINE1_POS0 0x80 // LCD command to move cursor to line 1, position 0
#define GOTO_LINE2_POS0 0xC0 // LCD command to move cursor to line 2, position 0
#define CLEAR_LCD 0x01       // LCD command to clear the display screen
