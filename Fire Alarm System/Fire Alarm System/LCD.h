/*
 * LCD.h
 *
 * Created: 18/09/2023 11:27:24 PM
 *  Author: ziad
 */ 


#ifndef LCD_H_
#define LCD_H_

// macros for LCd control pins:
#define RS 4
#define RW 2
#define EN 3
// Direction registers
#define LCD_Data_Dir DDRC
#define LCD_Control_Dir DDRD
//port registers
#define LCD_Data_Port PORTC
#define LCD_Control_Port PORTD



void LCD_INIT(void);
void LCD_send_command(unsigned char cmd);
void LCD_send_data(unsigned char data);
void LCD_send_string( char* str);



#endif /* LCD_H_ */