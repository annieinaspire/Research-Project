//funciotn0(argv=0): cold, funciotn1(argv=1): hot

#define _GNU_SOURCE
#include <sched.h>   //cpu_set_t , CPU_SET
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>
#include "test.h"

pthread_t threads[10];
int maxpri = 99;

unsigned int c[500000000];

struct parameter
{
	int id;
	int cpu;
};

struct parameter para[10];

void * thread_function1(void *arg)
{
	 struct parameter *para = (struct parameter *) arg;

	 pid_t tid = syscall(SYS_gettid);
	 //int ret = setpriority(PRIO_PROCESS, tid, -10);
	 //printf("1, core %d, tid of thread %d , %d, %d\n", para->cpu, tid ,para->id, getpriority(PRIO_PROCESS,tid));

	sleep(1); 
	/*int policy; 
	struct sched_param param1; 
	pthread_attr_t attr1;
	pthread_attr_init(&attr1);

	pthread_attr_setinheritsched(&attr1,PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr1,SCHED_RR);
	param1.sched_priority = 50;
	pthread_attr_setschedparam(&attr1,&param1);

	pthread_attr_getschedpolicy(&attr1,&policy);
	if(policy==SCHED_FIFO)
	    printf("Schedpolicy:SCHED_FIFO, hot = 1, thread %d,%d\n",para->id,tid);
	if(policy==SCHED_RR)
	    printf("Schedpolicy:SCHED_RR, hot = 1, thread %d,%d\n",para->id,tid);
	if(policy==SCHED_OTHER)
	    printf("Schedpolicy:SCHED_OTHER, hot = 1, thread %d,%d\n",para->id,tid);*/

	struct sched_param param;
	/*int maxpri; 
	maxpri = sched_get_priority_max(SCHED_RR);
	if(maxpri == -1) 
	{ 
	      perror("sched_get_priority_max() failed"); 
	      exit(1); 
	}*/
	//printf("### %d ###\n",maxpri);
	param.sched_priority = maxpri;
	//maxpri-=1; 
	if (sched_setscheduler(tid, SCHED_RR, &param) == -1) //设置优先级
	{ 
	      perror("sched_setscheduler() failed"); 
	      exit(1); 
	}

	cpu_set_t cpuset;
  	cpu_set_t cpuget; 
	int cpu = para->cpu; //the CPU we want to use

	CPU_ZERO(&cpuset);       //clears the cpuset
	CPU_SET( cpu , &cpuset); //set CPU 1 on cpuset
	if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) < 0) {
	  fprintf(stderr, "set thread affinity failed\n");
	}
	CPU_ZERO(&cpuget); 
	if (pthread_getaffinity_np(pthread_self(), sizeof(cpuget), &cpuget) < 0) {
	  fprintf(stderr, "get thread affinity failed\n");
	}

	hot(para->id, tid, param);
}

