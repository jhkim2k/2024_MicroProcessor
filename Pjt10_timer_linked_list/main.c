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

int main()
{
	char cmd[128], *cp, *ap;
	//int n = 0;

	uart_init();
	sei();
	while(1) {
		printf("$ ");
		if(fgets(cmd, sizeof(cmd), stdin) == NULL)
			break;
		if((cp = strtok(cmd,  "\n\r\t  ")) == NULL) continue;
		ap = strtok(NULL, "\n\r\t  ");
		if		(!strcmp(cp, "prime"))	app_prime(ap);
		else if (!strcmp(cp, "list"))	app_list(ap);
		else if (!strcmp(cp, "timer"))	app_timer(ap);
		else if (!strcmp(cp, "who"))	printf("I am JHKIM...\n");
		else							printf("Unknown command...\n");
	}
	printf( "logout, goodbye !!!\n" );
	while(1); return(0);
}
