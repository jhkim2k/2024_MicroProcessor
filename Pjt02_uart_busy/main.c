// main.c
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"

int main()
{
	char c;
	int i;
	//led_init();
	//led_off_all();
	uart_init();

	for(i=0; i<20; i++) {
		for(c = 'A'; c <= 'Z'; c++)
			uart_putchar(c);
		uart_putchar( '\n' );
	}
	uart_putstr("The end");
	return 0;
}
