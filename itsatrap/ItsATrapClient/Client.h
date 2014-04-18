#ifndef CLIENT_H
#define CLIENT_H

int initializeClient();
void startReceiverThread();
void sendStateUpdate(int id, float x, float y, float z);

#endif CLIENT_H