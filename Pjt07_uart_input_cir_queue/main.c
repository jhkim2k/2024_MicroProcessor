// main.c
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "que.h"
#include "prime.h"
#include "led.h"

int main()
{
	char cmd[128], ch;
	int n = 0;
	int i;

	uart_init();
	led_init();
	led_off_all();
	
	sei();
	printf("$ ");
	while(1) {
		cli();
		ch = qi_delete();
		sei();
		if(ch) {
			if(ch == '\r') continue;
			if(ch == '\n') {
				cmd[n] = 0;
				if(!strcmp(cmd, "app"))	app_prime(2000);
				else if(!strcmp(cmd, "who"))	printf("I am JHKIM...\n");
				else if(!strcmp(cmd, "led")) {
						for(i = 0; i < 8; i++) {
							if(i == 7) {
								led_on(i);
								_delay_ms(300);
							} else {
								led_on(i);
								_delay_ms(300);
								led_off(i);
							}
						}
				}
				else					printf("Unknown command...\n");
				n = 0;
				printf("$ ");
			}
			else
				cmd[n++] = ch;
		}
	}
	return 0;
}
