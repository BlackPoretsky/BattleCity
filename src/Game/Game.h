#pragma once

#include <array>
#include <glm/vec2.hpp>
#include <memory>

class Tank;
class Level;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void render();
	void update(const uint64_t duration);
	void setKey(const int key,  const int action);
	bool init();

private:
	std::array<bool, 349> m_keys;

	enum class EGameState { Active, Pause};

	EGameState m_eCurrentGameState;

	glm::ivec2 m_windowSize;

	std::unique_ptr<Tank> m_pTank;
	std::unique_ptr<Level> m_pLevel;
};