#include "Player.h"
#include "Level.h"
#include <SDL2/SDL_keycode.h>
Player::Player()
{
}


Player::~Player()
{
}

void Player::init(const glm::vec2 &pos, const glm::vec2 &size, const std::string &texturePath)
{
	m_pos = pos;
	m_size = size;
}

bool Player::intersects(const Block& block)
{
	return ((hitsTopOf(block) || hitsBottomOf(block)) && (hitsLeftOf(block) || hitsRightOf(block)));
}
bool Player::hitsRightOf(const Block& block)
{
	return (block.pos.x < m_pos.x) && (block.pos.x + block.size.x > m_pos.x);
}
bool Player::hitsLeftOf(const Block& block)
{
	return (m_pos.x < block.pos.x) && (m_pos.x + m_size.x > block.pos.x);
}
bool Player::hitsTopOf(const Block& block)
{
	return (block.pos.y <= m_pos.y) && (block.pos.y + block.size.y > m_pos.y);
}
bool Player::hitsBottomOf(const Block& block)
{
	return (m_pos.y < block.pos.y) && (m_pos.y + m_size.y > block.pos.y);
}
void Player::pushOffY(const Block& block)
{
	if (hitsTopOf(block))
		m_pos.y += block.pos.y + block.size.y - m_pos.y;
	else if (hitsBottomOf(block))
		m_pos.y -= m_pos.y + m_size.y - block.pos.y;
}
void Player::pushOffX(const Block& block)
{
	if (hitsLeftOf(block))
		m_pos.x -= (m_pos.x + m_size.x - block.pos.x);
	else if (hitsRightOf(block))
		m_pos.x += (block.pos.x + block.size.x - m_pos.x);
}	
float Player::XOverlap(const Block& block)
{
	if (hitsBottomOf(block) || hitsTopOf(block))
	{
		if (hitsLeftOf(block))
			return m_pos.x + m_size.x - block.pos.x;
		else if (hitsRightOf(block))
			return block.pos.x + block.size.x - m_pos.x;
	}
	return 0;
}
float Player::YOverlap(const Block& block)
{
	if (hitsLeftOf(block) || hitsRightOf(block))
	{
		if (hitsTopOf(block))
			return block.pos.y + block.size.y - m_pos.y;
		else if (hitsBottomOf(block))
			return m_pos.y + m_size.y - block.pos.y;
	}
	return 0;
}

