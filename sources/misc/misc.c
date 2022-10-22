#include "misc.h"
#include <sodium/crypto_sign.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

_Noreturn void die(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
/*
 * return a allocated memory with signed message
 *
 */
signed_message_t* get_a_signed_msg(user_keys keys){
	srand(time(NULL));
	test_msg_t somemsg = get_test_msg(100);	
	signed_message_t* a_msg = calloc(1,sizeof(signed_message_t));	
	*a_msg = sign_a_message(somemsg, keys.sk);
	//memory for somemsg free here

	put_a_PK(a_msg,keys.pk);
//	a_msg->length = 100;
	return a_msg;
}
signed_message_t* ls_get_a_signed_msg(user_keys keys){
	srand(time(NULL));
	test_msg_t somemsg = ls_get_test_msg(100);	
	signed_message_t* a_msg = calloc(1,sizeof(signed_message_t));	
	*a_msg = sign_a_message(somemsg,keys.sk);
	put_a_PK(a_msg,keys.pk);
//	a_msg->length = 100;
	return a_msg;
}


size_t get_timestamp(void* buffer){
	char date[32];
        time_t t = time(NULL);
    	struct tm* tm = gmtime(&t);
    	strftime(date, sizeof(date), "%d/%m/%Y %H:%M:%S GMT", tm);
	int n = snprintf(buffer, strlen(date)+1, "%s", date);
	return n;
}

u_int64_t  get_epoch_ns(){
	long int ns;
	u_int64_t all;
	time_t sec;
	unsigned long one_bill = 1000000000L;
	struct timespec spec;
	
	if (clock_gettime(CLOCK_REALTIME,&spec)!=0){
		all = 0;
		return all;
	}
	sec = spec.tv_sec;
	ns = spec.tv_nsec;
	all = (u_int64_t) sec *  one_bill + (u_int64_t) ns;
	return all;
}

long long get_date_usec_rec(){

	struct timeval ts;
	if (gettimeofday(&ts,NULL)) {
		long long ret = 0;
		return ret;
	}
	long long ret = ts.tv_sec*1000LL + ts.tv_usec/1000;
	return ret;
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
    
    srand((long)time(0)); // define a seed for the random number generator
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

test_msg_t ls_get_test_msg(size_t len){
    unsigned char* random = calloc(len,sizeof(char));
    if (sodium_init() < 0 ) die("libsodium!"); 
    randombytes_buf(random, len);
   test_msg_t msg;
   msg.test_msg = random;
   msg.len = len;
   return msg; 

}


