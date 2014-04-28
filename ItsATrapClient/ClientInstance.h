#pragma once
#ifndef CLIENTINSTANCE_H
#define CLIENTINSTANCE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <unordered_map>
#include <queue>

#include "SceneGraph.h"

class ClientInstance {
	public:
		unordered_map<int,sg::Player*> players;
		unordered_map<int, sg::Node*> objects;
		sg::Player *root;

		int m_xMouse, m_yMouse; // mouse position
		float m_xAngleChange, m_yAngleChange;
		float m_xAngleChangeFactor, m_yAngleChangeFactor;

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
