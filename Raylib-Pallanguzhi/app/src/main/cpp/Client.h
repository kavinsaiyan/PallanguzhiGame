#include <stdbool.h>

#ifndef CLIENT_H
#define CLIENT_H

typedef enum 
{
    NoMessage,
    Wait,
    FirstTurn,
    SecondTurn,
    Relay
} Message;

void connect_to_server(void);
bool try_receive_reply(float deltaTime, Message *msg, int *otherPlayerMoveIndex);
void send_move_to_server(int currentIndex);
void close_connection(void);

#endif
