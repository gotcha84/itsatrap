#ifndef PLAYER_H
#define PLAYER_H

class Player {
  public:
    int				playerId;
    sockaddr_in		clientAddress;
	bool			active;

    Player() {};

    Player(int id, sockaddr_in addr) {
      playerId = id;
      clientAddress = addr;
    };
};

#endif
