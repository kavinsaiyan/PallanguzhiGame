#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

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

    char* reply = zstr_recv(requester);
    printf("reply: %s\n", reply);
	if(strcmp(reply,"START")==0)
	{
		zstr_send(requester,"RELAY");
		printf("Relay message Sent to server!\n");
	}
	zstr_free(&reply);

    zsock_set_rcvtimeo(requester, 50);
    while(!zsys_interrupted)
    {
        reply = zstr_recv(requester);

        if(reply == NULL)
            continue;
    
        if(strcmp(reply,"RELAY")==0)
        {
            printf("Relay message was received from server!\n");
        }
         
        zstr_free(&reply);
    }

    zsock_destroy(&requester);
    return 0;
}
