#include "Level.h"
#include <fstream>

Level::Level()
{
}


Level::~Level()
{
}

glm::vec2 Level::load(const std::string &filepath)
{
    std::ifstream file;
    file.open(filepath.c_str(), std::ios_base::in);

    float startx,starty, stopx,stopy;
    if(!file.eof())
        file>>startx>>starty>>stopx>>stopy;

    while (!file.eof()){
        float cornerx,cornery,sizex,sizey;
        file>>cornerx>>cornery>>sizex>>sizey;
        if (cornery < m_deathHeight)
            m_deathHeight = cornery;
        m_levelData.push_back(Block(glm::vec2(cornerx,cornery), glm::vec2(sizex,sizey)));
    }
    m_deathHeight -= 25.f;
    m_finishPoint.pos = glm::vec2(stopx,stopy);
/*    // todo: read finish line ?
	for (int i = 0; i < 500; ++i)
    {
		m_levelData.emplace_back(glm::vec2(100.0f - i*10.0f, 60.0f), glm::vec2(10.0f, 6.0f));
    }*/
    // todo: read blocks from file
    return glm::vec2(startx,starty);
}

bool Level::isBelowLevel(const glm::vec2& point)
{
    return point.y < m_deathHeight;
}
const Block& Level::getFinishPoint()
{
    return m_finishPoint;
}

