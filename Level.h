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

	glm::vec2 load(const std::string &filepath);
	const std::vector<Block>& getLevelData() const { return m_levelData; }

private:
	std::vector<Block> m_levelData;
};

