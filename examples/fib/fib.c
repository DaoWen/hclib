#include <stdlib.h>
#include <stdio.h>
#include <hclib.h>
#include <assert.h>

#include "perf-support.h"

#define FINISH GEN_FINISH_SCOPE(MACRO_CONCAT(_hcGenVar_, __COUNTER__))
#define GEN_FINISH_SCOPE(V) for (int V=(start_finish(), 1); V; end_finish(), --V)
#define MACRO_CONCAT(a, b) DO_MACRO_CONCAT(a, b)
#define DO_MACRO_CONCAT(a, b) a ## b


////////////////////////////////////
// ITERATIVE VERSION

long fib_iter(int n) {
    int i, x, y;
    for (i=0, x=1, y=0; i<=n; i++) {
        int t = x;
        x = y;
        y += t;
    }
    return x;
}


////////////////////////////////////
// ASYNC-FINISH VERSION

typedef struct {
    int n;
    long res;
} FibArgs;

void fib(void * raw_args) {
    FibArgs *args = raw_args;
    if (args->n < 2) {
        args->res = args->n;
    }
    else {
        FibArgs lhsArgs = { args->n - 1, 0 };
        FibArgs rhsArgs = { args->n - 2, 0 };
        FINISH {
            async(fib, &lhsArgs, NO_DDF, NO_PHASER, NO_PROP);
            async(fib, &rhsArgs, NO_DDF, NO_PHASER, NO_PROP);
        }
        args->res = lhsArgs.res + rhsArgs.res;
    }
}


////////////////////////////////////
// DDT VERSION

#if 0
#define MY_ESCAPE_PROP NO_PROP
#else
#define MY_ESCAPE_PROP ESCAPING_ASYNC
#endif

typedef struct {
    int n;
    long resval;
    struct ddf_st *res;
    struct ddf_st *subres[3];
} FibDDtArgs;

static FibDDtArgs *setup_fib_ddt_args(int n) {
    FibDDtArgs *args = malloc(sizeof(*args));
    args->n = n;
    args->res = ddf_create();
    args->subres[2] = NULL;
    return args;
}

static void free_ddt_args(FibDDtArgs *args) {
    ddf_free(args->res);
    free(args);
}

void fib_ddt_res(void * raw_args) {
    FibDDtArgs *args = raw_args;
    FibDDtArgs *lhs = ddf_get(args->subres[0]);
    FibDDtArgs *rhs = ddf_get(args->subres[1]);
    args->resval =  lhs->resval + rhs->resval;
    ddf_put(args->res, args);
    // cleanup
    free_ddt_args(lhs);
    free_ddt_args(rhs);
}

void fib_ddt(void * raw_args) {
    FibDDtArgs *args = raw_args;
    if (args->n < 2) {
        args->resval = args->n;
        ddf_put(args->res, args);
    }
    else {
        FibDDtArgs *lhsArgs = setup_fib_ddt_args(args->n - 1);
        FibDDtArgs *rhsArgs = setup_fib_ddt_args(args->n - 2);
        args->subres[0] = lhsArgs->res;
        args->subres[1] = rhsArgs->res;
        // sub-computation asyncs
        async(fib_ddt, lhsArgs, NO_DDF, NO_PHASER, MY_ESCAPE_PROP);
        async(fib_ddt, rhsArgs, NO_DDF, NO_PHASER, MY_ESCAPE_PROP);
        // async-await for sub-results
        async(fib_ddt_res, args, args->subres, NO_PHASER, MY_ESCAPE_PROP);
    }
}

void fib_ddt_root_await(void * raw_args) { /* no-op */ }


////////////////////////////////////
// DRIVER

void taskMain(void *raw_args) {
    char **argv = raw_args;
    const int n = atoi(argv[1]);
    const int doDDT = argv[2] && atoi(argv[2]);
    const long fn = fib_iter(n);
    const long fnp1 = fib_iter(n+1);
    long answer;
    // ASYNC-FINISH version
    if (!doDDT) {
        start();
        FibArgs args = { n, 0 };
        FINISH {
            async(fib, &args, NO_DDF, NO_PHASER, NO_PROP);
        }
        double time_sec = stop();
        print_throughput(fnp1, time_sec);
        answer = args.res;
        //printf("asyncs = %ld\tfins=%ld\n", 2*fnp1-1, fnp1);
    }
    // DDT version
    else {
        start();
        FibDDtArgs *args = setup_fib_ddt_args(n);
        FINISH {
            async(fib_ddt, args, NO_DDF, NO_PHASER, MY_ESCAPE_PROP);
            args->subres[0] = NULL; // null terminate after res
            async(fib_ddt_root_await, args, &args->res, NO_PHASER, NO_PROP);
        }
        double time_sec = stop();
        print_throughput(fnp1, time_sec);
        answer = args->resval;
    }
    // check results
    printf("Fib(%d) = %ld = %ld\n", n, fn, answer);
    assert(answer == fn);
}

int main(int argc, char ** argv) {
    hclib_launch(&argc, argv, taskMain, argv);
}
