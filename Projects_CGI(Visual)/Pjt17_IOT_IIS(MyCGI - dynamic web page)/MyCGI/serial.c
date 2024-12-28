#include <stdio.h>
#include "MyCGI_web.h"

int main() {
	char data[64];

	sprintf_s(data, 64, "Current temperature is 10 degree.");
	MyCGI_web(data);

	return 0;
}