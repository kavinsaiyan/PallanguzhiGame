#include <unistd.h>
#include <czmq.h>

#define MAX_ROOMS 1024

typedef struct s_Room
{
    char* client1;
    int64_t c1_last_seen;
    char* client2;
    int64_t c2_last_seen;
} Room;

Room rooms[MAX_ROOMS] = { 0 };
int roomCount = 0;
int clientCount = 0;

char* RELAY = "RELAY";
char* JOIN = "JOIN";
char* WAIT = "WAIT";
char* DISCONNECT = "DISCONNECT";
char* YOUR_TURN = "YOUR_TURN";
char* WAIT_FOR_TURN = "WAIT_FOR_TURN";
char* ROOMS_FULL = "ROOMS_FULL";
char* HEARTBEAT = "HEARTBEAT";

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
    for(int i = 0; i < MAX_ROOMS; i++)
    {
        if(rooms[i].client1 != NULL && strcmp(identity,rooms[i].client1) == 0)
            return rooms[i].client2;
        if(rooms[i].client2 != NULL && strcmp(identity,rooms[i].client2) == 0)
            return rooms[i].client1;
    }
    return NULL;
}

void RemoveClient(zsock_t *responder, char* identity)
{
	for(int i = 0; i < MAX_ROOMS; i++)
	{
		char* otherIdentity = NULL;
		bool found = false;
        if(rooms[i].client1 != NULL && strcmp(identity,rooms[i].client1) == 0)
        {
			otherIdentity = rooms[i].client2;
			found = true;
		}

        else if (rooms[i].client2 != NULL && strcmp(identity,rooms[i].client2) == 0)
        {
			otherIdentity = rooms[i].client1;
			found = true;
		}

        if(found)
        {
			if(otherIdentity != NULL)
				send_msg(responder,otherIdentity, DISCONNECT);
            printf("Removing room for clients : %s and %s\n",rooms[i].client1,rooms[i].client2);

            if(rooms[i].client1 != NULL)
			{
				clientCount--;
                free(rooms[i].client1);
                rooms[i].client1 = NULL;
            }
            if(rooms[i].client2 != NULL)
            {
				clientCount--;
				free(rooms[i].client2);
				rooms[i].client2 = NULL;
			}
			return;
        }
	}
}

int GetWaitingRoomIndex()
{
    int index = -1;
    for(int i = 0; i < MAX_ROOMS; i++)
    {
        if(rooms[i].client1 != NULL && rooms[i].client2 == NULL)
        {
            index = i;
            break;
        }
    }
    return index;
}

int GetFreeRoomIndex()
{
    int index = -1;
    for(int i = 0; i < MAX_ROOMS; i++)
    {
        if(rooms[i].client1 == NULL && rooms[i].client2 == NULL)
        {
            index = i;
            break;
        }
    }
    return index;
}

void CheckHeartBeatAndRemoveClients(zsock_t* responder, int64_t now)
{
    for(int i=0;i < MAX_ROOMS; i++)
    {
        if(rooms[i].client1 != NULL && (now - rooms[i].c1_last_seen) > 10000)
            RemoveClient(responder,rooms[i].client1);
        else if(rooms[i].client2 != NULL && (now - rooms[i].c2_last_seen) > 10000)
            RemoveClient(responder,rooms[i].client2);
    }
}

void UpdateLastSeen(char* identity, int64_t now)
{
    for(int i=0;i < MAX_ROOMS; i++)
    {
        if(rooms[i].client1 != NULL && strcmp(identity, rooms[i].client1) == 0)
            rooms[i].c1_last_seen = now;
        if(rooms[i].client2 != NULL && strcmp(identity, rooms[i].client2) == 0)
            rooms[i].c2_last_seen = now;
    }
}

int main (void)
{
    zsys_catch_interrupts();
    
    zsock_t *responder = zsock_new_router("tcp://*:5555");
    
    printf("Server listening on port 5555...\n");
    printf("Press Ctrl+C to shutdown gracefully\n");
    
    zpoller_t *poller = zpoller_new(responder, NULL);

    
    while (!zsys_interrupted)
    {
        zsock_t *which = (zsock_t *)zpoller_wait(poller, 0);
        if (which == NULL) {
            // Timeout or interrupted
            continue;
        }
        
        if (which == responder)
        {
            int64_t now = zclock_mono();
            CheckHeartBeatAndRemoveClients(responder,now);

            char *identity = zstr_recv(responder);
            if (!identity || strlen(identity) > 32) continue;

            char *msg = zstr_recv(responder);
            if (!msg || strlen(msg) > 16) {
                zstr_free(&identity);
                continue;
            }
            
            printf("Got identity: '%s', msg: '%s'\n", identity, msg);

            bool updateHeartBeat = false;
            if(strcmp(msg, HEARTBEAT) == 0)
                updateHeartBeat = true;
            
            if(strcmp(msg, JOIN) == 0)
            {
                if(clientCount % 2 == 0)
                {
                    int freeRoomIdx = GetFreeRoomIndex();
                    if(freeRoomIdx == -1)
                        send_msg(responder, identity,ROOMS_FULL);
                    else
                    {
                        rooms[freeRoomIdx].client1 = strdup(identity);
                        send_msg(responder, identity,WAIT);
                        printf("Sending WAIT to: %s who is client no: %d\n", identity, clientCount);
                        updateHeartBeat = true;
                    }
                }
                else 
                {
                    int waitingRoomIdx = GetWaitingRoomIndex();
                    if(waitingRoomIdx == -1)
                        send_msg(responder, identity,ROOMS_FULL);
                    else 
                    {
                        rooms[waitingRoomIdx].client2 = strdup(identity);
                        send_msg(responder, identity, WAIT_FOR_TURN);
                        
                        send_msg(responder, rooms[waitingRoomIdx].client1, YOUR_TURN); 

                        printf("Sending YOUR_TURN to :%s\n", rooms[waitingRoomIdx].client1);
                        printf("Sending WAIT_FOR_TURN to :%s\n", rooms[waitingRoomIdx].client2);

                        updateHeartBeat = true;
                    }
                }
                clientCount++;
            }
            else if(strcmp(msg,RELAY) == 0)
            {
                //need to receive the next message to be relayed
                printf("receiving the relay move index\n");
                char *msgToRelay = zstr_recv(responder);
                if(msgToRelay == NULL || strlen(msgToRelay) > 8)
                {
                    printf("No message to Relay\n");
                    zstr_free(&identity);
                    zstr_free(&msg);
                    continue;
                }

                //sending the received message to other client
                char *idToSendRelayMsg = GetOtherClient(identity);
                if(idToSendRelayMsg != NULL)
                {
					printf("Sending relay message to %s\n",idToSendRelayMsg);
                    send_msg2(responder,idToSendRelayMsg,RELAY,msgToRelay);
                }
                else 
                    printf("couldn't find other client id\n");

                //telling the client that sent relay message to wait
                send_msg(responder,identity, WAIT_FOR_TURN);
                updateHeartBeat = true;
            }
            else if(strcmp(msg, DISCONNECT) == 0)
            {
                printf("received Disconnect message\n");
                RemoveClient(responder,identity);
            }

            if(updateHeartBeat)
                UpdateLastSeen(identity,now);
                
            
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
