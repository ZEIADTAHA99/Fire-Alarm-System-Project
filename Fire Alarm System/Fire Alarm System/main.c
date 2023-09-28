/*
 * Fire Alarm System.c
 *
 * Created: 18/09/2023 11:26:18 PM
 * Author : ziad
 */ 

#include <avr/io.h>
#define F_CPU 16000000ul
#include <stdio.h>
#include <util/delay.h>


// macros
#define setbit(reg,bit) reg|=(1<<bit)		// set bit 1
#define clearbit(reg,bit) reg&=~(1<<bit)	// clear bit 0
#define toggelbit(reg,bit) reg^=(1<<bit)	// toggle
#define readbit(reg,bit) ((reg>>bit)&1)		// input read


#include "LCD.h"
#include "UART.h"



int main(void)
{
	 
	 clearbit(DDRB,0); // Input Direction for Flame Sensor Pin (Port B,0)
	 
	 UART_INIT(103);  // init baud = 9600
	 LCD_INIT();
	 
	 LCD_send_command(0X01);	
	 
    while (1) 
    {
		
		// Use LCD in 2 line in 8-bit Mode:
		LCD_send_command(0x38);
		
		int Flame_Sensor = readbit(PINB,0) ;
		
		if (Flame_Sensor == 1)
		{
			
			UART_sendchar('f'); // Send Notification to the Second Bluetooth (Second Micro-controller, To control DC Motor and Set the Alarm LED)
			
			LCD_send_string (" Notification:");// displaying name
			LCD_send_command(0x80 + 0x40 + 0);// shifting cursor  to 1st  shell  of second line
			_delay_ms(10);
			LCD_send_string("Fire");
			LCD_send_string(" ");
			_delay_ms(60);
			LCD_send_command(0x01);
			
			
		}
		
		else if (Flame_Sensor == 0)
		{
			UART_sendchar('n'); //Send Notification to the Second Bluetooth (Second Micro-controller, That All Save , Stop Motor and LED the Green LED)
			
			LCD_send_string (" Notification:");// displaying name
			LCD_send_command(0x80 + 0x40 + 0);// shifting cursor  to 1st  shell  of second line
			_delay_ms(10);
			LCD_send_string("All Save");
			LCD_send_string(" ");
			_delay_ms(60);
			LCD_send_command(0x01);
			
			
		}
		
		
    }
}

