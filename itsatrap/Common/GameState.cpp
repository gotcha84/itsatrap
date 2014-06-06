#include "GameState.h"

GameStates	GameState::m_gameState;

GameState::GameState() {
	m_gameState = WELCOME;
}

GameState::~GameState() {

}

void GameState::setWelcome() {
	m_gameState = WELCOME;
}

void GameState::setGameReady() {
	m_gameState = GAMEREADY;
}

void GameState::setGameplay() {
	m_gameState = GAMEPLAY;
}

void GameState::setGameover() {
	m_gameState = GAMEOVER;
}

GameStates GameState::getState() {
	return m_gameState;
}