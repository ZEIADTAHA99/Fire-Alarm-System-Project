/*
 * UART.h
 *
 * Created: 19/09/2023 05:10:33 PM
 *  Author: ziad
 */ 


#ifndef UART_H_
#define UART_H_


// UART headers
void UART_INIT(unsigned int baud);
void UART_sendchar(unsigned char data);
unsigned char UART_receivechar();
void UART_sendString(char *str);

void UART_send_byte( char byte);
char  UART_recieve_byte(void);
void  UART_recieve_string(char * ptr);

#endif /* UART_H_ */