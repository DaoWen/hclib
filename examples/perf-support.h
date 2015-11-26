#ifndef _HC_PERF_SUPPORT_H
#define _HC_PERF_SUPPORT_H

/*** START PERF PRINT SUPPORT ***/
#include <sys/time.h>

static struct timeval t_start;
static struct timeval t_stop;

void start() {
    gettimeofday(&t_start, NULL);
}

// Returns time elapsed in seconds
double stop() {
    gettimeofday(&t_stop, NULL);
    long start = t_start.tv_sec*1000000+t_start.tv_usec;
    long stop = t_stop.tv_sec*1000000+t_stop.tv_usec;
    long elapsed = stop-start;
    double time_sec = ((double)elapsed)/1000000;
    return time_sec;
}

void print_throughput(long nb_instances, double time_sec) {
    //printf("Workload   (unit): %ld\n", nb_instances);
    //printf("Duration   (s)   : %f\n", time_sec);
    printf("Throughput (op/s): %ld\n", (long)(nb_instances/time_sec));
}
/*** END PERF PRINT SUPPORT ***/

#endif
