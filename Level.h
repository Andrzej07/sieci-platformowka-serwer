#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class SpriteBatch;
struct Block
{
	Block(glm::vec2 pos, glm::vec2 size) :
		pos(pos), size(size) {}
	glm::vec2 pos;
	glm::vec2 size;
	// rotation? etc.
};

class Level
{
public:
	Level();
	~Level();
    bool isBelowLevel(const glm::vec2& point);
    const Block& getFinishPoint();

	glm::vec2 load(const std::string &filepath);
	const std::vector<Block>& getLevelData() const { return m_levelData; }

private:
	std::vector<Block> m_levelData;

    Block m_finishPoint = Block(glm::vec2(0), glm::vec2(4.f));
    float m_deathHeight = 0.0f;
};

