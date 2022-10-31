#include "setup_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlog.h>
#include <threads.h>
#include <p2p_setup.h>
#include "p2p_event_loop.h"
#include "common.h"
#include <stdatomic.h>
#include "p2p_listen.h"


const char *client_conf_logfile; // = "/home/ker0/test/prj/sources/logging/zlog.conf";
int client_logging_enabled;
zlog_category_t *client_log;

int p2p_logging_enabled;
zlog_category_t *p2p_log;


struct io_uring ring_p2p;

int main(int argc, char *argv[])
{
	
       if ((argc != 2))  // ip for listen app
    {
        printf("please listening IP for a peer p2p client..\n");
        exit(1);
    }
  
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
    p2p_logging_enabled = 0;
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
        printf("failed init logging category client log!\n");
        zlog_fini();
    }

    p2p_log = zlog_get_category("logp2p");
    if (!p2p_log)
    {
        printf("failed init logging category p2p log!\n");
        zlog_fini();
    }

#if (WRITE_LOG == 1)
    client_logging_enabled = 1;
    p2p_logging_enabled = 1;
    zlog_info(client_log, "client logging started!");
#endif
    //------------------------------------------------
    //setup p2p
	
    setup_buffers();      // establish buffers
    int serv_fd = setup_serv_sock(10001); // set server fd
    printf("p2p server launched at 10001 port\n");
    zlog_info(p2p_log, "p2p server started");
    setup_iouring(&ring_p2p, false);
    kill_thread_p2p = false;
//----------------------------------------------------
    thrd_t thread_messages_accept; // create thread accept message from server
    thrd_t thread_p2p_worker; // create thread p2p server
    thrd_t thread_p2p_list; // create thread listen
    flag_block_created = 0;
    mtx_init(&block_created_mtx, mtx_plain); // init mtx
    mtx_init(&peer_connection_accepted ,mtx_plain);
   //setup and start client accept message thread 
   //------------------------------------------------------------------------------------------------------>>ok
   char* ip_server = "172.16.1.1";
   int tc_ret = thrd_create(&thread_messages_accept, (thrd_start_t)setup_client_iouring, (void *)ip_server);
//    int tc_ret = thrd_create(&thread_messages_accept, (thrd_start_t)setup_client_iouring, (void *)0);
   if (tc_ret == thrd_error)
        {
            printf("error while client accept thread creation\n");
            exit(1);
        }
   event_p2p_params_t* el_params = calloc(1,sizeof(event_p2p_params_t));
   el_params->ring = &ring_p2p;
   el_params->serv_sock=  serv_fd;
   tc_ret = thrd_create(&thread_p2p_worker, (thrd_start_t)event_loop_p2p, (void *)el_params);
   if (tc_ret == thrd_error)
        {
            printf("error while thread p2p creation\n");
            exit(1);
        }
   thrd_detach(thread_p2p_worker); //DETACH!
//     setup_p2p_listening();
    char* _ip = "127.0.0.1" ;
    tc_ret = thrd_create(&thread_p2p_list, (thrd_start_t)setup_p2p_listening, (void *)argv[1]);
//------------------------------------------------------------------------------------------------------>>ok
   tc_ret = thrd_create(&thread_p2p_list, (thrd_start_t)setup_p2p_listening, (void *)argv[1]);
   if (tc_ret == thrd_error)
     {
	 printf("error while thread p2p creation\n");
	 exit(1);
     }
   thrd_detach(thread_p2p_list); // dETACH!!

//join client accept thread
	int rez = -1;
        if (thrd_join(thread_messages_accept, &rez) != thrd_success)
        {
            printf("error joining thread \n");
        }
  
//    setup_client_iouring();
   destroy_buffers();
   teardown_server_sock(serv_fd);
   close(serv_fd);
   io_uring_queue_exit(&ring_p2p);
    zlog_info(p2p_log, "P2P APPLICATION FINISHED");
    free(el_params);
    return 0;
}
