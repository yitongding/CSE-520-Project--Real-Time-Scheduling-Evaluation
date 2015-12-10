#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <unistd.h>


/*
* Timing API
*/
#define BCM2708_ST_BASE 0x20003000 /* BCM 2835 System Timer */

volatile unsigned *TIMER_registers;
volatile long long int *timer_api;

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
    timer_api = (long long int *)((char *)TIMER_map + 4);
    TIMER_registers = (volatile unsigned *)TIMER_map;
}

/*
* Benchmark starts
*/

//#define HZ      700000000      /* second */
#define MHZ     1000000         /* millic second */
#define UHZ     1000            /* micro second */

#define MAX		20000           /* max recorded data */

typedef unsigned long long ticks;
typedef struct RECORD {
    ticks dispatch;             /* job release time */
    ticks finish;               /* job deadline */
} RECORD;



/*
 * Task related parameters, time unit is milli second
 */
int wcet = 0;                   /* worest case execution time */
int period = 0;                 /* period (we assume deadline equals period) */
long long duration = 0;         /* task execution duration */
int priority = 0;               /* task priority */
int count = 0;                  /* number of jobs to execute (duration / period) */
ticks start_time;               /* program start time */
int idx = 0;                    /* job index */
struct RECORD data[MAX];        /* recorded data */
unsigned long long init_time;

/*
 * print out the results
 */
static void
print_res(void) {
	int i = 0;
	int miss = 0;
	int wcet_print = 0;
	long long int all = 0;
	int average = 0;
    printf("Release         Start           Finish          Latency         ExTime\n");
	for ( i = 0; i < count; ++i ) 
	{
        printf("%-15lld %-15lld %-15lld %-15lld %-15lld\n", start_time - init_time, data[i].dispatch-init_time, data[i].finish-init_time , data[i].dispatch - start_time, data[i].finish - data[i].dispatch);
	//if (data[i].finish - init_time > start_time - init_time + period*UHZ) miss++;
	all += data[i].finish - data[i].dispatch;
	if (data[i].finish - data[i].dispatch > wcet_print) wcet_print = data[i].finish - data[i].dispatch;
        start_time += period*UHZ;	
    }
	average = all / count;
	printf("\nTotal %d, priority %d, AVG %d, WCET %d\n", count, priority, average, wcet_print);
}
/******************************************************/
/******************************************************/
/******************************************************/
/******************************************************/
/* $Id: cnt.c,v 1.3 2005/04/04 11:34:58 csg Exp $ */



/* sumcntmatrix.c */



//#include <sys/types.h>

//#include <sys/times.h>



// #define WORSTCASE 1

// #define MAXSIZE 100 Changed JG/Ebbe

#define MAXSIZE 50



// Typedefs

typedef int matrix [MAXSIZE][MAXSIZE];



// Forwards declarations


int Test(matrix);

int Initialize(matrix);

int InitSeed(void);

void Sum(matrix);

int RandomInteger(void);



// Globals

int Seed;

matrix Array;

int Postotal, Negtotal, Poscnt, Negcnt;



// The main function

static inline void
workload()

{

   InitSeed();

   //printf("\n   *** MATRIX SUM AND COUNT BENCHMARK TEST ***\n\n");

   //printf("RESULTS OF THE TEST:\n");

   Test(Array);

   //return 1;

}





int Test(matrix Array)

{

   long StartTime, StopTime;

   float TotalTime;



   Initialize(Array);

   StartTime = 1000.0; //ttime();

   Sum(Array);

   StopTime = 1500.0; //ttime();



   TotalTime = (StopTime - StartTime) / 1000.0;



   //printf("    - Size of array is %d\n", MAXSIZE);

   //printf("    - Num pos was %d and Sum was %d\n", Poscnt, Postotal);

   //printf("    - Num neg was %d and Sum was %d\n", Negcnt, Negtotal);

   //printf("    - Num neg was %d\n", Negcnt);

   //printf("    - Total sum time is %3.3f seconds\n\n", TotalTime);

   return 0;

}





// Intializes the given array with random integers.

int Initialize(matrix Array)

{

   register int OuterIndex, InnerIndex;



   for (OuterIndex = 0; OuterIndex < MAXSIZE; OuterIndex++) //100 + 1

      for (InnerIndex = 0; InnerIndex < MAXSIZE; InnerIndex++) //100 + 1

         Array[OuterIndex][InnerIndex] = RandomInteger();



   return 0;

}





// Initializes the seed used in the random number generator.

int InitSeed (void)

{

   Seed = 0;

   return 0;

}



void Sum(matrix Array)

