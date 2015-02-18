#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <fcntl.h>
#include <linux/mman.h>


#define rdtscll(val) __asm__ __volatile__ ("rdtsc" : "=A" (val))



int main(int argc, char *argv[])
{
	long long tick0,tick1;
	long difference;
	tick0 = 0;
	tick1 = 0;
	
	//Calculate time
	rdtscll(tick0);
	malloc(16);
	rdtscll(tick1);

	//Print the difference
	difference = (tick1-tick0);
	printf("Malloc at 16 bytes %d\n", difference);


	tick0 = 0;
	tick1 = 0;
	
	//Calculate time
	rdtscll(tick0);
	malloc(1024);
	rdtscll(tick1);

	//Print the difference
	difference = (tick1-tick0);
	printf("Malloc at 1024 bytes %d\n", difference);



	tick0 = 0;
	tick1 = 0;
	
	//Calculate time
	rdtscll(tick0);
	malloc(16000);
	rdtscll(tick1);

	//Print the difference
	difference = (tick1-tick0);
	printf("Malloc at 16000 bytes %d\n", difference);

	return 0;
}
