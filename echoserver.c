/* 
 * echoservert_pre.c - A prethreaded concurrent echo server


g++ -c -I /usr/local/include/opencv4 echoserver.c echo.c csapp.h csapp.c  sbuf.h sbuf.c
g++ echo.o echoserver.o csapp.o sbuf.o -o echoserver -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui  -L/usr/local/lib -lopencv_imgcodecs -L/usr/lib
 */
/* $begin echoservertpremain */
#include "csapp.h"
#include "sbuf.h"
#include <time.h>
#define NTHREADS  3
#define SBUFSIZE  4

void echo(int connfd);
void *threadTask(void *vargp);

sbuf_t sbuf; /* shared buffer of connected descriptors */

int main(int argc, char **argv) 
{
    time_t req_time;
    int i, listenfd, connfd, port, minPriority, mainThreadPriority, policy;
    socklen_t clientlen = sizeof(struct sockaddr_in);
    struct sched_param mainThreadParam;
    struct sockaddr_in clientaddr;
    pthread_t tid; 

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    /* MAIN-THREAD WITH HIGH PRIORITY */
	mainThreadParam.sched_priority = sched_get_priority_max(SCHED_FIFO);
	mainThreadPriority = mainThreadParam.sched_priority;
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &mainThreadParam);
	pthread_getschedparam (pthread_self(), &policy, &mainThreadParam);


    port = atoi(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); //line:conc:pre:initsbuf
    listenfd = Open_listenfd(argv[1]);

    for (i = 0; i < NTHREADS; i++)  /* Create worker threads */ //line:conc:pre:begincreate
	    Pthread_create(&tid, NULL, threadTask, NULL);               //line:conc:pre:endcreate

    int newRequest = 0;

    while (1) {
        newRequest = 0;
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        newRequest = 1;
        // buffer ka rear dekhu
        // size == rear, while()
        // for 10, firse check karega
        
        // time(&req_time);
        // printf("\nArrived at: %ld\n", req_time);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
        printf("Added request to the queue\n");
        
        P(&(sbuf.mutex));                          /* Lock the buffer */
        
        printf("rear: %d    front: %d\n", sbuf.rear, sbuf.front);
        
        V(&(sbuf.mutex));  

        if(!sbuf_is_empty(&sbuf) && newRequest == 1)
            printf("Request buffer is full, please wait. Connfd: %d\n", connfd);
    }
}

void *threadTask(void *vargp) 
{
    pthread_t threadId = pthread_self();
    struct sched_param param;
    time_t start_time, cur_time;
    int delay = 10;
    int priority;

    param.sched_priority = sched_get_priority_min(SCHED_FIFO);
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	// ret = pthread_getschedparam (threadId, &policy, &param);
	priority = param.sched_priority;	
	// printf("Thread %ld priority: %d \n", threadId, priority);
    
    Pthread_detach(threadId);
    // printf("Thread %ld waiting for %d seconds before servicing requests\n", threadId, delay);
    

    while (1) {

        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */ //line:conc:pre:removeconnfd
        // printf("Thread %ld processing requests\n", threadId);
        time(&start_time);
        do
        {
            time(&cur_time);
            // printf("cur_time: %ld\n", cur_time);
        }
        while((cur_time - start_time) < delay);
        echo(connfd);                /* Service client */
        Close(connfd);
               
    }
}
/* $end echoservertpremain */