void Player::resolveCollisions(const Level& level, bool& isGrounded)
{
	const std::vector<Block> *levelData = &level.getLevelData();
	std::vector<Block> collidingBlocks;
	collidingBlocks.reserve(4);
	for (auto& block : *levelData)
	{
		if (intersects(block))
			collidingBlocks.push_back(block);
		if (!isGrounded)
		{
			// Create box for detecting grounding
			Block groundBlock(glm::vec2(m_pos.x + 0.6f, m_pos.y - 0.1f), glm::vec2(m_size.x - 1.2f, 0.2f));
			float groundCenterX = groundBlock.pos.x + groundBlock.size.x / 2;
			float groundCenterY = groundBlock.pos.y + groundBlock.size.y / 2;
			float blockCenterX = block.pos.x + block.size.x / 2;
			float blockCenterY = block.pos.y + block.size.y / 2;
			bool x = (fabs(groundCenterX - blockCenterX) < (groundBlock.size.x + block.size.x) / 2);
			bool y = (fabs(groundCenterY - blockCenterY) < (groundBlock.size.y + block.size.y) / 2);
			isGrounded = x && y;
		}
	}

	
	for (size_t i = 0; i < collidingBlocks.size(); ++i)
	{
		if (!intersects(collidingBlocks[i])) continue;
		// Ceiling bumps
		if (hitsBottomOf(collidingBlocks[i]) && XOverlap(collidingBlocks[i]) > m_size.x / 5)
		{
			pushOffY(collidingBlocks[i]);
			collidingBlocks.erase(collidingBlocks.begin() + i);
			if (m_velocity.y > 0.0f)
				m_velocity.y = 0.0f;
		}
	//	else if (hitsTopOf(collidingBlocks[i]) && XOverlap(collidingBlocks[i]) > m_size.x / 20)
	//	{
//			isGrounded = true;
	//	}
	} 

	// Resolve in air wall collisions
	if (!isGrounded)
	{
		for (size_t i = 0; i < collidingBlocks.size(); ++i)
		{
			if (intersects(collidingBlocks[i]))
			{
				if (hitsRightOf(collidingBlocks[i]))
				{
					pushOffX(collidingBlocks[i]);
					collidingBlocks.erase(collidingBlocks.begin() + i);
					if (m_velocity.x < 0.0f)
						m_velocity.x = 0.0f;
				}
				else if (hitsLeftOf(collidingBlocks[i]))
				{
					pushOffX(collidingBlocks[i]);
					collidingBlocks.erase(collidingBlocks.begin() + i);
					if (m_velocity.x > 0.0f)
						m_velocity.x = 0.0f;
				}
			}
		}
	}
	// Resolve sink-into-ground collisions
	else
	{
		for (size_t i = 0; i < collidingBlocks.size(); ++i)
		{
			if (intersects(collidingBlocks[i]) && hitsTopOf(collidingBlocks[i]) && XOverlap(collidingBlocks[i]) > m_size.x / 20)
			{
				pushOffY(collidingBlocks[i]);
				collidingBlocks.erase(collidingBlocks.begin() + i);
				if (m_velocity.y > 0.0f)
					m_velocity.y = 0.0f;
			}
		}
	}
	// Resolve grounded wall collisions
	for (size_t i = 0; i < collidingBlocks.size(); ++i)
	{
		if (intersects(collidingBlocks[i]))
		{
			if (hitsRightOf(collidingBlocks[i]))
			{
				pushOffX(collidingBlocks[i]);
				if (m_velocity.x < 0.0f)
					m_velocity.x = 0.0f;
			}
			else if (hitsLeftOf(collidingBlocks[i]))
			{
				pushOffX(collidingBlocks[i]);
				if (m_velocity.x > 0.0f)
					m_velocity.x = 0.0f;
			}
		}
	}
}


void Player::update(float frameTime, const Level& level, InputManager* inputManager)
{
	// Update position
	m_pos += frameTime * m_velocity;

	// Update velocity based on input
	if (inputManager->isKeyDown(SDLK_a))
	{
		m_velocity -= glm::vec2(m_acceleration.x, 0);
		if (m_velocity.x < -m_maxVelocity.x)
			m_velocity.x = -m_maxVelocity.x;

	}
	else if (inputManager->isKeyDown(SDLK_d))
	{
		m_velocity += glm::vec2(m_acceleration.x, 0);
		if (m_velocity.x > m_maxVelocity.x)
			m_velocity.x = m_maxVelocity.x;
	}
	else
	{
		// Dampen movement if not pressing keys
		if (m_velocity.x > 0 && m_velocity.x > m_acceleration.x)
			m_velocity -= glm::vec2(m_acceleration.x, 0);
		else if (m_velocity.x < 0 && m_velocity.x < m_acceleration.x)
			m_velocity += glm::vec2(m_acceleration.x, 0);
		else
			m_velocity = glm::vec2(0, m_velocity.y);
	}

	bool isGrounded = false;

	// Check for collisions
	resolveCollisions(level, isGrounded);

	// Gravity
	if (!isGrounded)
	{
		const float GRAVITY_POWA = 1.5f;
		m_velocity -= glm::vec2(0, GRAVITY_POWA);
	}
	else
	{
	// Can jump
		m_velocity.y = 0.0f;
		if (inputManager->isKeyPressed(SDLK_w))
		{
			//m_pos.y += m_acceleration.y;
			m_velocity.y += m_acceleration.y * 20;
		}
	}
//	printf("On Ground %d\n", (int)isGrounded);
	
}

