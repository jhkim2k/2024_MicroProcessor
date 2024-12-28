#define EOT 0x04

void uart_init(void);
int uart_putchar(char ch, FILE *stream), uart_getchar(FILE *stream);
void uart_echo(char ch);

int uart_peek();
