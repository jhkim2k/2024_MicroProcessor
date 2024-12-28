// main.c
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "que.h"
#include "prime.h"

int main()
{
	uart_init();
	
	sei();

	app_prime(2000);

	while(1);

	return 0;
}
