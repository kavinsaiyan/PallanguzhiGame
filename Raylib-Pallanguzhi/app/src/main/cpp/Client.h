#include <stdbool.h>

#ifndef CLIENT_H
#define CLIENT_H

void test_connect(void);
bool try_receive_reply(float deltaTime);
void close_connection(void);

#endif
