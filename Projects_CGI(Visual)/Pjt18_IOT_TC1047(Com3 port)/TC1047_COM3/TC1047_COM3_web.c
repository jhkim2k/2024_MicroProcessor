// MyCGI.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.

#include <stdio.h>
#include "TC1047_COM3_web.h"

void TC1047_COM3_web(char* data)
{
	char* sp, * ep;

	printf("Content-Tupe: text/html; charset=euc-kr\n\n");
	printf("<!DOCTYPE HTML PUBLIC \" -//W3C//DTD HTML 4.0//EN\">\n");
	printf("<html>\n");
	printf("	<head>\n");
	printf("		<title> TC1047_COM3.exe(IOT) </title>\n");
	printf("	</head>\n");
	printf("	<body>\n");
	printf("		<H2> IOT: TC1047_COM3(using a fixed serial port) </H2>\n");
	printf("		<HR>\n");
	for (sp = ep = data; *ep; ep++) {
		if (*ep == '\n') {
			*ep = '\0';
			printf("%s\n<BR>", sp);
			sp = ep + 1;
		}
	}
	printf_s("%s\n<BR>\n", sp);
	printf("	</body>\n");
	printf("</html>\n");
}