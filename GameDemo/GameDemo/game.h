#pragma once
#include "defines.h"
#include "game.h"
#include <sgg/graphics.h>
#include <list>
#include "piece.h"
#include "event.h"
#include <string>

class Game {
public:
	enum game_state_t {STATE_INIT,STATE_LOADING,STATE_PLAYING, STATE_IDLE, STATE_MOVING,STATE_RETURNING, STATE_ATTACKING, GAME_OVER};


protected:

	static Game* m_instance;
	
	

	std::list<Piece*>* attacking_pieces = new std::list<Piece*>;
	std::list<Piece*>* defending_pieces = new std::list<Piece*>;

	std::list<Event*> m_events;

	Piece* m_active_piece = nullptr;
	Piece* m_target_piece = nullptr;
	Piece* m_teammate = nullptr;
	

	std::string winner;

	float m_init_pos_x;
	float m_init_pos_y;
	bool m_valid_action = false;
	bool m_change = false;
	game_state_t m_state = STATE_INIT;
	int m_loser_code;
	
	Game(){}

public :
	void draw();
	void update();
	void init();

	void changeTurn();

	~Game();

	static void releaseInstance() { if (m_instance) delete m_instance; m_instance = nullptr; }
	static Game* getInstance();
	
	void processEvents();
	void addEvent(Event* evt);

};

