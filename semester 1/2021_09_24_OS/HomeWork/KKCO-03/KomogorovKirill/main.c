#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	mtrace();
	void *ptr1 = new(0x2a);
	void *ptr2 = new(0xee);
	void *ptr3 = new(0xff);
	return 0;
}
