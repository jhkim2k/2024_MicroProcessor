// main.c
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "que.h"
#include "prime.h"
#include "list.h"
#include "timer.h"
#include "task.h"
#include "adc.h"

int main()
{
	int 	tag;
	struct	task task;

	sei();
	uart_init();
	task_init();
	timer_init();	
	adc_init();

	printf("$ ");
		
	while(1) {
		cli();
		tag = task_delete(&task);
		sei();
		if (tag)
			(*(task.fun))(task.arg);
	}
	return (0);
}

