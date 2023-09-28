/*
 * UART.c
 *
 * Created: 19/09/2023 05:11:16 PM
 *  Author: ziad
 */ 


#include <avr/io.h>  
#include <util/delay.h>


// macros
#define setbit(reg,bit) reg|=(1<<bit)		// set bit 1
#define clearbit(reg,bit) reg&=~(1<<bit)	// clear bit 0
#define toggelbit(reg,bit) reg^=(1<<bit)	// toggle
#define readbit(reg,bit) ((reg>>bit)&1)		// input read

#include "UART.h"

void UART_INIT(unsigned int baud)
{
	// 8 bit data 1 stop bit
	UCSRB |= (1 << RXEN) | (1 << TXEN);	// Enable UART transmitter and receiver
	UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);	//8 bit data and 1 stop bit
	UBRRL = baud;//lower
	UBRRH = (baud >> 8);//higher
}

void UART_sendchar(unsigned char data)
{   // 1-check empty or not
	// 2-udr data register for send or recive
	// 3-check data send
	while(readbit(UCSRA,UDRE)==0)// wait the reg to be empty
	{		}
	UDR = data; // send  data
	while(readbit(UCSRA,TXC)==0)// wait till the data to be send
	{		}
}

unsigned char UART_receivechar()
{
	while(readbit(UCSRA,RXC)==0) // wait the data to be receive
	{	;	}
	
	return UDR;// data source for variables
}


void UART_sendString(char *str)
{
	int count =0 ;
	while (str[count]  != 0)
	{
		UART_sendchar(str[count]);
		count ++;
	}
	UART_send_byte(str[count]);
}




void  UART_recieve_string(char * ptr)
{
	int i=0;
	ptr[i]=UART_recieve_byte();
	while(ptr[i] !='#')
	{
		i++;
		ptr[i]=UART_recieve_byte();

	}
	ptr[i]='\0';


}
void UART_send_byte( char byte)
{
	while(!(UCSRA &(1<<UDRE))); // while shift register flag != 1 (not empty))
	UDR=byte;

}

char  UART_recieve_byte(void)
{
	while((UCSRA & (1<<RXC)) ==0);

	return UDR ;


}