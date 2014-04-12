#ifndef PLAYER_H
#define PLAYER_H

#include <netinet/in.h>

class Player {
  public:
    int playerId;
    sockaddr_in clientAddress;

    Player() {};

    Player(int id, sockaddr_in addr) {
      playerId = id;
      clientAddress = addr;
    };
};

#endif
