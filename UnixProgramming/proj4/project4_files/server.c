/* CSci 4061 F2014 Assignment 4
 * Section 3
 * Date: 12/1/2014
 * Name: Jialun Jiang, Yaozhang Xu, Shang Ju
 * ID:   4467970,      4944346,     4455103
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <semaphore.h>
#include "util.h"

#define MAX_THREADS 100
#define MAX_REQUEST_LENGTH 64
#define MAX_CACHE_SIZE 100
#define MAX_PATH_LENGTH 1024
#define MAX_QUEUE_SIZE 100
#define BUF_SIZE 1024

typedef struct
{
	int		m_socket;
	char	m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

typedef struct  
{
	request_queue_t* contents;
	int front;
	int count;
	int size;

} request_queue;

typedef struct
{
	char* path;
	char* buffer;
	long numbytes;
} cache_entry;

pthread_mutex_t cache_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;

sem_t producer;
sem_t consumer;

int cache_sz = 0;
int current_cache_size = 0;

FILE* logfile;

pthread_t dispatchers[MAX_THREADS];
pthread_t workers[MAX_THREADS];

static cache_entry* cache;
char path[MAX_PATH_LENGTH];

static request_queue* task_queue;

char* getType(char* filename);
char* appendString(char* str1, char* str2);
double comp_time(struct timeval time_s, struct timeval time_e);
static void handler(int signo);

// Queue operations
request_queue* initqueue(int size);
void push(request_queue* queue, request_queue_t item);
request_queue_t* pop(request_queue* queue);


void* dispatch(void* args)
{
	
	char file_name[BUF_SIZE];
	char* content_type;
	while(1)
	{
		int fd = accept_connection();
		
		//accept_connection function fails, decrease current dispatchers number
		if(fd < 0)
		{	
			pthread_exit(NULL);
		}
		
		int rcv_request = get_request(fd, file_name);

		//receives faulty requests	
		if(rcv_request)
		{
			continue;
			// The current implementation is to skip the current loop. This is probably not right.
		}
		
		request_queue_t request;
		request.m_socket = fd;		
		strcpy(request.m_szRequest, file_name);
	
		sem_wait(&producer);
		push(task_queue, request);	
		sem_post(&consumer);
		//sleep(1);

	}
	return NULL;
}

void* worker(void* args)
{

	int i, threadId;
	// Find thread ID. Took us about 3 years.
	for (i = 0; i < MAX_THREADS; i++)
	{
		if (pthread_self() == workers[i])
		{
			threadId = i;
			break;
		}
	}

	int number_of_request = 0;

	while(1)
	{
		// Get a request from the queue
		sem_wait(&consumer);
		request_queue_t* task = pop(task_queue);
		sem_post(&producer);
		
		number_of_request++;
		
		// Start timer
		struct timeval time_s, time_e;
		gettimeofday (&time_s, NULL);
		
		char* request_type = getType(task->m_szRequest);
		char* abs_path = appendString(path, task->m_szRequest);
		
		int hit = 0;
		
		// Cache hit
		for (i = 0; i < cache_sz; i++)
		{
			if (cache[i].path != NULL && strcmp(cache[i].path, abs_path) == 0)
			{
				hit = 1;
				break;
			}
		} 
		
		if (hit)
		{
			return_result(task->m_socket, request_type, cache[i].buffer, cache[i].numbytes);
			
			//End timer
			gettimeofday(&time_e, NULL);
			double time_taken = comp_time(time_s, time_e) / 1000.0;
				
			flockfile(logfile);
			fprintf(logfile, "[%d][%d][%d][%s][%ld][%f%s][%s]\n", threadId, number_of_request, task->m_socket, task->m_szRequest, cache[i].numbytes, time_taken, "ms", "HIT");
			fflush(logfile);
			funlockfile(logfile);
			continue;
		}
		
		// Cache miss

		cache_entry new_entry;
		new_entry.path = strdup(abs_path);
		
		FILE* infile = fopen(abs_path, "r");
		if (infile == NULL) 
		{
			char* errmsg = "File not found.";
			return_error(task->m_socket, errmsg);

			// Write error to log
			flockfile(logfile);
			fprintf(logfile, "[%d][%d][%d][%s][%s]\n", threadId, number_of_request, task->m_socket, task->m_szRequest, errmsg);
			fflush(logfile);
			funlockfile(logfile);
			free(abs_path);
			

			// Skip loop
			continue;
		}

		// Get file size in bytes
		long numbytes;
		fseek(infile, 0L, SEEK_END);
		numbytes = ftell(infile);
		fseek(infile, 0L, SEEK_SET);

		// Allocate file buffer
		char* file_buffer = (char*) calloc(numbytes, sizeof(char));

		if (file_buffer == NULL)
		{
			char* errmsg = "File buffer not created.";
			return_error(task->m_socket, errmsg);

			// Write error to log
			flockfile(logfile);
			fprintf(logfile, "[%d][%d][%d][%s][%s]\n", threadId, number_of_request, task->m_socket, task->m_szRequest, errmsg);
			fflush(logfile);
			funlockfile(logfile);
			free(abs_path);

			//Skip loop
			continue;
		}

		// Read file into buffer
		fread(file_buffer, sizeof(char), numbytes, infile);
		fclose(infile);	
		
		new_entry.buffer = file_buffer;
		new_entry.numbytes = numbytes;
		
		// Store the new entry in cache
		pthread_mutex_lock(&cache_lock);
		int index = current_cache_size % MAX_CACHE_SIZE;
		if (cache[index].path != NULL) 
		{
			free(cache[index].path);
			free(cache[index].buffer);
		}
		cache[current_cache_size % MAX_CACHE_SIZE] = new_entry;
		current_cache_size++;
		pthread_mutex_unlock(&cache_lock);

		return_result(task->m_socket, request_type, file_buffer, numbytes);
		
		//End timer
		gettimeofday(&time_e, NULL);
		double time_taken = comp_time(time_s, time_e) / 1000.0;

		// Write to log
		flockfile(logfile);
		fprintf(logfile, "[%d][%d][%d][%s][%ld][%f%s][%s]\n", threadId, number_of_request, task->m_socket, task->m_szRequest, numbytes, time_taken, "ms", "MISS");
		fflush(logfile);
		funlockfile(logfile);
		free(abs_path);


	}	
	return NULL;
}

int main(int argc, char **argv)
{
	
	//Error check first.
	if(argc != 6 && argc != 7)
	{
		printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
		return -1;
	}
	
	// Initialization
	int port_number = atoi(argv[1]);
	strcpy(path,argv[2]);
	int num_dispatchers = atoi(argv[3]);
	int num_workers = atoi(argv[4]);
	int queue_length = atoi(argv[5]);

	if (queue_length > MAX_QUEUE_SIZE)
	{
		fprintf(stderr, "Error: Queue length cannot exceed %d.\n", MAX_QUEUE_SIZE);
		return -1;
	}

	if(num_dispatchers > MAX_THREADS || num_workers > MAX_THREADS)
	{
		fprintf(stderr, "Error: Number of dispatchers and workers cannot exceed %d.\n", MAX_THREADS);
		return -1;
	}

	int cache_size = 0;

	// If cache size is provided
	if (argc == 7)
	{
		cache_size = atoi(argv[6]);
	}

	if (cache_size > MAX_CACHE_SIZE)
	{
		fprintf(stderr, "Error: Cache size cannot exceed %d.\n", MAX_CACHE_SIZE);
		return -1;
	}
	
	if ((cache = (cache_entry*) malloc(cache_size * sizeof(cache_entry))) == NULL)
	{
		fprintf(stderr, "Error: Failed to allocate cache.\n");
	}
	
	cache_sz = cache_size;

	// Open server logfile
	if ((logfile = fopen("web_server_log", "w+")) == NULL)
	{
		fprintf(stderr, "Error: Failed to create log.\n");
		return -1;
	}

	// Initialize request queue
	task_queue = initqueue(queue_length);

	sem_init(&consumer, 0, 0);
	sem_init(&producer, 0, queue_length);
	
	struct sigaction act;
	sigfillset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = 0;
	
	if (sigaction(SIGINT, &act, NULL) == -1) 
	{
		fprintf(stderr, "Error: Failed setting signal handler.\n");
		return -1;
	}
	

	//Call init() first and make dispatcher and worker threads
	
	init(port_number);	
	fprintf(stderr, "██╗    ██╗███████╗██████╗     ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ \n");
    fprintf(stderr, "██║    ██║██╔════╝██╔══██╗    ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗ \n");
    fprintf(stderr, "██║ █╗ ██║█████╗  ██████╔╝    ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝ \n");
    fprintf(stderr, "██║███╗██║██╔══╝  ██╔══██╗    ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗ \n");
    fprintf(stderr, "╚███╔███╔╝███████╗██████╔╝    ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║ \n");
    fprintf(stderr,  "╚══╝╚══╝ ╚══════╝╚═════╝     ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝ \n");
	
	int i;

	//create dispatcher threads 
	for(i = 0; i < num_dispatchers; i++)
	{
		if(pthread_create(&dispatchers[i], NULL, dispatch, NULL))
		{
			fprintf(stderr, "Error: fail to create dispatcher %d\n", i);
			fclose(logfile);
			return -1;
		}
	}
	
	//create worker threads 	
	for(i = 0; i < num_workers; i++)
	{

		if (pthread_create(&workers[i], NULL, worker, NULL))
		{
			fprintf(stderr, "Error: fail to create worker %d\n", i);
			fclose(logfile);
			return -1;
		}
	}
	
	for (i = 0; i < num_workers; i++)
	{
		pthread_join(workers[i], NULL);
	}
	
	for (i = 0; i < num_dispatchers; i++)
	{
		pthread_join(dispatchers[i], NULL);
	}
	
	fclose(logfile);

	return 0;
}

char* getType(char* filename)
{
	char* result;
	char* html = ".html";
	char* htm = ".htm";
	char* jpg = ".jpg";
	char* gif = ".gif";

	if(strstr(filename, html) != NULL || strstr(filename, htm) != NULL)
		result = "text/html";
	else if (strstr(filename, jpg) != NULL)
		result = "image/jpeg";
	else if (strstr(filename, gif) != NULL)
		result = "image/gif";
	else
		result = "text/plain";
	
	return result;	
}	

char* appendString(char* str1, char* str2)
{
	char* new_str ;
	if ((new_str = malloc(strlen(str1)+strlen(str2)+1)) != NULL)
	{
    	new_str[0] = '\0';   // Make sure the memory is an empty string
    	strcat(new_str,str1);
    	strcat(new_str,str2);
	}	 
	else 
	{
    	fprintf(stderr,"malloc failed!\n");
	}
	return new_str;
}

void push(request_queue* queue, request_queue_t item)
{
  int newElementIndex;
  pthread_mutex_lock(&queue_lock);
  newElementIndex = (queue->front
                    + queue->count)
                    % queue->size;
  queue->contents[newElementIndex] = item;
  queue->count++;
  pthread_mutex_unlock(&queue_lock);
}

request_queue_t* pop(request_queue* queue)
{
  request_queue_t* oldElement;
  pthread_mutex_lock(&queue_lock);
  if (queue->count <= 0) {
    fprintf(stderr, "Pop on empty queue.\n");
  }
  oldElement = &(queue->contents[queue->front]);
  queue->front++;
  queue->front %= queue->size;
  queue->count--;
  pthread_mutex_unlock(&queue_lock);
  return oldElement;
}

request_queue* initqueue(int size)
{
	request_queue* queue = (request_queue*)malloc(sizeof(request_queue));
	queue->count = 0;
	queue->size = size;
	queue->contents =(request_queue_t*)malloc(size*sizeof(request_queue_t));
	return queue;
}

int isEmpty(request_queue* queue)
{
	return queue->count <= 0 ? 1 : 0;
}

double comp_time(struct timeval time_s, struct timeval time_e) 
{

	double elap = 0.0;

	if (time_e.tv_sec > time_s.tv_sec) 
	{
		elap += (time_e.tv_sec - time_s.tv_sec - 1) * 1000000.0;
		elap += time_e.tv_usec + (1000000 - time_s.tv_usec);
	}
	else 
	{
		elap = time_e.tv_usec - time_s.tv_usec;
	}
	return elap;
}

static void handler(int signo)
{
	flockfile(logfile);
	fflush(logfile);
	fclose(logfile);
	funlockfile(logfile);
	fprintf(stderr, "\nExiting web server.\n");
	fprintf(stderr, "████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗    ██╗   ██╗ ██████╗ ██╗   ██╗\n");    
    fprintf(stderr, "╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝    ╚██╗ ██╔╝██╔═══██╗██║   ██║\n");   
    fprintf(stderr, "   ██║   ███████║███████║██╔██╗ ██║█████╔╝      ╚████╔╝ ██║   ██║██║   ██║\n");    
    fprintf(stderr, "   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗       ╚██╔╝  ██║   ██║██║   ██║\n");    
    fprintf(stderr, "   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       ██║   ╚██████╔╝╚██████╔╝\n");    
    fprintf(stderr, "   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       ╚═╝    ╚═════╝  ╚═════╝ \n");    
                                                                              
    fprintf(stderr, "                        ███████╗ ██████╗ ██████╗\n");                                                       
	fprintf(stderr, "                        ██╔════╝██╔═══██╗██╔══██╗\n");                                                      
	fprintf(stderr, "                        █████╗  ██║   ██║██████╔╝\n");                                                      
	fprintf(stderr, "                        ██╔══╝  ██║   ██║██╔══██╗\n");                                                      
	fprintf(stderr, "                        ██║     ╚██████╔╝██║  ██║\n");                                                      
	fprintf(stderr, "                        ╚═╝      ╚═════╝ ╚═╝  ╚═╝\n");                                                      
                                                                              
	fprintf(stderr, "          ██████╗ ██╗      █████╗ ██╗   ██╗██╗███╗   ██╗ ██████╗\n");                         
	fprintf(stderr, "          ██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██║████╗  ██║██╔════╝\n");                         
	fprintf(stderr, "          ██████╔╝██║     ███████║ ╚████╔╝ ██║██╔██╗ ██║██║  ███╗\n");                        
	fprintf(stderr, "          ██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██║██║╚██╗██║██║   ██║\n");                        
	fprintf(stderr, "          ██║     ███████╗██║  ██║   ██║   ██║██║ ╚████║╚██████╔╝\n");                        
	fprintf(stderr, "          ╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝\n");                         
                                                       
	exit(0);
}