{

  register int Outer, Inner;



  int Ptotal = 0; /* changed these to locals in order to drive worst case */

  int Ntotal = 0;

  int Pcnt = 0;

  int Ncnt = 0;



  for (Outer = 0; Outer < MAXSIZE; Outer++) //Maxsize = 100

    for (Inner = 0; Inner < MAXSIZE; Inner++)

#ifdef WORSTCASE

      if (Array[Outer][Inner] >= 0) {

#else

	if (Array[Outer][Inner] < 0) {

#endif

	  Ptotal += Array[Outer][Inner];

	  Pcnt++;

	}

	else {

	  Ntotal += Array[Outer][Inner];

	  Ncnt++;

	}



  Postotal = Ptotal;

  Poscnt = Pcnt;

  Negtotal = Ntotal;

  Negcnt = Ncnt;

}





// This function returns in milliseconds the amount of compiler time

//int ttime()

//{

//  struct tms buffer;

//int utime;



//times(&buffer);

//utime = (buffer.tms_utime / 60.0) * 1000.0;

//return (utime);

//}





// Generates random integers between 0 and 8095

int RandomInteger(void)

{

   Seed = ((Seed * 133) + 81) % 8095;

   return Seed;

}


/******************************************************/
/******************************************************/
/******************************************************/
/******************************************************/
/*
static inline void
workload()
{
    double temp = 0;
    long long i = 0;

    for (i = 0; i < 2000; i++)
    temp = sqrt((double)i*i);
}
*/

/*
 * each job's work
 * record start time, finish time
 */

static void
work(int sig, siginfo_t *extra, void *cruft) {
	ticks now;
	//int i;

    /* We have reached the count. Print res and quit */
	if (idx >= count) {
        sleep(2);              /* sleep for 10 sec, wait for other task to finish */
        print_res();
        exit(1);
    }

	data[idx].dispatch = *timer_api; //task start time
    
    workload();
    
    data[idx].finish = *timer_api; //task finish time

    ++idx;
}


static void
Usage(void)
{
    fprintf(stderr, "Usage: ./period -c wcet -p period -d duration -l priority\n");
    exit(EXIT_FAILURE);
}

/*
 * Set affinity of the task, alwasy pin it to core 0
 */
static void
set_sched(void){
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) < 0) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    struct sched_param sched;
    sched.sched_priority = priority;
    if (sched_setscheduler(getpid(), SCHED_RR, &sched) < 0) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
}



int
main(int argc, char *argv[]) {
    sigset_t allsigs;
    int cur_val = 0;
	
	TIMER_Init();
	init_time = *timer_api;
	
    while ((cur_val = getopt(argc, argv, "c:p:d:l:")) != -1) {
        switch (cur_val) {
            case 'c': //change wcet to task count 2000
                wcet = atoi(optarg);        
                if ( wcet <= 0 ) {
                    printf("wcet must be greater than zero\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'p':
                period = atoi(optarg);
                if ( period <= 0 ) {
                    printf("period must be greater than zero\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'd': 
                duration = atol(optarg);  //useless!
                if ( duration <= 0 ) {
                    printf("duration must be greater than zero\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':
                priority = atoi(optarg);
                if ( priority <= 0 ) {
                    printf("priority must be greater than zero\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                printf("Error Input!\n");
                Usage();
                exit(EXIT_FAILURE);
                break;
        }
    }

    if ( wcet == 0 || period == 0 || duration == 0 || priority == 0) {
        Usage();
        exit(1);
    }

    count = (duration - 1) / period + 1;  /* number of jobs to release */
    printf("task_count: %d, period: %d, priority: %d\n", count, period, priority);

	set_sched();

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = work;

    if (sigaction(SIGRTMIN, &sa, NULL) < 0) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }

    /* the timer */

    struct itimerspec timerspec;
    timerspec.it_interval.tv_sec = period / 1000;
    timerspec.it_interval.tv_nsec = (period % 1000) * 1000000;

    /* the start time */

    struct timespec now;
    if(clock_gettime(CLOCK_REALTIME, &now) < 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    // Start one second from now.
    timerspec.it_value.tv_sec = now.tv_sec + 1;
    timerspec.it_value.tv_nsec = now.tv_nsec;
	start_time = *timer_api;
	start_time += MHZ;

    struct sigevent timer_event;
    timer_t timer;
    timer_event.sigev_notify = SIGEV_SIGNAL;
    timer_event.sigev_signo = SIGRTMIN;
    timer_event.sigev_value.sival_ptr = (void *)&timer;

    if (timer_create(CLOCK_REALTIME, &timer_event, &timer) < 0) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    if (timer_settime(timer, TIMER_ABSTIME, &timerspec, NULL) < 0) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&allsigs);
    while(1) {
        sigsuspend(&allsigs);
    }
    
    exit(EXIT_SUCCESS);
}

