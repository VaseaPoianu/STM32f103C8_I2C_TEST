/*
 * A0,A1,A2=0  , pcf addr=(0x20<<1)
 * A0,A1,A2=1  , pcf addr=(0x27<<1)
 *
 * 			LCD pin | RS | RW | CS | DB4 | DB5 | DB6 | DB7 | LED_A |
 * 			--------------------------------------------------------
 *			PCF pin | P0 | P1 | P2 | P4  | P5  | P6  | P7  |   P3  |
 */

#ifndef __I2C_LCD1602_H
#define __I2C_LCD1602_H

#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1;

typedef unsigned char uint8_t;
typedef unsigned long uint32_t;

typedef enum NumberBase
{
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16
} numBase;

#define RS_Pin (1 << 0)
#define EN_Pin (1 << 2)
#define BACKLIGHT (1 << 3)
#define LCD_ADDR (0x27 << 1)

// LCD module information
#define lcd_LineOne 0x00 // start of line 1
#define lcd_LineTwo 0x40 // start of line 2

// LCD instructions
#define lcd_Clear 0x01                 // replace all characters with ASCII 'space'
#define lcd_Home 0x02                  // return cursor to first position on first line
#define lcd_EntryMode 0x06             // shift cursor from left to right on read/write
#define lcd_DisplayOff 0b00001000      // turn display off
#define lcd_DisplayOn 0b00001100       // display on, cursor off, don't blink character
#define lcd_FunctionReset 0b00110000   // reset the LCD
#define lcd_FunctionSet4bit 0b00101000 // 4-bit data, 2-line display, 5 x 7 font

#define lcd_SetCursor 0x80 // set cursor position

// #define BIN 2
// #define OCT 8
// #define DEC 10
// #define HEX 16

// commands
#define LCD_ClearDisplay 0x01
#define LCD_ReturnHome 0x02
#define LCD_EntryModeSet 0x04
#define LCD_DisplayControl 0x08
#define LCD_CursorShift 0x10
#define LCD_FunctionSet 0x20
#define LCD_SetCGrAddr 0x40
#define LCD_SetDDRaddr 0x80

// flags for display entry mode
#define LCD_EntryRight 0x00
#define LCD_EntryLeft 0x02
#define LCD_EntryShiftIncrement 0x01
#define LCD_EntryShiftDecrement 0x00

// flags for display on/off control
#define LCD_DisplayON 0x04
#define LCD_DisplayOFF 0x00
#define LCD_CursorON 0x02
#define LCD_CursorOFF 0x00
#define LCD_BlinkON 0x01
#define LCD_BlinkOFF 0x00

// flags for display/cursor shift
#define LCD_DisplayMove 0x08
#define LCD_CursorMove 0x00
#define LCD_MoveRight 0x04
#define LCD_MoveLeft 0x00

// flags for function set
// #define LCD_8BitMode 0x10
// #define LCD_4BitMode 0x00
// #define LCD_2Line 0x08
// #define LCD_1Line 0x00
// #define LCD_5x10Dots 0x04
// #define LCD_5x8Dots 0x00

typedef enum lcdFlags
{   
    lcd_NULL=       0x00,
    lcd_RS_Pin=     0x01,
    lcd_EN_Pin=     (1 << 2),

    lcd_LCD_ADDR=   (0x27 << 1),
    lcd_BACKLIGHT=  (1 << 3),

    // flags for function set
    lcd_8BitMode =  0x10,
    lcd_4BitMode =  0x00,
    lcd_2Line =     0x08,
    lcd_1Line =     0x00,
    lcd_5x10Dots =  0x04,
    lcd_5x8Dots =   0x00,

    /*flags for display/cursor shift */
    lcd_LCD_DisplayMove=    0x08,
    lcd_LCD_CursorMove=     0x00,
    lcd_LCD_MoveRight=      0x04,
    lcd_LCD_MoveLeft=       0x00,

    // flags for display/cursor shift
lcd_DisplayMove= 0x08,
lcd_CursorMove =0x00,
lcd_MoveRight =0x04,
lcd_MoveLeft =0x00

} LCD_Flags;

// flags for backlight control
// #define LCD_BackLight 0x08
// #define LCD_NoBackLight 0x00

//  #define En 0B00000100  // Enable bit
//  #define Rw 0B00000010  // Read/Write bit
//  #define Rs 0B00000001  // Register select bit

typedef struct
{
    unsigned char u8Addr;
    unsigned char u8Data;

} LCD_1602;

 uint8_t I2C_Lcd_Command(uint8_t lcd_addr, uint8_t data, uint8_t flags);
 void I2C_Lcd_SendCmd(uint8_t lcd_addr, uint8_t cmd);
 void I2C_Lcd_SendData(uint8_t lcd_addr, uint8_t data);
void I2C_Lcd_Init(uint8_t lcd_addr);

//void I2C_Lcd
void I2C_Lcd_ClearDisplay(uint8_t addr);
void I2C_Lcd_SetCursor(uint8_t addr, uint8_t col, uint8_t line);
void I2C_Lcd_PrintNumber(uint8_t addr, uint32_t number, numBase nBase);
void I2C_Lcd_PrintInteger(uint8_t addr, int iValue);
void I2C_Lcd_PrintFloat(uint8_t addr, double fValue, uint8_t digits);

void I2C_Lcd_PrintChar(uint8_t addr, char ch);
void I2C_Lcd_PrintString(uint8_t addr, char *ch);

#endif /*-*-*/
