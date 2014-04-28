#ifndef PLAYER_H
#define PLAYER_H

class Player {
  public:
    int				playerId, yPosition;
    sockaddr_in		clientAddress;

    Player() {};

    Player(int id, sockaddr_in addr) {
      playerId = id;
      clientAddress = addr;
    };
};

#endif
