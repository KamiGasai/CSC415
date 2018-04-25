#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MYNAME "Peitong"

int main() {
	char str[100] = "Hello CSC415, I am ";
	sprintf(str+strlen(str), MYNAME);
								//strcpy(str, "Hello CSC415, I am ");
								//size = sprintf(str+strlen(str), MYNAME);
	write(1, str, strlen(str));
	puts("");
	return 0;

}