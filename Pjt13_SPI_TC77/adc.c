#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"
#include "que.h"
#include "prime.h"
#include "list.h"
#include "timer.h"
#include "adc.h"

void adc_init()
{
	cbi(DDRF, PF0);
	ADMUX = 0x80 | 0x00;
}

void adc_start()
{
	sbi(ADCSRA, ADEN);
	_delay_us(120);
	sbi(ADCSRA, ADIE);
	sbi(ADCSRA, ADSC);
}

ISR(ADC_vect)
{
	uint16_t	data;
	char		arg[8];

	data = ADCW & 0x03ff;
	cbi(ADCSRA, ADEN);

	sprintf(arg, "%d", data);

	task_tc1047(arg);
}

void task_tc1047(char *arg)
{
	static int	value;
	static int	volatile in_adc = 0;
	if (!strcmp(arg, "")) {
		adc_start(); in_adc = 1; while(in_adc);
		printf("Current temperature is %d degree.\n$ ", value);
	}
	else {
		value = atoi(arg) * (1.1/1023) * 1000;
		value = (value - 500) / 10;
		in_adc = 0;
	}
}
