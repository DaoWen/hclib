#include <stdlib.h>
#include <stdio.h>
#include <hclib.h>

#define FINISH GEN_FINISH_SCOPE(MACRO_CONCAT(_hcGenVar_, __COUNTER__))
#define GEN_FINISH_SCOPE(V) for (int V=(start_finish(), 1); V; end_finish(), --V)
#define MACRO_CONCAT(a, b) DO_MACRO_CONCAT(a, b)
#define DO_MACRO_CONCAT(a, b) a ## b

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

void taskMain(void *raw_args) {
    char **argv = raw_args;
    int n = atoi(argv[1]);
    FibArgs args = { n, 0 };
    FINISH {
        async(fib, &args, NO_DDF, NO_PHASER, NO_PROP);
    }
    printf("Fib(%d) = %ld\n", n, args.res);
}

int main(int argc, char ** argv) {
    hclib_launch(&argc, argv, taskMain, argv);
}
