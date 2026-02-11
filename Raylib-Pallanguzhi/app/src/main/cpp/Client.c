#include "Client.h"
#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

static zsock_t* requester = NULL;

const char* JOIN = "JOIN";
const char* START = "START";
const char* RELAY = "RELAY";
const char* WAIT = "WAIT";

void connect_to_server(void)
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
    //zsock_connect(requester, "tcp://140.245.225.75:5555");
    zsock_connect(requester, "tcp://127.0.0.1:5555");
    
    // Send message
    zstr_send(requester, JOIN);
    printf("message sent\n");

    zsock_set_rcvtimeo(requester, 1);
}

bool try_receive_reply(float deltaTime, Message* msg, int* otherPlayerMoveIndex)
{
    char *reply = zstr_recv(requester);
    *msg = NoMessage;
    *otherPlayerMoveIndex = -1;
    if(reply != NULL)
    {
        printf("reply: %s\n", reply); // only prints in PC, not in android

        if(strcmp(reply,WAIT)==0)
        {
            //return the status for the clients which should be an enum
            //there should be differentiation between waiting for other player to join and waiting for the connected other player's move
            *msg = Wait;
        }
        else if(strcmp(reply,START)==0)
        {
            *msg = Start;
        }
        else if(strcmp(reply,RELAY)==0)
        {
            *msg = Relay;
             char *otherPlayerMove = zstr_recv(requester);
             if(otherPlayerMove != NULL)
                 *otherPlayerMoveIndex = atoi(otherPlayerMove);     
             zstr_free(&otherPlayerMove);
        }
        return true;
    }

    if(reply != NULL)
        zstr_free(&reply);
    return false;
}

void send_move_to_server(int currentIndex)
{
    zstr_sendm(requester, RELAY);
    char str[4];
    snprintf(str,sizeof(str),"%d",currentIndex);
    zstr_send(requester, str);
}

void close_connection(void)
{
    zsock_destroy(&requester);
}
