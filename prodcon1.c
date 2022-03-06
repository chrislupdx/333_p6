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

pthread_mutex_t mutex; //this is gonna need a better name
void *producer_f(void * arg);
void *consumer_f(void * arg);

int main()
{
    int count = 10;

    char * buffer[256]; //we might need to make this a pointer
    char input[256]; 

    //WHERE DO WE PUT THIS into producer_f?
    int txtcount = 10;

    pthread_t producer[count];
    pthread_t consumer[count];

    myargs_t args = { *buffer, input};  //will it work in this scope?
    args.count = 0;
    for(int i = 0; i < count; i++)
    {
        pthread_create(&producer[i], NULL, producer_f, &args); //perhaps args_struct instead of a single string buffer?
    }

    //what order do we wait for these
    for(int i = 0; i < count; i++)
    {
        pthread_join(producer[i], NULL);
    }
    return 1; 
}

//ID THE race condition
//copies a text into the buffer, it should add a termination char at the end of the text
void *producer_f(void * arg)
{
    pthread_mutex_lock(&mutex);
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
        //printf("printing file %d: %s\n\n", args->count, args->text);
        printf("args->count is %d: text is: %s\n\n", args->count, args->text);
        //write the contents into buffer
        //can we just strcpy(args.buffer, input)
    }
    //pthread_mutex_unlock(&mutex);
    //read the contents from the file up to 256 chars
    //write the contents into the bufffer
    //write a string termination char to the end of buffer
    (args->count)++;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

//should read the buffer, print the contents to stdout, then exit
void *consumer_f(void * arg)
{
    //prints the buffer out
    //then clears it?
    return NULL;
}
