//this is prodcon1.c
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
pthread_mutex_t consumer_mutex; 
void *producer_f(void * arg);
void *consumer_f(void * arg);

int main()
{
    int count = 10;
    int rc = pthread_mutex_init(&consumer_mutex, NULL);
    int rc2 = pthread_mutex_init(&producer_mutex, NULL);
    char * buffer = (char *)malloc(256 * sizeof(char));  //this is what we're going to pass back and forth between producer_f and consumer_f
    char * input = (char *)malloc(256 * sizeof(char));  //this is used to 
    pthread_t producer[count];
    pthread_t consumer[count];

    myargs_t args;
    args.buffer = buffer;
    args.text = input; //what is this for
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
    return NULL;
}
void *consumer_f(void * arg)
{
    myargs_t * args = (myargs_t *) arg;
    //printf("count is %d buffer is %s \n", args->count, args->buffer);
    printf("%s \n", args->buffer);
    return NULL;
}
