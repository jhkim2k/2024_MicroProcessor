#include <stdio.h>
#include <stdlib.h>
#include "prime.h"

int is_prime(int n) {
	int i; 
	for (i = 2; i <= n/2; i++)
		if((n % i) == 0)
			return (0);
	return (1);
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
