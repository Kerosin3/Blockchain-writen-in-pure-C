#include "setup_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlog.h>
#include <threads.h>

const char *client_conf_logfile; // = "/home/ker0/test/prj/sources/logging/zlog.conf";
int client_logging_enabled;
zlog_category_t *client_log;

int main(int argc, char *argv[])
{
    client_conf_logfile = getenv("cblockchain_conf");
    if (!client_conf_logfile)
    {
        printf("no cblockchain_conf env variable has been found\n");
    }

    char logdir[128]={0};
    snprintf(logdir,128,"%s", getenv("HOME"));
    snprintf(logdir+strlen(logdir),128,"%s","/logs");
    struct stat st = {0};
    if (stat(logdir,&st) == -1){
        fprintf(stdout, "no logs folder exists, creating...\n");
	mkdir(logdir,0744);
    }
    client_logging_enabled = 0;

    int rc;

    rc = zlog_init(client_conf_logfile);
    if (rc)
    {
        printf("logging failed\n");
    }

    atexit(zlog_fini);

    client_log = zlog_get_category("logclient");
    if (!client_log)
    {
        printf("failed init logging category!\n");
        zlog_fini();
    }
#if (WRITE_LOG == 1)
    client_logging_enabled = 1;
    zlog_info(client_log, "client logging started!");
#endif
    thrd_t thread_messages_accept; // create threads
    
   int tc_ret = thrd_create(&thread_messages_accept, (thrd_start_t)setup_client_iouring, (void *)0);
   if (tc_ret == thrd_error)
        {
            printf("error while thread creation\n");
            exit(1);
        }
//check thread
	int rez = -1;
        if (thrd_join(thread_messages_accept, &rez) != thrd_success)
        {
            printf("error joining thread \n");
        }
//    setup_client_iouring();

    return 0;
}
