#include "piece.h"
#include "sgg/graphics.h"
#include "defines.h"
#include <string>
#include <cmath>

void Piece::draw()
{
	graphics::Brush br;
	
	float h = 1.0f * m_active;
	
	SETCOLOR(br.fill_color, h*0.5f, h, h*0.5f);
	
	br.fill_opacity = 1.0f;
	graphics::drawDisk(m_pos[0], m_pos[1], PIECE_SIZE * 0.5f, br);
	

	SETCOLOR(br.fill_color, m_color[0], m_color[1], m_color[2]);
	br.outline_opacity = 1.0f * m_active;
	graphics::drawDisk(m_pos[0], m_pos[1], PIECE_SIZE*0.45f , br);
	
	
	
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	
	br.fill_opacity = 1.0f;
	br.texture = m_texture;
	br.outline_opacity = 0.0f;
	
	graphics::drawRect(m_pos[0], m_pos[1], m_size, m_size, br);
	graphics::resetPose();
}

void Piece::update()
{
}

Piece::Piece()
{
	
	
	

}

bool Piece::contains(float x, float y)
{
	return distance(x, y, m_pos[0], m_pos[1]) < PIECE_SIZE * 0.4f;
}

bool Piece::intersect(Piece * target)
{
	return (distance(m_pos[0], m_pos[1], target->m_pos[0], target->m_pos[1]) < PIECE_SIZE);
		
	
	
}

Zombie::Zombie(int code) 
{
	m_code = code;
	
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("zombie1.png");
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;

	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("zombie2.png");
		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}

}
bool Zombie::inMovingRange(float init_x, float init_y, float x, float y)
{
	
	if (m_code == 1) {
		bool c1 = x - init_x > 1.0f;						//MOVES ONLY 1 TILE FORWARD
		bool c2 = x - init_x <= 2.0f;		
		bool c3 = std::abs(y - init_y) <= 0.5f;
		return c1 && c2 && c3;
	}
	if (m_code == 2) {
		bool c1 = init_x -x > 1.0f;
		bool c2 = init_x - x <= 2.0f;
		bool c3 = std::abs(y - init_y) <= 0.5f;
		return c1 && c2 && c3;


	}


	
	return false;
}
bool Zombie::inAttackRange(float init_x, float init_y, float x, float y)
{
	
	float dy = std :: abs (y - init_y);
	
	if (m_code == 1) {
		bool c1 = x - init_x > 1.0f;				//ATTACKS ONE TILE FORWARD DIAGONALLY
		bool c2 = x - init_x <= 2.0f;
		bool c3 = dy <= 2.0f && dy>=0.5;
		return c1 && c2 && c3;
	}
	else {
		bool c1 = init_x - x > 1.0f;
		bool c2 = init_x - x <= 2.0f;
		bool c3 = dy <= 2.0f && dy >= 0.5;
		return c1 && c2 && c3;
	}

	
	return false;
}





Mummy::Mummy(int code)
{
	m_code = code;
	m_size = 1.5f;
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("mummy1.png");					
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;

	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("mummy2.png");
		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}
}
bool Mummy::inMovingRange(float init_x, float init_y, float x, float y)
{
	float dx = std::abs(init_x - x);
	float dy = std::abs(init_y - y);

	bool c1 = dx <= 4.0f && dx >= 1.0f && dy <= 0.5f;				//MOVES 2 STEPS VERTICALLY OR HORIZONTALLY , BACK AND FORTH
	bool c2 = dy <= 4.0f && dy >= 1.0f && dx <= 0.5f;
	if ( c1 || c2 )
	{
		return true;
	}
	return false;
}

bool Mummy::inAttackRange(float init_x, float init_y, float x, float y)
{
	return inMovingRange( init_x,  init_y,  x,  y);
}

Robot::Robot(int code)
{
	m_code = code;
	m_size = 1.5f;
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("robot1.png");
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;
		
	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("robot2.png");
		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}
}



bool Robot::inMovingRange(float init_x, float init_y, float x, float y)
{
	float dx =( x - init_x)*(x-init_x);
	float dy = (y - init_y)* (y - init_y);
	if (  (dx - dy)*(dx-dy) <= 2.0f&& dx>=0.5f && dy>=0.5f) {
		return true;						//MOVES DIAGONALLY
	}
	return false;
}

bool Robot::inAttackRange(float init_x, float init_y, float x, float y)
{
	return inMovingRange(init_x, init_y, x, y);
}

Archer::Archer(int code)
{
	m_size = 1.5f;
	m_code = code;
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("archer1.png");
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;
	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("archer2.png");
		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}

}	

bool Archer::inMovingRange(float init_x, float init_y, float x, float y)
{
	float d = distance(x, y,init_x ,init_y );
	
	if (d <= 2.0f && d>=1.0f) {
		return true;					//MOVES 1 STEP AT ANY DIRECTION
	}				
	return false;
}


bool Archer::inAttackRange(float init_x, float init_y, float x, float y)
{
	
	float dx =std::abs( x - init_x);
	float dy = std::abs (init_y - y);						//ATTACKS AT TILES FURTHER THAN 2 STEPS
	return (dx >= 6.0f && dx <= 8.0f && dy <= 0.5);		
}

Dino::Dino(int code)
{
	
	m_code = code;
	m_size = 1.5f;
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("dino1.png");
		m_color[0] = 1.0f;										
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;
	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("dino2.png");

		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}
}

bool Dino::inMovingRange(float init_x, float init_y, float x, float y)
{
	float dx = std::abs(init_x - x);
	float dy = std::abs(init_y - y);

	bool c1 = dx <= 4.0f && dx >= 3.5f && dy >= 1.5f && dy <= 2.0f;				//MOVES IN 'GAMMA' SHAPE
	bool c2 = dy <= 4.0f && dy >= 3.5f && dx >= 1.5f && dx <= 2.0f;
	
	if (c1||c2) {
		return true;
	}
	return false;
}
bool Dino::inAttackRange(float init_x, float init_y, float x, float y)
{

	return inMovingRange(init_x, init_y, x, y);
}

Diamond::Diamond(int code)
{
	m_code = code;
	m_size = 1.5f;
	if (code == 1) {
		m_texture = ASSET_PATH + std::string("diamond1.png");
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;
	}
	if (code == 2) {
		m_texture = ASSET_PATH + std::string("diamond2.png");

		m_color[0] = 1.0f;
		m_color[1] = 0.0f;
		m_color[2] = 1.0f;
	}

}


bool Diamond::inMovingRange(float init_x, float init_y, float x, float y)
{
	
	float dx = std::abs(init_x - x);
	float dy = std::abs(init_y - y);

	bool c1 = dx <= 2.0f && dx >= 1.0f && dy <= 0.5f;
	bool c2 = dy <= 2.0f && dy >= 1.0f && dx <=0.5f ;					//MOVES ONE STEP AT ANY DIRECTION, CANNOT ATTACK AT ALL

	if (c1 || c2) {
		return true;
	}
	return false;
}
bool Diamond::inAttackRange(float init_x, float init_y, float x, float y)
{

	return false;
}
