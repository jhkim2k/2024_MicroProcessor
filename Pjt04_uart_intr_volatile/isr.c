#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

ISR(USART0_TX_vect)
{
	char ch = buf[bufi];
	if(!ch) {
		txend = 1; buf[0] = 0; return;
	}
	if(ch == '\n')  buf[bufi] = '\n';
	else			bufi++;
	UDR0 = ch;
}
