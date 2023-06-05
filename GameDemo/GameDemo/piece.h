#pragma once
#include <string>
#include "defines.h"
class Piece {
	float m_pos[2];
	
	
	float m_orientation;

	bool m_highlighted = false;
	bool m_active = false;
	
	

protected:
	float m_color[3];
	std::string m_texture;
	float m_size = 2.0f;
	int m_code;
	
	
	
	
public:
	void draw();
	void update();
	
	

	

	Piece();

	float getPosX() { return m_pos[0]; }
	float getPosY() { return m_pos[1]; }
	void setPosX(float x) { m_pos[0] = x; }
	void setPosY(float y) { m_pos[1] = y; }

	void setHighlight(bool h) { m_highlighted = h; }
	void setActive(bool a) { m_active = a; }
	

	bool contains(float x, float y);

	bool intersect(Piece* target);
	virtual bool inMovingRange(float init_x, float init_y, float x, float y) { return false; };
	virtual bool inAttackRange(float init_x, float init_y, float x, float y) { return false; };
	virtual bool isDiamond() { return false; };
	
	int getCode() { return m_code; };
};

class Zombie : public Piece 
{
public:
	
	Zombie(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y) override;
	
};

class Mummy : public Piece
{
public:
	Mummy(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y)  override;
};

class Robot : public Piece
{
public:
	Robot(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y)  override;
};


class Archer : public Piece
{
public:
	Archer(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y)  override;
};

class Dino : public Piece
{
public:
	Dino(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y)  override;
};

class Diamond : public Piece
{
public:
	Diamond(int code);
	bool inMovingRange(float init_x, float init_y, float x, float y) override;
	bool inAttackRange(float init_x, float init_y, float x, float y) override;
	bool isDiamond() override { return true; };
};