void uart_init(void);
void uart_putstart(char str[]);

extern char buf[64];
extern int  volatile bufi, txend;
