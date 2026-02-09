#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

const char* RELAY = "RELAY";
const char* JOIN = "JOIN";
const char* WAIT = "WAIT";
const char* MOVE = "MOVE";

void waitAndSendRelay(zsock_t *dealer, char* msg)
{
    sleep(10);
    zstr_sendm(dealer,RELAY);
    zstr_send(dealer,msg);
}

int main (void)
{
    zsys_catch_interrupts();

    printf ("Connecting to relay server...\n");
    
    char identity[16];
    srand((unsigned) time(NULL));

    snprintf(identity, 16, "ZMQ%d", rand());
    printf("My identity: %s\n", identity);
    
    // Create socket WITHOUT connecting
    zsock_t* requester = zsock_new(ZMQ_DEALER);

    // Set identity BEFORE connecting
    zsock_set_identity(requester, identity);
    
    // Now connect
    zsock_connect(requester, "tcp://localhost:5555");
    
    // Send message
    zstr_send(requester, "JOIN");
    printf("message sent\n");

    char *reply;
    //reply = zstr_recv(requester);
    //printf("reply: %s\n", reply);
	//if(strcmp(reply,MOVE)==0)
	//{
	//	zstr_sendm(requester,RELAY);
	//	zstr_send(requester,"hoi!");
	//	printf("Relay message Sent to server!\n");
	//}
	//zstr_free(&reply);

    zsock_set_rcvtimeo(requester, 50);
    while(!zsys_interrupted)
    {
        reply = zstr_recv(requester);

        if(reply == NULL)
            continue;

        if(strcmp(reply, MOVE)==0)
        {
            //send relay move message
            printf("sending current player move to server\n");
            waitAndSendRelay(requester,"2");
        }

        if(strcmp(reply,WAIT)==0)
        {
            printf("Waiting for other player's move\n");
        }
    
        if(strcmp(reply,RELAY)==0)
        {
            char *msgFromOtherClient = zstr_recv(requester);
            if(msgFromOtherClient == NULL)
            {
                zstr_free(&reply);
                continue;
            }
            printf("Relay message was received from server!\n");
            printf("message received from other client is %s\n",msgFromOtherClient);
            zstr_free(&msgFromOtherClient);
            char index[4];
            snprintf(index,4,"%d",rand());
            waitAndSendRelay(requester,index);
        }
         
        zstr_free(&reply);
    }

    zsock_destroy(&requester);
    return 0;
}
