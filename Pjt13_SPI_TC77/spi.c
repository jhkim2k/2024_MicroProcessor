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
#include "spi.h"

#define TC77_WAIT_HI 1
#define TC77_WAIT_LO 2

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

void task_tc77(void *arg)
{
	static uint8_t state;
	static uint16_t value;

	if (!strcmp(arg, ""))
	{
		state = TC77_WAIT_HI;
		spi_select();
		spi_write(0x00);
	}
	else
	{
		switch(state)
		{
			case TC77_WAIT_HI :
				value = atoi(arg) << 8; state = TC77_WAIT_LO; spi_write(0x00);
				break;
			case TC77_WAIT_LO:
				value |= atoi(arg); value = (value >> 3) * 0.0625;
				spi_release();
				printf("task_tc77() : current_temperature -> %d degree.\n$ ", value);
			break;
			default:
				spi_release();
				printf("task_tc77() : unexpecetd state in task_tc77...\n$ ");
		}
	}
}
