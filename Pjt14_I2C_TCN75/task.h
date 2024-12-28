#define MAX_TASK 16

void task_init();
void task_cmd(char *arg);
int task_insert(struct task *tskp);
int task_delete(struct task *tskp);
void task_prime(char *ap);
void task_sum(char *ap);
void task_tc1047(char *arg);
void task_tc77(void *arg);
void task_tcn75_i2c(void *arg);
