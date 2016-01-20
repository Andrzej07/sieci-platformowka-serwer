#include "Level.h"
#include "SpriteBatch.h"
#include <fstream>

Level::Level()
{
}


Level::~Level()
{
}

glm::vec2 Level::load(const std::string &filepath)
{
	m_texture = ResourceManager::getTexture("Assets/dude.png");
    std::ifstream file;
    file.open(filepath, std::ios_base::in);

    float startx,starty, stopx,stopy;
    file>>startx>>starty>>stopx>>stopy;
    while (!file.eof()){
        float cornerx,cornery,sizex,sizey;
        file>>cornerx>>cornery>>sizex>>sizey;
        m_levelData.emplace_back(glm::vec2(cornerx,cornery), glm::vec2(sizex,sizey));
    }
/*    // todo: read finish line ?
	for (int i = 0; i < 500; ++i)
    {
		m_levelData.emplace_back(glm::vec2(100.0f - i*10.0f, 60.0f), glm::vec2(10.0f, 6.0f));
    }*/
    // todo: read blocks from file
    return glm::vec2(startx,starty);
}

void Level::draw(SpriteBatch& spriteBatch)
{
	for (auto& block : m_levelData)
	{
		spriteBatch.draw(glm::vec4(block.pos.x, block.pos.y, block.size.x, block.size.y), glm::vec4(0,0,1,1), m_texture.id, 0, m_color);
	}

}
