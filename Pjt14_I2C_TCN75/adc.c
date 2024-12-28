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
