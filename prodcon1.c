//this is prodcon1.c
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    char * buffer;
    char * text;
} myargs_t;

pthread_mutex_t mutex; //this is gonna need a better name
void *producer_f(void * arg);
void *consumer_f(void * arg);

int main()
{
    int count = 10;
    myargs_t args;

    //allocate a 256 char buffer
    char buffer[256];
    char input[256];

    //for each text, make a custom struct?
    //put eeach text into a 2
    int txtcount = 4; 
    for(int i = 0; i < txtcount; i++)
    {
        FILE * pFile;

        char fform[50] = "txts/in"; //this is  missing the # that we may iterate through
        char curr[50];
        snprintf(curr, 10, "%d", i);
        
        strcat(fform, curr);
        //printf("fform is |%s|\n", fform);
        
        pFile = fopen (fform, "r");
        //figure out how to dynamically read all ten files 
        if(!pFile)
        {
            perror("Error opening file");
            return(-1);
        }
        if( fgets(input, 256, pFile) != NULL)
        {
            printf("printing: %s\n", input);
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
