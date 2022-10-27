#ifndef P2PLISTEN

#define P2PLISTEN

#include "p2p_settings.h"
#include <dirent.h>
#include <fcntl.h>
#include <liburing.h>
#include "arpa/inet.h"
#include <linux/limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <threads.h>

void setup_p2p_listening(char*);


#endif
