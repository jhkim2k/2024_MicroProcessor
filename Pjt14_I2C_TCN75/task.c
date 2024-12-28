#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "list.h"
#include "que.h"
#include "uart.h"
#include "prime.h"
#include "timer.h"
#include "task.h"
#include "adc.h"
#include "spi.h"
#include "i2c.h"

struct task_tab {
	char task_name[32];
	void (*task_fun)(char *arg);
};

struct task_tab Task_tab[] = {
	{ "prime", task_prime },
	{ "sum", task_sum },
	{ "tc1047", task_tc1047 },
	{ "tc77", task_tc77 },
	{ "tcn75_i2c", task_tcn75_i2c } 
};

int Task_cnt = sizeof(Task_tab) / sizeof(struct task_tab);

struct task_tab *what_task(char *name)
{
	int i;
	for (i = 0; i < Task_cnt; i++)
		if (!strcmp(name, Task_tab[i].task_name))
			return(&Task_tab[i]);

	return(NULL);
}

struct task 	Task_q[MAX_TASK];
volatile int	Task_f, Task_r;

void task_init()
{
	Task_f = Task_r = 0;
}

int task_insert(struct task *tskp)
{
	if ((Task_r + 1) % MAX_TASK == Task_f)
		return 0;
	Task_r = (Task_r + 1) % MAX_TASK;
	Task_q[Task_r] = *tskp;
	return (1);
}

int task_delete(struct task *tskp)
{
	if (Task_r == Task_f)
		return(0);
	Task_f = (Task_f + 1) % MAX_TASK;
	*tskp = Task_q[Task_f];
	return(1);
}

void task_cmd(char *arg)
{
	char buf[64], *cp0, *cp1, *cp2, *cp3;
	struct task_tab *tp;
	struct task task;
	int ms;

	if (!uart_peek()) return;

	if (gets(buf) == NULL) {
		printf("logout, goodbye !!!\n");
		while(1);
	}

	cp0 = strtok(buf,  " \t\n\r");
	cp1 = strtok(NULL, " \t\n\r");
	cp2 = strtok(NULL, " \t\n\r");
	cp3 = strtok(NULL, " \t\n\r");

	if (cp0 == NULL) {
		printf("$ "); return;
	}

	if (!strcmp(cp0, "timer")) {
		if (!cp1 || !cp2) 
			printf("Unknown command...\n$ ");
		ms = atoi(cp1) / 256;

		if (!(tp = what_task(cp2))) {
			printf("Unknown command...\n$ ");
			return;
		}

		task.fun = tp->task_fun;
		if (cp3) strcpy(task.arg, cp3);
		else	 strcpy(task.arg, "");
		
		cli();
		insert_timer(&task, ms);
		sei();
	} else {
		if (!(tp = what_task(cp0))) {
			printf("Unknown command...\n$ ");
			return;
		}
		if (cp2) {
			(*tp->task_fun)(cp1);
		} else {
			(*tp->task_fun)("");
		}
	}
}

void task_prime(char *ap)
{
	int	t = 2000, n, count = 0;

	if(ap) t = atoi(ap);
	for(n = 2; n <= t; n++) {
		if (is_prime(n)) {
			count++;
			printf( "%d is a prime number !!!\n", n);
		}
	}
	printf("count = %d\n", count);
	printf("$ ");
}

void task_sum(char *ap)
{
	int	t = 100, n, sum = 0;

	if(ap) t = atoi(ap);
	for(n = 1; n <= t; n++) {
		sum += n; 
	}
	printf("sum = %d\n", sum);
	printf("$ ");
}

void task_tc1047(char *arg)
{
	static int	value;
	static int	volatile in_adc = 0;
	if (!strcmp(arg, "")) {
		adc_start(); in_adc = 1; while(in_adc);
		printf("Current temperature is %d degree.\n", value);
	}
	else {
		value = atoi(arg) * (1.1/1023) * 1000;
		value = (value - 500) / 10;
		in_adc = 0;
	}
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

void task_tcn75_i2c(void *arg)
{
	uint16_t value;

	i2c_tcn75_trans_start();
	if (i2c_tcn75_write_one_byte(0x90) != 0)
	{ // address + write_operation
		i2c_tcn75_trans_stop(); printf("task_tcn75_i2c() : SLA+W write fail...\n");
		return;
	}

	if (i2c_tcn75_write_one_byte(0x00) != 0)
	{ // pointer(TEMP)
		i2c_tcn75_trans_stop(); printf("task_tcn75_i2c() : pointer write fail...\n");
		return;
	}

	i2c_tcn75_trans_start(); // Repeat Start
	if (i2c_tcn75_write_one_byte(0x91) != 0)
	{ // address + read_operation
		i2c_tcn75_trans_stop(); printf("task_tcn75_i2c() : SLA+R write fail...\n");
		return;
	} // read TEMP register

	value = ((i2c_tcn75_read_one_byte(0) << 8) | i2c_tcn75_read_one_byte(1)) >> 7;

	i2c_tcn75_trans_stop();

	value = value >> 1; // value = value * 0.5
	printf("task_tcn75_i2c() : current_temperature -> %d degree.\n$ ", value);
}

/*
void task_cmd(char *arg)
{
	char buf[64], *cp0, *cp1, *cp2, *cp3;
	struct task task;
	int ms;

	if (!uart_peek()) return;

	if (gets(buf) == NULL) {
		printf("logout, goodbye !!!\n");
		while(1);
	}

	cp0 = strtok(buf,  " \t\n\r");
	cp1 = strtok(NULL, " \t\n\r");
	cp2 = strtok(NULL, " \t\n\r");
	cp3 = strtok(NULL, " \t\n\r");

	if (cp0 == NULL) {
		printf("$ "); return;
	}

	if (!strcmp(cp0, "prime"))
		task_prime(cp1);
	else if (!strcmp(cp0, "sum")) {
		task_sum(cp1);
	}
	else if (!strcmp(cp0, "timer")) {
		if (cp1 == NULL) {
			printf("!!!-222\n");
			printf("$ ");
			return;
		}
		ms = atoi(cp1) / 256;
		if (!strcmp(cp2, "prime")) {
			task.fun = task_prime;
			if (cp3) strcpy(task.arg, cp3);
			else	 strcpy(task.arg, "");
			cli();
			insert_timer(&task, ms);
			sei();
		} else if (!strcmp(cp2, "sum")) {
			task.fun = task_sum;
			if (cp3) strcpy(task.arg, cp3);
			else	 strcpy(task.arg, "");
			cli();
			insert_timer(&task, ms);
			sei();
		}
		else printf("!!!-333\n$ ");
	}
	else printf("Unknown command...\n$ ");
	
	//printf("$ ");
}
*/
