#include "ClientInstance.h"

ClientInstance::ClientInstance() {
	cout << "DEFAULT CTOR - DO NOT USE" << endl;
}

ClientInstance::ClientInstance(int id) {
	m_xMouse = 0;
	m_yMouse = 0;

	players = unordered_map<int,sg::Player*>();
	objects = unordered_map<int,sg::Node*>();

	//root = new sg::Player(glm::vec3(75.0f,0.0f,0.0f));
	root = new sg::Player(glm::vec3(0.0f,0.0f,0.0f));
	root->setPlayerID(id);
	root->setObjectID(id);

	this->addPlayer(root);
	objects[id] = root;

	tabPressed = false;
}

ClientInstance::~ClientInstance() {
	delete root;
	root = nullptr;
}

bool ClientInstance::addPlayer(sg::Player *p) {
	if (players.size() == 0) {
		players[p->getPlayerID()] = p;
		return true;
	}
	else if (!doesPlayerExist(p)) {
		players[p->getPlayerID()] = p;
		root->addChild(p);
		return true;
	}

	cout << "ERROR : cannot add p" << p->getPlayerID() << "(already exists)" << endl;
	return false;
}

bool ClientInstance::setCurrentPlayer(int p) {
	// try to find player with player id = p
	unordered_map<int,sg::Player*>::iterator it = players.find(p);
	
	// if not found, return false
	if (it == players.end()) {
		cout << "ERROR : cannot set current player to p" << p << "(does not exist)" << endl;
		return false;
	}

	// else set root to the chosen player
	swapRootNode(it->first);
	return true;
}

bool ClientInstance::setCurrentPlayer(sg::Player *p) {
	return setCurrentPlayer(p->getPlayerID());
}

void ClientInstance::toggleCurrentPlayer() {
	int nextPlayer = getNextPlayer();

	if (nextPlayer > 0) {
		swapRootNode(nextPlayer);
	}
}

int ClientInstance::getNextPlayer(int p) {
	// if only 1 player, return 0
	if (players.size() == 1) {
		cout << "ERROR : cannot get next player (only 1 player in world)" << endl;
		return 0;
	}

	// find player with next highest player id
	int nextPlayer = INT_MAX;
	for (unordered_map<int,sg::Player*>::iterator it = players.begin(); it != players.end(); it++) {
		if (it->first > p && it->first < nextPlayer) {
			nextPlayer = it->first;
		}
	}

	if (nextPlayer < INT_MAX && nextPlayer > p) {
		return nextPlayer;
	}
	else { // else find first player (the one with the lowest player id)
		int lowest = p;

		for (unordered_map<int,sg::Player*>::iterator it = players.begin(); it != players.end(); it++) {
			if (it->first < lowest && it->first > 0) {
				lowest = it->first;
			}
		}

		if (lowest < p) {
			return lowest;
		}
		else { // return error if alg fails
			cout << "ERROR : algorithm error in ClientInstance::getNextPlayer(" << p << ")" << endl;
			return -1;
		}
	}
}

int ClientInstance::getNextPlayer() {
	return getNextPlayer(root->getPlayerID());
}

bool ClientInstance::doesPlayerExist(int p) {
	// try to find player with player id = p
	unordered_map<int,sg::Player*>::iterator it = players.find(p);
	
	// if not found, return false
	if (it == players.end()) {
		return false;
	}

	return true;
}

bool ClientInstance::doesPlayerExist(sg::Player *p) {
	return doesPlayerExist(p->getPlayerID());
}

void ClientInstance::printPlayers() {
	for (unordered_map<int,sg::Player*>::iterator it = players.begin(); it != players.end(); it++) {
		cout << "p" << it->first << " : " << glm::to_string(it->second->getPosition()) << endl;
	}
}

void ClientInstance::swapRootNode(int p) {
	sg::Player *old_root = root;
	sg::Player *new_root = players[p];

	sg::Player *new_root_parent = dynamic_cast<sg::Player*>(new_root->getParent());
	new_root_parent->removeChild(new_root);

	vector<sg::Node*> children = new_root_parent->getChildren();
	for (vector<sg::Node*>::iterator it = children.begin(); it != children.end(); it++) {
		new_root_parent->removeChild(*it);
		new_root->addChild(*it);
	}

	root = new_root;
	root->addChild(old_root);
}

void ClientInstance::swapRootNode(sg::Player *p) {
	swapRootNode(p->getPlayerID());
}

void ClientInstance::printSceneGraph() {
	queue<sg::Node*> nodeQ;
	queue<int> sizeQ;

	nodeQ.push(root);
	sizeQ.push(1);
	cout << "[";

	int currSize = sizeQ.front();

	while (sizeQ.size()) {
		if (currSize == 0) {
			cout << "]" << endl;
			sizeQ.pop();
			if (sizeQ.size()) {
				currSize = sizeQ.front();
				cout << "[";
			}
		}
		else {
			nodeQ.front()->print();
			
			sg::Group *group = dynamic_cast<sg::Group*>(nodeQ.front());
			nodeQ.pop();
			currSize--;
			if (group != nullptr) {
				sizeQ.push(group->getNumChildren());
				for (int i=0; i<group->getNumChildren(); i++) {
					nodeQ.push(group->getChild(i));
				}
			}
		}
	}
}
