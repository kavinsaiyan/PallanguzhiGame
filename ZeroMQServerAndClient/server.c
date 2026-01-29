#include <unistd.h>
#include <czmq.h>

typedef struct s_Room
{
    char* client1;
    char* client2;
} Room;

Room rooms[1024] = { 0 };
int roomCount = 0;
int clientCount = 0;

void send_msg(zsock_t *responder,char *identity, char *msg)
{
    zstr_sendm(responder,identity);
    zstr_send(responder,msg);
}

void send_msg2(zsock_t *responder,char *identity, char *msg, char *msg2)
{
    zstr_sendm(responder,identity);
    zstr_sendm(responder,msg);
    zstr_send(responder,msg2);
}

char* GetOtherClient(char* identity)
{
    for(int i = 0; i < roomCount; i++)
    {
        if(strcmp(identity,rooms[i].client1) == 0)
            return rooms[i].client2;
        if(strcmp(identity,rooms[i].client2) == 0)
            return rooms[i].client1;
    }
    return NULL;
}

int main (void)
{
    // Use CZMQ's built-in interrupt handling
    zsys_catch_interrupts();
    
    zsock_t *responder = zsock_new_router("tcp://*:5555");
    
    printf("Server listening on port 5555...\n");
    printf("Press Ctrl+C to shutdown gracefully\n");
    
    // Create a poller
    zpoller_t *poller = zpoller_new(responder, NULL);
    
    while (!zsys_interrupted)
    {
        zsock_t *which = (zsock_t *)zpoller_wait(poller, 60);
        
        if (which == NULL) {
            // Timeout or interrupted
            continue;
        }
        
        if (which == responder)
        {
            char *identity = zstr_recv(responder);
            if (!identity) continue;

            char *msg = zstr_recv(responder);
            if (!msg) {
                zstr_free(&identity);
                continue;
            }
            
            printf("Got identity: '%s', msg: '%s'\n", identity, msg);
            
            if(strcmp(msg, "JOIN") == 0)
            {
                if(clientCount % 2 == 0)
                {
                    roomCount++;
                    rooms[roomCount-1].client1 = strdup(identity);
                    send_msg(responder, identity,"SUCCESS");
                }
                else 
                {
                    rooms[roomCount-1].client2 = strdup(identity);
                    send_msg(responder, identity,"START");
                }
                printf("Sending SUCCESS to: %s\n", identity);                
                
                clientCount++;
            }
            else if(strcmp(msg,"RELAY") == 0)
            {
                //need to receive the next message to be relayed
                char *msgToRelay = zstr_recv(responder);
                if(msgToRelay == NULL)
                {
                    zstr_free(&identity);
                    zstr_free(&msg);
                    continue;
                }
                char *idToSendRelayMsg = GetOtherClient(identity);
                if(idToSendRelayMsg != NULL)
                {
					printf("Sending relay message to %s\n",idToSendRelayMsg);
                    send_msg2(responder,idToSendRelayMsg,"RELAY",msgToRelay);
                }
            }
            
            zstr_free(&identity);
            zstr_free(&msg);
        }
    }
    
    // Cleanup
    printf("\nCleaning up resources...\n");
    for(int i = 0; i < roomCount; i++)
    {
        if(rooms[i].client1) free(rooms[i].client1);
        if(rooms[i].client2) free(rooms[i].client2);
    }
    
    zpoller_destroy(&poller);
    zsock_destroy(&responder);
    printf("Server shutdown complete.\n");
    
    return 0;
}
