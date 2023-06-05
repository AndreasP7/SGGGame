#include "defines.h"
#include "game.h"
#include <sgg/graphics.h>
#include <sgg/scancodes.h>
#include "event.h"
#include <string>	

void Game::draw()
{
	
	
	//PRINT TITLE SCREEN
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	
	br.outline_opacity = 0.0f;
	br.texture = ASSET_PATH + std::string("title_background.png");
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH +1.0f, CANVAS_HEIGHT, br);

	
	br.texture = ASSET_PATH + std::string("title.png");
	graphics::drawRect(CANVAS_WIDTH / 2, (CANVAS_HEIGHT / 2)-3.0f, CANVAS_WIDTH/2 + 1.0f, CANVAS_HEIGHT/2+1.0f, br);


	if (m_state == STATE_INIT) {
		br.outline_opacity = 1.0f;
		SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::setFont(ASSET_PATH + std::string("CHOCO-DONUT.ttf"));
		graphics::drawText((CANVAS_WIDTH / 2) - 4.0f, CANVAS_HEIGHT / 2, 1.0f, "PRESS SPACE TO START", br);
	}

	if (m_state == GAME_OVER) {
		
		//PRINT THE WINNER
		
		br.outline_opacity = 1.0f;
		SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::setFont(ASSET_PATH + std::string("CHOCO-DONUT.ttf"));
		graphics::drawText((CANVAS_WIDTH / 2) - 4.0f, CANVAS_HEIGHT / 2, 1.0f, "THE WINNER IS", br);


		

		if (m_loser_code == 1) {
			SETCOLOR(br.fill_color, 1.0f, 0.5f, 1.0f);
			graphics::drawText((CANVAS_WIDTH / 2) + 1.5f, CANVAS_HEIGHT / 2, 1.0f, "PURPLE", br);
		}
		else {
			SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
			graphics::drawText((CANVAS_WIDTH / 2) + 1.5f, CANVAS_HEIGHT / 2, 1.0f, "WHITE", br);
		}
		
		SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::drawText((CANVAS_WIDTH / 2) -5.5f , (CANVAS_HEIGHT / 2) + 4.0f, 1.0f, "PRESS SPACE TO PLAY AGAIN", br);

	}

	
	
	
	if (m_state != STATE_INIT ) {
		
		//DRAW BOARD
		br.outline_opacity = 0.0f;
		br.texture = ASSET_PATH + std::string("game-board.png");
		SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

		graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		graphics::Brush sq;
		sq.outline_opacity = 0.3f;
		sq.fill_opacity = 0.0f;

		
		//DRAW TILES, GREEN IF IN MOVING RANGE, RED IF IN ATTACK RANGE
		for (int i = 0.0f; i < CANVAS_WIDTH / PIECE_SIZE; i++)
			for (int j = 0.0f; j < CANVAS_HEIGHT / PIECE_SIZE; j++)
			{
				float x = (i + 0.5f) * PIECE_SIZE;
				float y = (j + 0.5f) * PIECE_SIZE;

				if (m_state == STATE_MOVING && m_active_piece->inMovingRange(m_init_pos_x, m_init_pos_y, x, y))
				{
					SETCOLOR(sq.fill_color, 0.0f, 1.0f, 0.0f);
					sq.fill_opacity = 0.5f;
					

				}
				else if (m_state == STATE_ATTACKING && m_active_piece->inAttackRange(m_init_pos_x, m_init_pos_y, x, y))
				{
					SETCOLOR(sq.fill_color, 1.0f, 0.0f, 0.0f);
					sq.fill_opacity = 0.5f;
				}
				
				else
				{
					sq.fill_opacity = 0.0f;
				}

				graphics::drawRect(x, y, PIECE_SIZE, PIECE_SIZE, sq);
			}

		graphics::Brush reticle;
		
		

		
		if (m_state == STATE_MOVING || m_state == STATE_ATTACKING) {
			reticle.fill_opacity = 1.0f;
			graphics::drawDisk(m_init_pos_x, m_init_pos_y, 0.1f, reticle);

			graphics::drawLine(m_init_pos_x, m_init_pos_y, m_active_piece->getPosX(), m_active_piece->getPosY(), reticle);

		}

		


		if (m_state != GAME_OVER && m_state!=STATE_LOADING) {
			for (auto ev : m_events)
			{
				ev->draw();
			}

			for (auto piece : *attacking_pieces) {
				piece->draw();
			}
			for (auto piece : *defending_pieces) {
				piece->draw();
			}
		}
		
		
		
	}
	
}
void Game::update()
{
	
	
	if (m_state == GAME_OVER) 
	{
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			
			m_state = STATE_LOADING;
			 attacking_pieces = new std::list<Piece*>;
			 defending_pieces = new std::list<Piece*>;
			
		}
		return;
	}

	
	//CHECK IF SPACE IS PRESSED
	if (m_state == STATE_INIT)
	{
		graphics::playMusic(ASSET_PATH + std::string("Intro.mp3"), 1.0f,true,100);
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) {
			m_state = STATE_LOADING;
		}
		return;
	}
	//START GAME
	if (m_state ==STATE_LOADING) {
		init();
		m_state = STATE_IDLE;
		addEvent(new FadeFromBlackEvent());
		
		return;
	}
	
	processEvents();
	

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	
	


	float mx = graphics::windowToCanvasX(ms.cur_pos_x);
	float my = graphics::windowToCanvasY(ms.cur_pos_y);

	Piece* cur_piece = nullptr;



	for (auto piece : *attacking_pieces) {
		if (piece->contains(mx, my)) {
			
			cur_piece = piece;
		}
		

		
	}

	
	//SET ACTIVE PIECE
	if (ms.button_left_pressed && cur_piece) {
		m_active_piece = cur_piece;
		m_active_piece->setActive(true);
		for (auto piece : *attacking_pieces) {
			if (piece != m_active_piece) {
				piece->setActive(false);
			}
		}
		m_init_pos_x = m_active_piece->getPosX();
		m_init_pos_y = m_active_piece->getPosY();
		
		m_valid_action = false;
	}

	if (ms.dragging && m_active_piece) {
			
			mx = PIECE_SIZE * (0.5f + (int)(mx / PIECE_SIZE));
			my = PIECE_SIZE * (0.5f + (int)(my / PIECE_SIZE));

			m_active_piece->setPosX(mx);
			m_active_piece->setPosY(my);
			m_state = STATE_MOVING;
			addEvent(new RedDot(mx, my));
			
		//MOVE ONLY ON TILES IN MOVING RANGE	
		if (m_active_piece->inMovingRange(m_init_pos_x,m_init_pos_y,mx, my)) {
			m_valid_action = true;

			

		}
		else {
			m_valid_action = false;
		}
		
		
			
			
			

			
		m_target_piece = nullptr;
		
		for (auto piece : *defending_pieces ) {
			
			
			//FIND TARGET PIECE

			if (piece->intersect(m_active_piece)  ) {
				m_state = STATE_ATTACKING;
				m_target_piece = piece;
			}
		}
		
		
		//CHECK IF ATTACK IS VALID
		if (m_state == STATE_ATTACKING) {
			if (m_target_piece!= nullptr && m_active_piece->inAttackRange(m_init_pos_x,m_init_pos_y, m_target_piece->getPosX(), m_target_piece->getPosY()))  {
					m_valid_action = true;

			}
			else {
					m_valid_action = false;
			}
		}

		//CHECK IF TEAMMATE ON TILE
		m_teammate = nullptr;
		for (auto piece : *attacking_pieces) {
			if (piece->intersect(m_active_piece) && piece != m_active_piece) {
				m_teammate = piece;
			}
		}
		if (m_teammate != nullptr) {
			m_valid_action = false;
		}
		
		



	}

	//CHECK IF VALID ACTION AFTER MOUSE BUTTON IS RELEASED
	if (ms.button_left_released && m_active_piece) {
		if (!m_valid_action)
		{
			m_state = STATE_RETURNING;
			
			
			m_change = false;
			m_active_piece->setPosX(m_init_pos_x);
			m_active_piece->setPosY(m_init_pos_y);
			
		}
		else {
			m_change = true;
		}
		
		m_state = STATE_IDLE;




		if (m_valid_action && m_target_piece) {
			Piece* tgt = m_target_piece;

			defending_pieces->remove_if([tgt](Piece* pl) {return pl == tgt; });

			if (tgt->isDiamond()) {
				m_state = GAME_OVER;
				m_loser_code = tgt->getCode();
			}
		
			delete m_target_piece;
			
		}
		
			

		m_target_piece = nullptr;
		m_active_piece->setActive(false);
		m_active_piece = nullptr;
		
		
	}
	
	

	for (auto piece : *attacking_pieces) {
		piece->update();
	}
	for (auto piece : *defending_pieces) {
		piece->update();
	}

	if (m_change) {
		changeTurn();
		m_change = false;
		m_active_piece = nullptr;
	}


}
void Game::init()
{
	
	
	
	Piece* p;

	
	for (float y = 1.0f; y <= 16.0; y += 2.0f) {
		p = new Zombie(1);
		attacking_pieces->push_front(p);
		p->setPosX(3.0f);
		p->setPosY(y);

	}
	
	
	
	//SETTING WHITE PIECES

	p = new Diamond(1);
	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(7.0f);
	
	 p = new Mummy(1);

	 attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(1.0f);

	p = new Mummy(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(15.0f);


	p = new Robot(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(5.0f);

	p = new Robot(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(11.0f);

	p = new Archer(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(9.0f);

	p = new Dino(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(3.0f);

	p = new Dino(1);

	attacking_pieces->push_front(p);
	p->setPosX(1.0f);
	p->setPosY(13.0f);

	//SETTING PURPLE PIECES
	
	for (float y = 1.0f; y <= 16.0; y += 2.0f) {
		p = new Zombie(2);
		defending_pieces->push_front(p);
		p->setPosX(13.0f);
		p->setPosY(y);

	}
	

	
	p = new Diamond(2);
	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(7.0f);

	

	p = new Mummy(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(15.0f);

	p = new Mummy(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(1.0f);


	p = new Robot(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(11.0f);
	
	p = new Robot(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(5.0f);

	p = new Archer(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(9.0f);

	p = new Dino(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(3.0f);

	p = new Dino(2);

	defending_pieces->push_front(p);
	p->setPosX(15.0f);
	p->setPosY(13.0f);

	
	graphics::preloadBitmaps(ASSET_PATH);

	
}



Game::~Game()
{
	for (auto piece :*attacking_pieces) {
		delete piece;
	}
	attacking_pieces->clear();
	attacking_pieces = nullptr;
	for (auto piece : *defending_pieces) {
		delete piece;
	}
	defending_pieces->clear();
	defending_pieces = nullptr;
}
Game* Game::getInstance()
{
	if (!m_instance)
		m_instance = new Game();
	return m_instance;
}
void Game::processEvents()
{
	for (auto e : m_events) 
	{
		e->update();

	}
	m_events.remove_if([](Event* ev) {return !ev->active(); });

}
void Game::addEvent(Event* evt)
{
	m_events.push_front(evt);
}
Game* Game::m_instance = nullptr;

void Game::changeTurn() {
	std::swap(attacking_pieces, defending_pieces);
}

