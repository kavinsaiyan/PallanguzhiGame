#include "Client.h"
#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef SERVER_URL
#define SERVER_URL "tcp://127.0.0.1:5555"
#endif

static zsock_t* requester = NULL;

const char* JOIN = "JOIN";
const char* YOUR_TURN = "YOUR_TURN";
const char* WAIT_FOR_TURN = "WAIT_FOR_TURN";
const char* RELAY = "RELAY";
const char* WAIT = "WAIT";
const char* DISCONNECT = "DISCONNECT";

void free_socket()
{
    if(requester != NULL)
        zsock_destroy(&requester);
    requester = NULL;   
}

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
    zsock_connect(requester, SERVER_URL);
    
    // Send message
    zstr_send(requester, JOIN);
    printf("message sent\n");

    zsock_set_rcvtimeo(requester, 0);
}

bool try_receive_reply(float deltaTime, Message* msg, int* otherPlayerMoveIndex)
{
    *msg = NoMessage;
    *otherPlayerMoveIndex = -1;

    if(requester == NULL)
        return false;

    char *reply = zstr_recv(requester);
    bool res = false;
    if(reply != NULL)
    {
        printf("reply: %s\n", reply); // only prints in PC, not in android

        if(strcmp(reply,WAIT)==0)
        {
            *msg = Wait;
        }
        else if(strcmp(reply,YOUR_TURN)==0)
        {
            *msg = YourTurnMsg;
        }
        else if(strcmp(reply,WAIT_FOR_TURN)==0)
        {
            *msg = WaitForTurn;
        }
        else if(strcmp(reply,RELAY)==0)
        {
            *msg = Relay;
            char *otherPlayerMove = zstr_recv(requester);
            if(otherPlayerMove != NULL)
                *otherPlayerMoveIndex = atoi(otherPlayerMove);     
            zstr_free(&otherPlayerMove);
        }
        else if(strcmp(reply,DISCONNECT)==0)
        {
            printf("Handle the disconnect!\n");
            *msg = Disconnect;
            free_socket();
        }
        res = true;
    }

    if(reply != NULL)
        zstr_free(&reply);
    return res;
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
    if(requester)
    {
        printf("sendind the disconnect message to server!\n");
        zstr_send(requester, DISCONNECT);
    }
    free_socket();
}
