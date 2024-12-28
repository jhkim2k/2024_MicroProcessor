// main.c
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

int main()
{
	int i = 0;
	// int n = 8;
	led_init();
	led_off_all();

	while(1) {
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
		for(i = 7; i >= 0; i--) {
			led_on(i);
			_delay_ms(300);
			led_off(i);
		}
	}

	return 0;
}
