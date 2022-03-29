#include "I2C_LCD1602.h"
 
uint8_t I2C_Lcd_Command(uint8_t lcd_addr, uint8_t data, LCD_Flags eflags )
// static uint8_t I2C_Lcd_Command(uint8_t lcd_addr, uint8_t data, uint8_t flags )
{
	HAL_StatusTypeDef res;
//	while ( !(HAL_I2C_IsDeviceReady(&hi2c1,lcd_addr,1,HAL_MAX_DELAY) == HAL_OK ));

	uint8_t high_byte=data & 0xF0;
	uint8_t low_byte=(data<<4) & 0xF0;
	uint8_t data_arr[4];

	data_arr[0]=high_byte | eflags | lcd_BACKLIGHT | lcd_EN_Pin;
	data_arr[1]=high_byte | eflags | lcd_BACKLIGHT;
	data_arr[2]=low_byte  | eflags | lcd_BACKLIGHT | lcd_EN_Pin;
	data_arr[3]=low_byte  | eflags | lcd_BACKLIGHT ;

	res= HAL_I2C_Master_Transmit(&hi2c1,lcd_addr, data_arr,  sizeof(data_arr), 100);
	// res= HAL_I2C_Master_Transmit(&hi2c1,lcd_addr, data_arr,  sizeof(data_arr), HAL_MAX_DELAY);

     HAL_Delay(1); // 5ms for lcd
    //  HAL_Delay(5); // 5ms for lcd

	return res;
}

void I2C_Lcd_SendCmd(uint8_t lcd_addr, uint8_t cmd)
{
	I2C_Lcd_Command(lcd_addr, cmd, lcd_NULL);
};

void I2C_Lcd_SendData(uint8_t lcd_addr, uint8_t data)
{
	I2C_Lcd_Command(lcd_addr, data, lcd_RS_Pin);
}


void I2C_Lcd_Init(uint8_t lcd_addr)
{
	   // 4-bit mode, 2 lines, 5x7 format
	I2C_Lcd_SendCmd(lcd_addr, 0b00110000);
	    // display & cursor home (keep this!)
	I2C_Lcd_SendCmd(lcd_addr, 0b00000010);
	    // display on, right shift, underline off, blink off
	I2C_Lcd_SendCmd(lcd_addr, 0b00001100);
	    // clear display (optional here)
//	I2C_Lcd_SendCmd(lcd_addr, 0b00000001);
	I2C_Lcd_SendCmd(lcd_addr, 0b0000001);
}

void I2C_Lcd_ClearDisplay(uint8_t addr)
{
	I2C_Lcd_SendCmd(addr, LCD_ClearDisplay);
};




/**
  * @brief  Set cursor possition
  * @param  addr: i2c adrress
  * @param  col: LCD colum
  * @param  line: LCD lcd line
  * @retval None
  */
void I2C_Lcd_SetCursor(uint8_t addr, uint8_t col, uint8_t line)
{
	I2C_Lcd_SendCmd(addr, 0x80 | (col+((line)? 0x40:0x00)));

};

void I2C_Lcd_PrintChar(uint8_t addr, char ch)
{
	I2C_Lcd_SendData(addr,ch);
}

void I2C_Lcd_PrintString(uint8_t addr, char *ch)
{
	while(*ch)
	{
		I2C_Lcd_SendData(addr,*ch);
		ch++;
	}
}

void I2C_Lcd_PrintNumber(uint8_t addr, uint32_t number, numBase nBase)
{
    char buff[8*sizeof(long)+1];
	char *str=&buff[sizeof(buff)-1];
	*str='\0';
	if (nBase<2) 
        nBase=10;
	do
	{
		char c=number%nBase;
		number/=nBase;
		str--;
		*str=(c<10)?c+'0':c+'A'-10;
	} while (number);

    I2C_Lcd_PrintString(addr, str);
}

void I2C_Lcd_PrintFloat(uint8_t addr, double fValue, uint8_t digits)
{
  //size_t n=0;
	if(fValue >4294967040.0 || fValue<-4294967040.0) 
        return;
	
	if(fValue<0.0)
	{
		// this->write('-');
        I2C_Lcd_PrintChar(addr,'-');
		fValue=-fValue;
	}
	
	double rouding=0.5;
	for(uint8_t i=0; i<digits; ++i)
		rouding/=10.0;
	fValue+=rouding;
	
	uint32_t int_part=(unsigned int)fValue;
	double remainder=fValue-(double)int_part;
	
	// this->printNumber((unsigned long)int_part,DEC);
    I2C_Lcd_PrintNumber(addr, int_part, DEC);
	
	if(digits > 0)
		
    {   // this->write('.');
        I2C_Lcd_PrintChar(addr,'.');
    }
	while(digits-- >0)
	{
		remainder*=10.0;
		unsigned int toPrint=(unsigned int)(remainder);
		I2C_Lcd_PrintNumber(addr, toPrint, DEC);
		remainder-=toPrint;
	}	  
}




