
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread_pool.h"
#include "ring_buffer_impl.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("usage: %s count.\n", argv[0]);
        return -1;
    }

    int task_count = atoi(argv[1]);

    //time_t start = get_current_timestamp();
    time_t start;
    time(&start);

    threadpool_t* pool = threadpool_create(16, 65535, 0);

    int* file_index = (int*)malloc(sizeof(int) * task_count);
    for (int i = 1; i < task_count+1; i ++)
    {
        file_index[i] = i;
    }

    for (int i = 1 ; i < task_count+1; i ++)
    {        
        if(0 != threadpool_add(pool, do_work, (void*)&file_index[i]))
        {
            printf("add task to thread pool failed.\n");
        }else{
            printf("add task thread file_index:%d to pool success.\n",i);
        }
    }

#if 1
    if (0 != threadpool_destroy(pool, graceful_shutdown))
    {
        printf("destroy thread pool failed.\n");
    }
#endif

    //time_t end = get_current_timestamp();
    time_t end;
    time(&end);

    printf("cost: %d\n", end - start);

    for(;;);

    return 0;
}


