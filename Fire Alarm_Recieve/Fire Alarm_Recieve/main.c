/*
 * Fire Alarm_Recieve.c
 *
 * Created: 21/09/2023 09:49:43 PM
 * Author : ziad
 */ 


#define F_CPU 16000000ul
#include <avr/io.h>
#include <util/delay.h>


void UART_INIT(unsigned int baud);
void UART_sendchar(unsigned char data);
unsigned char UART_receivechar();
void UART_sendString(char *str);

void UART_send_byte( char byte);
char  UART_recieve_byte(void);
void  UART_recieve_string(char * ptr);

void LCD_send_data_4bit(unsigned char data);
void LCD_send_command_4bit(unsigned char cmd);
void LCD_INIT_4bit(void);
void LCD_send_string_4bit( char* str);

void motor(unsigned char d,unsigned char r);

#define setbit(reg,bit) reg|=(1<<bit)
#define clearbit(reg,bit) reg&=~(1<<bit)
#define toggelbit(reg,bit) reg^=(1<<bit)
#define readbit(reg,bit) ((reg>>bit)&1)





int main(void)
{
	UART_INIT(103);
	unsigned char x;
	
	
	setbit(DDRC,6); // Alarm RED LED
	setbit(DDRC,5); // Green LED
	
	while (1)
	{
	    x = UART_receivechar();
		//UART_sendchar(x);

	
		if (x == 'f')
		{
			setbit(PORTC,6);  // Alarm RED LED (On)
			clearbit(PORTC,5);  // Green LED   (off)
			
			motor(100,1);  // Let Motor Turn Right (1: IN1) With Full Speed (Indication of DC motor Connected with Window, T0 open it in Alarm Mode)
			_delay_ms(500);
			motor(0,0);    // Motor Stop
			_delay_ms(500);
			
		}
		
		else if (x == 'n')
		{
			
			setbit(PORTC,5);  // Green LED
			clearbit(PORTC,6);  // Alarm RED LED
			/*
			motor(100,0);     // Let Motor Turn left (0: IN2) With 50% of its Full Speed (Indication of DC motor Connected with Window, To Close it in Normal Mode)
			_delay_ms(500); 
			motor(0,0);     // Motor Stop
			_delay_ms(500);
			*/

		}
		
		
	}
}

void motor(unsigned char d,unsigned char r) // d for Duty Cycle (Speed) and r for direction
{
	
	//Set Direction of PORT A0 and PORT A1 to Output.    // Another way : [ DDRA  |= (1<<PA0 ) | (1<<PA1); ]
	setbit(DDRA,0);
    setbit(DDRA,1);

	if (r>=1)
		setbit(PORTA,0); // PA0 IN1 high [ PORTA = 1<<PA0; ] 
	else
		setbit(PORTA,1); // PA1 IN2 high [ PORTA = 1<<PA1; ]
	
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00) ; // PWM Register : for Enable the FASTPWM mode , To Enable Non-Inverted Mode , To set no Prescale
	DDRB|=(1<<PB3);// PIN OC0 output Enable1 pwm
	OCR0=((d*255)/100);
	
}

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