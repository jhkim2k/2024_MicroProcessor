#define MAX_TASK 16

void task_init();
void task_cmd(char *arg);
int task_insert(struct task *tskp);
int task_delete(struct task *tskp);
