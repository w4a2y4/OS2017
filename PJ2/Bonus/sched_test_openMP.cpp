/*
OS PJ2 Part 3_bonus:
Written By Team 28: B04902077 B04902079
Ref: http://morris821028.github.io/2016/01/21/realtime-system-using-scheduler/
Compilation: g++ sched_test_OpenMP.cpp -fopenmp -lrt -o sched_test_OpenMP
Execution:
1. Default: ./sched_test_OpenMP
2. FIFO: sudo ./sched_test_OpenMP SCHED_FIFO
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <assert.h>
#include <linux/sched.h>
#include <omp.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <pthread.h>

using namespace std;

cpu_set_t mask;

static double my_clock(void) {
    struct timespec t;
    assert(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) == 0);
    return 1e-9 * t.tv_nsec + t.tv_sec;
}

int main(int argc, char **argv){
	omp_set_num_threads(2);
	if(argc == 2 && strcmp(argv[1],"SCHED_FIFO")==0){
		CPU_ZERO(&mask);
		CPU_SET(0, &mask);
		sched_setaffinity(0, sizeof(mask), &mask);
		struct sched_param param;
		param.sched_priority = sched_get_priority_max(SCHED_FIFO);
		sched_setscheduler(0, SCHED_FIFO, &param);
	}
	else if(argc != 1){
		printf("Parameter Error\n");
		return 0;
	}
	#pragma omp parallel for schedule(dynamic,1)
	for(int i=0;i<2;i++){
		for(int g=0;g<3;g++){
			printf("Thread %d is running\n",i);
			volatile double a = 0;
			double st = my_clock();
			while(my_clock()-st<=0.5){
				a += 0.1f;
			}
		}
	}

}
