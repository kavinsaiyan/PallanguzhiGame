#include "Client.h"
#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

static float timer = 0.0f;
static zsock_t* requester = NULL;

void test_connect(void)
{
    printf ("Connecting to relay server...\n");
    
    char identity[16];
    srand((unsigned) time(NULL));

    snprintf(identity, 16, "ZMQ%d", rand());
    printf("My identity: %s\n", identity);
    
    // Create socket WITHOUT connecting
    requester = zsock_new(ZMQ_DEALER);

    // Set identity BEFORE connecting
    zsock_set_identity(requester, identity);

    // Now connect
    zsock_connect(requester, "tcp://140.245.225.75:5555");
    
    // Send message
    zstr_send(requester, "JOIN");
    printf("message sent\n");

    zsock_set_rcvtimeo(requester, 1);
}

bool try_receive_reply(float deltaTime)
{
    timer += deltaTime;
    if(timer < 1.0f)
        return false;
    timer = 0.0f;

    char* reply = zstr_recv(requester);
    if(reply != NULL && strcmp(reply,"START")==0)
    {
        printf("reply: %s\n", reply);
        return true;
    }

    if(reply != NULL)
        zstr_free(&reply);
    return false;
}

void close_connection(void)
{
    zsock_destroy(&requester);
}
