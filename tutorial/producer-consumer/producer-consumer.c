#include <stdlib.h>
#include <stdio.h>
#include <hclib.h>

// Put on a DDF, transmitting data
void producer(void *args) {
    struct ddf_st *ddf = args;
    int *data = malloc(sizeof(int));
    data[0] = 5;
    printf("Producing value %d\n", data[0]);
    ddf_put(ddf, data);
}

// Get on a DDF dependence
void consumer(void *args) {
    struct ddf_st *ddf = (struct ddf_st *) args;
    int *data = ddf_get(ddf);
    printf("Consuming value %d\n", data[0]);
}

int main(int argc, char ** argv) {
    hclib_init(&argc, argv);

    struct ddf_st **ddf_list = ddf_create_n(1 /*size*/, 1 /*is null-terminated*/);
    struct ddf_st *ddf = ddf_create();
    ddf_list[0] = ddf;

    // Create a DDT depending on ddf
    // Pass the ddf has an argument too for consumption
    async(&consumer, ddf, ddf_list, NO_PHASER, NO_PROP);

    // Pass the ddf down to the consumer
    async(&producer, ddf, NO_DDF, NO_PHASER, NO_PROP);

    hclib_finalize();
}
