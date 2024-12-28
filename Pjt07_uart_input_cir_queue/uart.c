#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "que.h"

int uart_putchar(char ch, FILE *stream);

FILE Mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
char uart_busy;

void uart_init()
{
	stdout = &Mystdout;
	uart_busy = 0;
	q_init();
	UBRR0H = 0x00; UBRR0L = 0x07;
	sbi(UCSR0A, U2X0);
	sbi(UCSR0B, TXEN0);
	sbi(UCSR0B, TXCIE0);
	sbi(UCSR0B, RXEN0);
	sbi(UCSR0B, RXCIE0);
}

int uart_putchar(char ch, FILE *stream)
{
	if (ch == '\n')
		uart_putchar('\r', stream);
	
	cli();

	if(!uart_busy) {
		UDR0 = ch;
		uart_busy = 1;
	}
	else {
		while(qo_insert(ch) == 0) {
			sei();
			_delay_us(100);
			cli();
		}			
	}
	sei();

	return(1);
}

ISR(USART0_TX_vect)
{
	char ch;

	if((ch = qo_delete()) == 0)
		uart_busy = 0;
	else
		UDR0 = ch;
}

ISR(USART0_RX_vect)
{
	char ch;

	ch = UDR0;
	qi_insert(ch);
}
