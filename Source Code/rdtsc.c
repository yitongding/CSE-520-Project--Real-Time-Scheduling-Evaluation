#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <unistd.h>

#define BCM2708_ST_BASE 0x20003000 /* BCM 2835 System Timer */


volatile unsigned *TIMER_registers;
volatile long long int *timer;

unsigned int TIMER_GetSysTick()
{
    return TIMER_registers[1];
}

void TIMER_Init()
{
    /* open /dev/mem */

    int TIMER_memFd;

    if ((TIMER_memFd = open("/dev/mem", O_RDWR/*|O_SYNC*/) ) < 0)
    {
        printf("can't open /dev/mem - need root ?\n");
        exit(-1);
    }

    /* mmap BCM System Timer */
    void *TIMER_map = mmap(
        NULL,
        4096, /*BLOCK_SIZE */
        PROT_READ |PROT_WRITE /*|PROT_WRITE*/,
        MAP_SHARED,
        TIMER_memFd,
        BCM2708_ST_BASE
    );

    close(TIMER_memFd);

    if (TIMER_map == MAP_FAILED)
    {
        printf("mmap error %d\n", (int)TIMER_map);
        exit(-1);
    }
    timer = (long long int *)((char *)TIMER_map + 4);
    TIMER_registers = (volatile unsigned *)TIMER_map;
}

int main()
{
  struct timeval begin; 
  struct timespec gt ={0,0};
  TIMER_Init();
  long int i = 0;
  long int j = 0;
  long int k = 0;
  long long test;
  long long int start = *timer;

  long long int end1 = *timer;

  for(i = 0; i < 10000000; i++){;}
  long long int end2 = *timer;

  for(k = 0; k < 10000000; k++)
{
  gettimeofday(&begin, NULL);
}
  long long int end3 = *timer;

for(k = 0; k < 10000000; k++)
{
  clock_gettime(CLOCK_MONOTONIC, &gt);
}
  long long int end4 = *timer;

  for(j = 0;j< 10000000;j++)
{
  test = *timer;
}
  long long int end5 = *timer;
  printf("Result in microsecond:\n");
  printf("without any load:\n");
  printf("%lld\n",end1-start);
  printf("call empty loop 10M times:\n");
  printf("%lld\n",end2-end1);
  printf("call gettimeofday() 10M times:\n");
  printf("%lld\n",end3-end2);
  printf("call clock_gettime() 10M times:\n");
  printf("%lld\n",end4-end3);
  printf("call direct ticks 10M times:\n");
  printf("%lld\n",end5-end4);
  fflush(stdout);
  return 0;
}


