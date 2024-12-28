#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "prime.h"
#include "timer.h"
#include "task.h"
#include "adc.h"
#include "spi.h"

struct task_tab {
	char task_name[32];
	void (*task_fun)(char *arg);
};

struct task_tab Task_tab[] = {
	{ "prime", task_prime },
	{ "sum", task_sum },
	{ "tc1047", task_tc1047 },
	{ "tc77", task_tc77}
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
