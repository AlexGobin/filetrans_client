#ifndef _RING_BUFFER_IMPL_H_
#define _RING_BUFFER_IMPL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "ringbuffer.h"

#define	  FIFO_SIZE	1024*1024*10

struct worker_thread_context{
    //pthread_t pthread_t_id;//线程ID
    unsigned int worker_id;//工作ID
    struct ringbuffer *ring_buf;//内存指针
    char original_file_name[FIFO_SIZE];//源文件名
    char segement_file_name[FIFO_SIZE];//分片的文件名
};

void * consumer_proc(void *m_worker_t_context);
pthread_t consumer_thread(void *m_wb,int m_work_id);
void * producer_proc(void *m_worker_t_context);
pthread_t producer_thread(void *m_rb,int m_work_id);
void * do_work(void* argv);

#endif

