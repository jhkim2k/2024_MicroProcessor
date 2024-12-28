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
#include "task.h"
#include "spi.h"
#include "adc.h"
#include "i2c.h"



void spi_init()
{
	sbi(DDRB, 0); // sbi(DDRB, nSS,nCS); // output
	sbi(DDRB, 1); // sbi(DDRB, SCK); // output
	sbi(DDRB, 2); // sbi(DDRB, MOSI); // output
	cbi(DDRB, 3); // cbi(DDRB, MISO); // input
	sbi(SPSR, SPI2X); // double speed
	SPCR = ( (1 << MSTR) | (1 << SPE) | (1 << SPIE) ); // MASTER, MSB first, Fosc/4
}

void spi_select() { cbi(PORTB, 0); } // cbi(PORTB, nSS)
void spi_release() { sbi(PORTB, 0); } // sbi(PORTB, nSS)
void spi_write(uint8_t data) { SPDR = data; }

ISR(SPI_STC_vect)
{
	struct task task;

	uint8_t data;
	data = SPDR;

	task.fun = task_tc77;
	sprintf(task.arg, "%d", data);
	task_insert(&task);
}
