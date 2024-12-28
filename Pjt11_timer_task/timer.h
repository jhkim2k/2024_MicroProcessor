#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct task {
	void	(*fun)(char *);
	char 	arg[8];
};

struct timer {
	int		time;
	struct 	task	task;
	struct	timer	*link;
};

extern struct timer *Thead;
struct timer *get_timer();
void insert_timer(struct task *tskp, int ms);

void tour_timer();
void free_timer();

struct timer *get_timer();

void app_timer(char *ap);

void timer_init();
void timer_expire(void);
