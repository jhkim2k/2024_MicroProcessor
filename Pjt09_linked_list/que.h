#define QI_SIZE 64
#define QO_SIZE 64

extern char qi[QI_SIZE], qo[QO_SIZE];
//extern char q[];
extern int  fi, ri, fo, ro;

void q_init();
int qi_insert(char ch);
int qi_delete();
int qo_insert(char ch);
int qo_delete();
