//this is prodcon3.c
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
typedef struct {
    char * buffer;
    char * text;
    int count;
} myargs_t;
void *producer_f(void * arg);
void *consumer_f(void * arg);
sem_t empty;
sem_t full;
sem_t mutex;
int main()
{
    int count = 10;
    char * buffer = (char *)malloc(256 * sizeof(char));  //this is what we're going to pass back and forth between producer_f and consumer_f
    char * input = (char *)malloc(256 * sizeof(char));  //this is used to 
    pthread_t producer[count];
    pthread_t consumer[count];
    sem_init(&empty, 0, 1); //what hsould we init empty to
    sem_init(&full, 0, 0); //what should we init full to
    sem_init(&mutex, 0, 1);
    myargs_t args;
    args.buffer = buffer;
    args.text = input;
    args.count = 0;

    for(int i = 0; i < count; i++)
    {
        pthread_create(&producer[i], NULL, producer_f, &args);
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
    sem_wait(&empty);
    sem_wait(&mutex);
    myargs_t *args = (myargs_t *) arg;
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
    sem_post(&mutex);
    sem_post(&full);

    return NULL;
}
void *consumer_f(void * arg)
{
    sem_wait(&full);
    sem_wait(&mutex);
    myargs_t * args = (myargs_t *) arg;
    printf("%s \n", args->buffer);
    args->buffer[0] = 0;
    sem_post(&mutex);
    sem_post(&empty);
    return NULL;
}
