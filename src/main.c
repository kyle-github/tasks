#include "get_cpu_count.h"
#include "threadlet.h"
#include <stdio.h>

// Example threadlet function
threadlet_state_t my_threadlet_func(void *data, intptr_t *result) {
    printf("Threadlet started with data: %s\n", (char *)data);

    threadlet_yield();  // Yield back to the main context

    printf("Threadlet finished with data: %s\n", (char *)data);

    return THREADLET_FINISHED;
}

int main() {
    printf("CPU Count: %d\n", get_cpu_count());

    threadlet *t;

    if(threadlet_create(&t, my_threadlet_func, "Hello, World!") != 0) {
        printf("Failed to create threadlet!\n");
        return -1;
    }

    printf("Resuming threadlet...\n");
    threadlet_resume(t);

    printf("Joining threadlet...\n");
    threadlet_join(t);

    printf("Threadlet destroyed\n");

    return 0;
}
