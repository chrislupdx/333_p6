//this is prodcon1.c
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    char * buffer;
    char * text;
    //ordinal_count?
} myargs_t;

pthread_mutex_t mutex; //this is gonna need a better name
void *producer_f(void * arg);
void *consumer_f(void * arg);

int main()
{
    int count = 10;
    myargs_t args;

    char buffer[256];
    char input[256]; 

    //WHERE DO WE PUT THIS into producer_f?
    int txtcount = 10;
    for(int i = 0; i < txtcount; i++)
    {
        FILE * pFile;
        char fform[50] = "txts/in"; //this is  missing the # that we may iterate through
        char curr[50];
        snprintf(curr, 10, "%d", i);
        strcat(fform, curr);
        pFile = fopen (fform, "r");
        if(!pFile)
        {
            perror("Error opening file");
            return(-1);
        }
        if( fgets(input, 256, pFile) != NULL)
        {
            printf("printing file %d: %s\n", i, input);
        }
    }


    pthread_t producer[count];
    pthread_t consumer[count];

    //we need to structure our solution such that there's mutual exclusion on the buffer 
    for(int i = 0; i < count; i++)
    {
        //what are we putting where exactly?
        pthread_create(&producer[i], NULL, producer_f, &args); //perhaps args_struct instead of a single string buffer?
        pthread_create(&producer[i], NULL, consumer_f, &args);
    }

    //i presume there is an ORDER to how we want this to go
    // write new data into the buffer
    // read freshly written data out of the buffer

    //what order do we wait for these
    for(int i = 0; i < count; i++)
    {
        pthread_join(producer[i], NULL);
        pthread_join(consumer[i], NULL);
    }
    return 1; 
}

//copies a text into the buffer, it should add a termination char at the end of the text
void *producer_f(void * arg)
{
    return NULL;
}

//should read the buffer, print the contents to stdout, then exit
void *consumer_f(void * arg)
{
    return NULL;
}