void * thread_function0(void *arg)
{
	struct parameter *para = (struct parameter *) arg;
	
	 pid_t tid = syscall(SYS_gettid);
	 //int ret = setpriority(PRIO_PROCESS, tid, 10);
	 //printf("0, core %d, tid of thread %d , %d, %d\n", para->cpu, tid, para->id, getpriority(PRIO_PROCESS,tid));

	sleep(1); 
	/*int policy; 
	struct sched_param param0; 
	pthread_attr_t attr0;
	pthread_attr_init(&attr0);

	pthread_attr_setinheritsched(&attr0,PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr0,SCHED_RR);
	param0.sched_priority = 50;
	pthread_attr_setschedparam(&attr0,&param0);
	
	pthread_attr_getschedpolicy(&attr0,&policy);
	if(policy==SCHED_FIFO)
	    printf("Schedpolicy:SCHED_FIFO, cold = 0, thread %d,%d\n",para->id,tid);
	if(policy==SCHED_RR)
	    printf("Schedpolicy:SCHED_RR, cold = 0, thread %d,%d\n",para->id,tid);
	if(policy==SCHED_OTHER)
	    printf("Schedpolicy:SCHED_OTHER, cold = 0, thread %d,%d\n",para->id,tid);*/


	struct sched_param param; 
	/*int maxpri; 
	maxpri = sched_get_priority_max(SCHED_RR);
	if(maxpri == -1) 
	{ 
	      perror("sched_get_priority_max() failed"); 
	      exit(1); 
	}*/
	//printf("### %d ###\n",maxpri); 
	param.sched_priority = maxpri;
	//maxpri-=1; 
	if (sched_setscheduler(tid, SCHED_RR, &param) == -1) //设置优先级
	{ 
	      perror("sched_setscheduler() failed"); 
	      exit(1); 
	}


	cpu_set_t cpuset;
  	cpu_set_t cpuget; 
	int cpu = para->cpu; //the CPU we want to use

	CPU_ZERO(&cpuset);       //clears the cpuset
	CPU_SET( cpu , &cpuset); //set CPU 3 on cpuset
	if (pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset) < 0) {
	  fprintf(stderr, "set thread affinity failed\n");
	}
	CPU_ZERO(&cpuget); 
	if (pthread_getaffinity_np(pthread_self(), sizeof(cpuget), &cpuget) < 0) {
	  fprintf(stderr, "get thread affinity failed\n");
	}

	
	
	int *c[1000000];
	int **ptr;
	int i,j,k;

	for(i=0;i<1000000;i++)
	{
		j=(i+64)%1000000;
		c[i] = (int*)&c[j];
		//printf("%p\n",c[i]);
	}
	//printf("%p,%p,%p\n",c[0],c[2],c[4]);

	struct timeval start_1, end_1;
	gettimeofday(&start_1, 0);

	ptr = &c[0]; //printf("%p\n",*ptr);
	for(k=0;k<1500000;k++)
	{
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr; //printf("%p\n",*ptr);
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr; 
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;
		ptr = (int**)*ptr;


		//if( !(k%1000) )printf(" %d",para->id);

		/*if( k!=0 && !(k%1000) ){
			//printf(" %d",para->id);

			if(maxpri == 0){
				maxpri=99;
				//printf(" thread id: %d\n",tid);
				pid_t tid_t = tid;
				int para_id_t = para->id;
				int kk;
				for(kk=0;kk<10;kk++){
					if(para_id_t>10){
						para_id_t-=10;
						tid_t-=10;
						param.sched_priority = maxpri;
						if (sched_setscheduler(tid_t, SCHED_RR, &param) == -1) //设置优先级
						{ 
						      //perror("sched_setscheduler() failed"); 
						      exit(1); 
						}
						maxpri-=1;
					}
					else{
						param.sched_priority = maxpri;
						if (sched_setscheduler(tid_t, SCHED_RR, &param) == -1) //设置优先级
						{ 
						      //perror("sched_setscheduler() failed"); 
						      exit(1); 
						}
						maxpri-=1;
						tid_t+=1;
						para_id_t+=1;
					}
				}
			}
			else{
				//printf(" %d(%d)",para->id,maxpri);
				printf(" %d",para->id);
				param.sched_priority = maxpri;
				maxpri-=1; 
				if (sched_setscheduler(tid, SCHED_RR, &param) == -1) //设置优先级
				{ 
				      perror("sched_setscheduler() failed"); 
				      exit(1); 
				}
			}
		}*/
	}
	
	gettimeofday(&end_1, 0);
    	int sec1 = end_1.tv_sec - start_1.tv_sec;
	int usec1 = end_1.tv_usec - start_1.tv_usec;
	float interval1 = (sec1*1000+(usec1/1000.0));
	//printf("\n0 Pthread %d exit, Elapsed Time: %f milliseconds\n",para->id,interval1);
}


int main(int argc, char *argv[])
{
	pid_t pid = getpid();
	pid_t tid = syscall(SYS_gettid);
	//printf("main thread : pid = %d , tid = %d \n" , pid, tid);

	int i,j;
	for(i=0;i<10;i++)
	{	
		para[i].cpu=0;	
		para[i].id=i+1;
		//argv=1,function1;argv=0,function0
		j=atoi(argv[i+1]);
		if(j==0)pthread_create(&threads[i], NULL, thread_function0, (void *) &para[i]);
		else pthread_create(&threads[i], NULL, thread_function1, (void *) &para[i]);
		//printf("thread %d created\n", i);
		usleep(1000);
	}	
	for(;i<20;i++)
	{
		para[i].cpu=1;	
		para[i].id=i+1;
		//argv=1,function1;argv=0,function0
		j=atoi(argv[i+1]);
		if(j==0)pthread_create(&threads[i], NULL, thread_function0, (void *) &para[i]);
		else pthread_create(&threads[i], NULL, thread_function1, (void *) &para[i]);
		//printf("thread %d created\n", i);
		usleep(1000);
	}

	for(i=0;i<10;i++)pthread_join(threads[i], NULL);

	return 0;
}
