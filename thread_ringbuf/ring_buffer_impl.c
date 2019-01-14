#include "ring_buffer_impl.h"

void * consumer_proc(void *m_worker_t_context)
{
	int i_result = 0;
	struct worker_thread_context *_worker_t_context = (struct worker_thread_context *)m_worker_t_context;

    int _w_fd = open(_worker_t_context->segement_file_name, O_CREAT|O_RDWR|O_APPEND);

	while(1)
	{
		usleep(100);
		//printf("------------------------------------------\n");
		//printf("get data from ring buffer.\n");			
		{
			
			if (ringbuffer_is_empty(_worker_t_context->ring_buf)) {
				printf("buffer is empty !\n");
				sleep(1);
				continue;
			}

			ringbuffer_to_fd(_w_fd,_worker_t_context->ring_buf, sizeof(_worker_t_context->ring_buf));
		}
		//printf("ring buffer length: %u\n", ringbuffer_len(_worker_t_context->ring_buf));
		//printf("------------------------------------------\n");
	}
}

pthread_t consumer_thread(void *m_wb,int m_work_id)
{
	int err;
	pthread_t tid;
    struct worker_thread_context _worker_t_context;
    _worker_t_context.worker_id = m_work_id;
    sprintf(_worker_t_context.segement_file_name,"./output/Cpp_Concurrency_In_Action.pdf%d.tmp",m_work_id);
    //printf("consumer_thread segement_file_name:%s\n",_worker_t_context.segement_file_name);
    _worker_t_context.ring_buf = (struct ringbuffer *)m_wb;
	err = pthread_create(&tid, NULL, consumer_proc, (void*)&_worker_t_context);
	if (err != 0)
	{
		fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
				errno, strerror(errno));
		return -1;
	}
	return tid;

}

void * producer_proc(void *m_worker_t_context)
{
    int _result = 0;
    
	struct worker_thread_context *_worker_t_context = (struct worker_thread_context *)m_worker_t_context;

    printf("******************************************\n");
    printf("put file(%s) datas to ring buffer.\n",_worker_t_context->original_file_name);
    int _r_fd = open(_worker_t_context->original_file_name, O_RDONLY );
    //sleep(2);

	while(1)
	{
        usleep(100);
		//printf("******************************************\n");
		//printf("put datas to ring buffer.\n");
		
		if (ringbuffer_is_full(_worker_t_context->ring_buf)) {
			printf("buffer is full !\n");
			sleep(1);
			continue;
		}        
        _result = ringbuffer_from_dev(_r_fd,_worker_t_context->ring_buf,FIFO_SIZE+1);
        if(_result < 0){
            printf("producer_proc ringbuffer_from_dev failed! Error number:%d\n",_result);
            return NULL;
        }
        
		//printf("ring buffer length: %u\n", ringbuffer_len(_worker_t_context->ring_buf));
		//printf("******************************************\n");
		sleep(1);
	}
}
pthread_t producer_thread(void *m_rb,int m_work_id)
{
	int err;
	pthread_t tid;
    struct worker_thread_context _worker_t_context;
    _worker_t_context.worker_id = m_work_id;
    snprintf(_worker_t_context.original_file_name,FIFO_SIZE, "./Cpp_Concurrency_In_Action.pdf%d.tmp",m_work_id);
    _worker_t_context.ring_buf = (struct ringbuffer *)m_rb;
	err = pthread_create(&tid, NULL, producer_proc, (void*)&_worker_t_context);
	if (err != 0)
	{
		fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
				errno, strerror(errno));
		return -1;
	}
	return tid;
}


//int __attribute__((const))dump_pthread_join_errinfo(int errno)
/*int dump_pthread_join_errinfo(int i_errno)
{
    if(i_errno == EDEADLK){
        printf(" A deadlock was detected \n");
    }else if(i_errno==EINVAL){
        printf(" thread is not a joinable thread or Another thread is already waiting to join with this thread\n");
    }else{
        printf(" No thread with the ID thread could be found \n");
    }
    return 0;
}*/


void * do_work(void* argv)
{
	struct ringbuffer *ring_buf;
	pthread_t produce_pid, consume_pid; 
    int file_index = *((int*)argv);

	ring_buf = ringbuffer_create(FIFO_SIZE);
	if (!ring_buf) {
		perror("ringbuffer_create()");
		exit(1);
	}

	printf("multi thread test.......\n");

	produce_pid  = producer_thread((void*)ring_buf,file_index);
	consume_pid  = consumer_thread((void*)ring_buf,file_index);

	int i_result = pthread_join(produce_pid, NULL);
    //dump_pthread_join_errinfo(i_result);
     if(i_result == EDEADLK){
        printf(" A deadlock was detected \n");
    }else if(i_result==EINVAL){
        printf(" thread is not a joinable thread or Another thread is already waiting to join with this thread\n");
    }else{
        printf(" No thread with the ID thread could be found \n");
    }

	i_result = pthread_join(consume_pid, NULL);
    if(i_result == EDEADLK){
        printf(" A deadlock was detected \n");
    }else if(i_result==EINVAL){
        printf(" thread is not a joinable thread or Another thread is already waiting to join with this thread\n");
    }else{
        printf(" No thread with the ID thread could be found \n");
    }

	ringbuffer_destroy(ring_buf);

	return NULL;
}

