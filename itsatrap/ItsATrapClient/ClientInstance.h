#pragma once
#ifndef CLIENTINSTANCE_H
#define CLIENTINSTANCE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <unordered_map>
#include <queue>

#include "SceneGraph.h"

#include "Mesh.h"

class ClientInstance {
	public:
		Mesh test;
		bool loaded;

		unordered_map<int,sg::Player*> players;
		unordered_map<int,sg::Trap*> traps;
		unordered_map<int, sg::Node*> objects;
		sg::Player *root;
		sg::MatrixTransform *hotSpot;

		int m_xMouse, m_yMouse; // mouse position

		ClientInstance();
		ClientInstance(int id);
		~ClientInstance();

		bool addPlayer(sg::Player *p);
		
		bool setCurrentPlayer(int p);
		bool setCurrentPlayer(sg::Player *p);
		void toggleCurrentPlayer();
		
		int getNextPlayer(int p);
		int getNextPlayer();

		bool doesPlayerExist(int p);
		bool doesPlayerExist(sg::Player *p);
		void printPlayers();

		void swapRootNode(int p);
		void swapRootNode(sg::Player *p);

		void printSceneGraph();
};

#endif CLIENTINSTANCE_H
