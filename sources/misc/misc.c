#include "misc.h"
#include <stdlib.h>
#include <time.h>

_Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
/*
 * for debugging
 * */
void DumpHex(const void *data, size_t size)
{
    char ascii[17];
    size_t i, j;
    ascii[16] = '\0';
    for (i = 0; i < size; ++i)
    {
        printf("%02X ", ((unsigned char *)data)[i]);
        if (((unsigned char *)data)[i] >= ' ' && ((unsigned char *)data)[i] <= '~')
        {
            ascii[i % 16] = ((unsigned char *)data)[i];
        }
        else
        {
            ascii[i % 16] = '.';
        }
        if ((i + 1) % 8 == 0 || i + 1 == size)
        {
            printf(" ");
            if ((i + 1) % 16 == 0)
            {
                printf("|  %s \n", ascii);
            }
            else if (i + 1 == size)
            {
                ascii[(i + 1) % 16] = '\0';
                if ((i + 1) % 16 <= 8)
                {
                    printf(" ");
                }
                for (j = (i + 1) % 16; j < 16; ++j)
                {
                    printf("   ");
                }
                printf("|  %s \n", ascii);
            }
        }
    }
}

test_msg_t get_test_msg(size_t len){
    
//    srand((long)time(0)); // define a seed for the random number generator
    const char ALLOWED[] = "abcdefghijklmnopqrstuvwxyz1234567890";
    unsigned char* random = calloc(len,sizeof(char));
    int i = 0;
    int c = 0;
    int nbAllowed = sizeof(ALLOWED) - 1;
        for (i = 0; i < len; i++)
        {
            c = rand() % nbAllowed;
            random[i] = ALLOWED[c];
        }
   test_msg_t msg;
   msg.test_msg = random;
   msg.len = len;
   return msg; 
}

