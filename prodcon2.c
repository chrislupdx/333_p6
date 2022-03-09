//this is prodcon2.c
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct {
    char * buffer;
    char * text;
    int count;
} myargs_t;
pthread_mutex_t producer_mutex; 
//pthread_mutex_t consumer_mutex; 
pthread_cond_t  fill = PTHREAD_COND_INITIALIZER; //DOES THIS HAVE A DONE VAR INSDIE IT
pthread_cond_t  empty = PTHREAD_COND_INITIALIZER; //DOES THIS HAVE A DONE VAR INSDIE IT
//figure out the states we can toggle in cond_t

void *producer_f(void * arg);
void *consumer_f(void * arg);

int main()
{
    int count = 10;
    //int rc = pthread_mutex_init(&consumer_mutex, NULL);
    int rc2 = pthread_mutex_init(&producer_mutex, NULL);
    char * buffer = (char *)malloc(256 * sizeof(char));  //this is what we're going to pass back and forth between producer_f and consumer_f
    char * input = (char *)malloc(256 * sizeof(char));  //this is used to 
    pthread_t producer[count];
    pthread_t consumer[count];

    myargs_t args;
    args.buffer = buffer;
    args.text = input;
    args.count = 0;

    for(int i = 0; i < count; i++)
    {
        pthread_create(&producer[i], NULL, producer_f, &args);
        //signal to the consumer when the buffer is ready
    
        //force consumer to wait until buffer is filled?
        pthread_create(&consumer[i], NULL, consumer_f, &args);
    }
    for(int i = 0; i < count; i++)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
    }
    return 1; 
}
void *producer_f(void * arg)
{
    pthread_mutex_lock(&producer_mutex);
    myargs_t *args = (myargs_t *) arg;
    //while the buffer is full, wait.
    while(strlen(args->buffer) > 0)
    {
        //printf("inside while arg->buffer %s\n", args->buffer);
        pthread_cond_wait(&empty, &producer_mutex);
    }
    FILE * pFile;
    char fform[50] = "txts/in"; //this is  missing the # that we may iterate through
    char curr[50];
    snprintf(curr, 10, "%d", args->count);
    strcat(fform, curr);
    pFile = fopen (fform, "r");
    if(!pFile)
    {
        perror("Error opening file");
        return NULL;
    }
    if(fgets(args->text, 256, pFile) != NULL)
    {
        strncpy(args->buffer, args->text, 256); 
        args->buffer[256] = '\0';
    }
    (args->count)++;
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&producer_mutex);
    return NULL;
}

void *consumer_f(void * arg)
{
    pthread_mutex_lock(&producer_mutex);
    myargs_t * args = (myargs_t *) arg;
    //while the buffer is empty, wait
    while(strlen(args->buffer) == 0) 
    {
        pthread_cond_wait(&fill, &producer_mutex);
    }
    printf("%s \n", args->buffer);
    args->buffer[0] = 0;
    pthread_cond_signal(&empty); //signals buffer is empty+ready
    pthread_mutex_unlock(&producer_mutex);
    return NULL;
}
