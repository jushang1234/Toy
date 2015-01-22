/* csci4061 F2013 Assignment 4 
* section: one_digit_number 
* date: mm/dd/yy 
* names: Name of each member of the team (for partners)
* UMN Internet ID, Student ID (xxxxxxxx, 4444444), (for partners)
*/


//NEED MUTEX LOOK AROUND POP() AND PUSH()

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// #include "util.h"
#include <unistd.h>
#include <semaphore.h>
#define MAX_THREADS 100
#define MAX_QUEUE_SIZE 100
#define MAX_REQUEST_LENGTH 64
#define MAX_PATH_LENGTH 1024
//Structure for queue.
typedef struct 
{
	int		m_socket;
	char	m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;


typedef struct  
{
	request_queue_t* contents;
	sem_t occupied;
	sem_t empty;
	int front;
	int count;
	int size;
} request_queue; 


 pthread_mutex_t queuelock;
 sem_t producer;
 sem_t consumer;





static char path[MAX_PATH_LENGTH];
static request_queue* queue;

char* appendstring(char* str1, char*str2)
{
	char * new_str ;
	if((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL){
    new_str[0] = '\0';   // ensures the memory is an empty string
    strcat(new_str,str1);
    strcat(new_str,str2);
} else {
    fprintf(stderr,"malloc failed!\n");
}
}

int push(request_queue* queue, request_queue_t item)
{

  int newElementIndex;
	// if(sem_getvalue(&(queue->empty), &val)!=0)
	// 	printf("faile to getvalue\n");
	// 	printf("value of empty %d\n", val);
	  pthread_mutex_lock(&queuelock);
  newElementIndex = (queue->front
                    + queue->count)
                    % queue->size;
  queue->contents[newElementIndex] = item;
  queue->count++;
  printf("push good with %d \n", queue->count);
  pthread_mutex_unlock(&queuelock);


}

request_queue_t* pop(request_queue* queue)
{
  request_queue_t* oldElement;

  pthread_mutex_lock(&queuelock);
  if (queue->count <= 0) {
    fprintf(stderr, "QueueDelete on Empty Queue.\n");
  }
  oldElement = &(queue->contents[queue->front]);
  queue->front++;
  queue->front %= queue->size;
  queue->count--;
  printf("pop good with %d \n", queue->count);
  pthread_mutex_unlock(&queuelock);
  return oldElement;
}

request_queue* initqueue(int size)
{
	request_queue* queue = (request_queue*)malloc(sizeof(request_queue));
	queue->count = 0;
	queue->size = size;
	if(sem_init(&(queue->empty), 0, size)!=0)
		printf("fail to init\n");
	if(sem_init(&(queue->occupied), 0, 0)!=0)
		printf("fail to init\n");
	queue->contents =(request_queue_t*)malloc(size*sizeof(request_queue_t));
	return queue;
}



void * dispatch(void * arg)
{
	while(1){
		request_queue_t* package = (request_queue_t*)malloc(sizeof(request_queue_t));
		sem_wait(&(queue->empty));
		push(queue, *package);
		sem_post(&(queue->occupied));
	}
	// while (1)
	// {
	// int fd;
	// char request[1024];

	// if ((fd = accept_connection())==-1)
	// {
	// 	fprintf(stderr, "error accept_connection\n");
	// 	pthread_exit(NULL);
	// }
	// if(get_request(fd, request)!=0)
	// {
	// 	fprintf(stderr, "error get_request\n");
	// }
	// printf("request: %s\n", request);
	// request_queue_t* package = (request_queue_t*)malloc(sizeof(request_queue_t));
	// package->m_socket = fd;
	// strcpy(package->m_szRequest, request);
	// printf("request cpyed: %s\n", package->m_szRequest);

	// sem_wait(&producer);
	// push(queue, *package);
	// sem_post(&consumer);
	// }


}

void * worker(void * arg)
{
	while(1){
		printf("%ld\n", );
		  sem_wait(&(queue->occupied));
		pop(queue);
		  sem_post(&(queue->empty));


	}
	// while(1)
	// {
	// sem_wait(&consumer);
	// printf("workers awaked\n");
	// request_queue_t* ele = pop(queue);
	// printf("pop works\n");
	// sem_post(&producer);
	// char* abspath = appendstring(path, ele->m_szRequest);
	// printf("get abspath %s\n", abspath);
	// char* type;
	// if(access(abspath, F_OK) != -1 ) 
	// {
	// 	printf("file exit!\n");
 //    	// file exists
 //    	if(strstr(ele->m_szRequest, ".html")!=NULL)
	// 	{
	// 		type = "text/html";

	// 	}
	// 	if (strstr(ele->m_szRequest, ".jpg")!=NULL)
	// 	{
	// 		type = "image/jpeg";
	// 	}
	// 	if (strstr(ele->m_szRequest, ".jpg")!=NULL)
	// 	{
	// 		type = "image/gif";
	// 	}
	// 	else
	// 	{
	// 		type="text/plain";
	// 	}
	// 	printf("type is %s\n", type);
	// 	char buffer[512];
	// 	int fp;
 //    	if ((fp = open(abspath, O_RDONLY)) < 0)
 //    	{
 //    		return_error(ele->m_socket, "you go fuck you self");
 //    	}
 //    	printf("file opened\n");
	// 	size_t nbytes = 0;
	// 	while ((nbytes = read(fp, buffer, 1)) > 0)
	// 	{
	// 		printf("\n");
	// 		return_result(ele->m_socket, type, buffer, nbytes);
	// 	}
	// 	free(abspath);
	// } 
	// else 
	// {
	// 	printf("file not exit!\n");

 //    	// file doesn't exist
 //    	free(abspath);
	// 	return_error(ele->m_socket, "you go fuck you self");
	// }
	// }

}

int main(int argc, char **argv)
{
	//Error check first.
	// if(argc != 6 && argc != 7)
	// {
	// 	printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
	// 	return -1;
	// }
	// printf("Call init() first and make a dispather and worker threads\n");

	// init(atoi(argv[1]));
	// getcwd(path, MAX_PATH_LENGTH);
	// strcpy(path, argv[2]);
	int num_workers = 10;
	int num_dispatcher = 100;
	int queue_length = 20;
	// int num_workers = atoi(argv[3]);
	// int num_dispatcher =  atoi(argv[4]);
	// int queue_length = atoi(argv[5]);

    pthread_t dispatchid[num_dispatcher];
    pthread_t workerid[num_workers];

    queue = initqueue(queue_length);

    pthread_mutex_init(&queuelock, NULL);
printf("cwd is : %s\n", path);

    for (int i = 0; i < num_dispatcher; ++i)
    {
	    pthread_create(&dispatchid[i], NULL, (void *)dispatch, NULL);
    }
    for (int i = 0; i < num_workers; ++i)
    {
    	pthread_create(&workerid[i], NULL, (void *)worker, NULL);
    }
    for (int i = 0; i < num_dispatcher; ++i)
    {
	    pthread_join(dispatchid[i],NULL);
    }
    for (int i = 0; i < num_workers; ++i)
    {
	    pthread_join(workerid[i],NULL);
    }

    return 0;

}