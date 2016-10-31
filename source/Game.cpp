#include "Game.hpp"

touchPosition touch;
int lastX, lastY;


Game::Game() : m_state(0), m_last_state(0), m_score(0), m_timer(0), m_lives(5), m_open_effect_tick(255), m_spawn_freq(10), m_bg_freq(500), m_init_bg_freq(10), m_playing(false), m_touch_locked(true), m_open_effect(true), updateBorder(false), m_difficulty_str("hello")
{
	m_bgColor.push_back(RGBA8(0, 0, 0, 255));
	m_bgColor.push_back(RGBA8(175, 210, 255, 255));
	m_bgColor.push_back(RGBA8(86, 137, 202, 255));
	m_bgColor.push_back(RGBA8(19, 75, 148, 255));
	m_bgColor.push_back(RGBA8(47, 36, 105, 255));
	m_bgColor.push_back(RGBA8(152, 19, 104, 255));
	m_bgColor.push_back(RGBA8(132, 11, 11, 255));
	m_bgColor.push_back(RGBA8(66, 18, 18, 255));
	m_bgColor.push_back(RGBA8(0, 0, 0, 255));
}

void Game::CheckInputs()
{
	hidScanInput();
	hidTouchRead(&touch);
}

void Game::Update()
{
	//Effet fondu
	if(hidKeysDown() & KEY_A || m_state != ST_PAUSE && m_open_effect_tick <= 0)
		m_open_effect = false;
	if(m_state != ST_PAUSE && m_open_effect)
		m_open_effect_tick-=3;

	//Si on joue et qu'on a pas perdu, on remet le status à ST_PLAYING; sert si on vient de se faire toucher.
	if(m_playing && m_state != ST_GAMEOVER && m_state != ST_PAUSE)
		m_state = ST_PLAYING;

	if(m_state != ST_PAUSE){
		lastX = touch.px;
		lastY = touch.py;
	}
	else
		m_touch_locked = true;

	//start: quitte la partie, ou quitte le menu donc le jeu
	if(hidKeysDown() & KEY_START){
		if(m_state == ST_MENU)
			m_state = ST_EXIT_CONFIRM;
		else if(m_state == ST_EXIT_CONFIRM)
			m_state = ST_EXIT;
		else if(m_playing)
			m_state = ST_MENU_RESET;
		else
			m_state = ST_MENU;
	}
	if(hidKeysDown() & KEY_B)	//pour annuler la sortie
	{
		if(m_state != ST_MENU && !m_playing)
			m_state = ST_MENU;
	}
	if(hidKeysDown() & KEY_B || hidKeysDown() & KEY_DOWN){
		if(m_state == ST_PLAYING){
			m_state = ST_PAUSE;
			m_last_state = ST_PLAYING;
			goto next;
		}
		else if(m_state == ST_PAUSE){
			m_state = m_last_state;
			goto next;
		}
	}
	next:
	if(hidKeysDown() & KEY_A)	//
	{
		if(!m_playing && m_state == ST_MENU){	//dans le menu
			int difficulty(1);
			if(hidKeysHeld() & KEY_UP)
				difficulty = 0;
			if(hidKeysHeld() & KEY_DOWN)
				difficulty = 2;

			Init(difficulty);
		}

		if(m_state == ST_GAMEOVER)	//quand on a perdu, il faut cliquer sur A pour revenir au menu
			m_state = ST_MENU_RESET;
	}

	if(m_playing && m_state != ST_GAMEOVER && m_state != ST_PAUSE)
	{
		m_score++;
		m_timer++;
		if(m_timer >= m_spawn_freq)
		{
			m_timer = 0;
			Ball *ball = new Ball(0);
			ballsIndex.push_back(*ball);
		}

		if(m_spawn_freq > 2 && m_score % 300 == 0)
			m_spawn_freq--;

		if(m_difficulty_str != "easy" && !updateBorder && m_score >= m_init_bg_freq*4){
			Ball *ball1 = new Ball(2, 0, 0, 0, C_RED);
			ballsIndex.push_back(*ball1);
			Ball *ball2 = new Ball(2, 0, HEIGHT, 0, C_RED);
			ballsIndex.push_back(*ball2);
			Ball *ball3 = new Ball(2, BOT_W, HEIGHT, 0, C_RED);
			ballsIndex.push_back(*ball3);
			Ball *ball4 = new Ball(2, BOT_W, 0, 0, C_RED);
			ballsIndex.push_back(*ball4);
			Ball *ball5 = new Ball(2, BOT_W/2, HEIGHT/2, 0, C_RED);
			ballsIndex.push_back(*ball5);

			updateBorder = true;
		}

		for(unsigned i = 0; i < ballsIndex.size(); i++){
			if(ballsIndex[i].Update(touch, updateBorder))
				ballsIndex.erase(ballsIndex.begin() + i);

			if(player->touch(ballsIndex[i], updateBorder))
				m_state = ST_LOST;
		}

		//On doit appuyer sur le joueur pour le deplacer
		if((hidKeysDown() & KEY_TOUCH) && (touch.px == lastX && touch.py == lastY) && player->isPressed(lastX, lastY))
			m_touch_locked = false;
		if(!(hidKeysHeld() & KEY_TOUCH))
			m_touch_locked = true;
		if(!m_touch_locked)
			player->Update(touch, updateBorder);
		//----------------------------------------------

		if(m_state == ST_LOST)
		{
			m_score--;
			m_lives--;
			if(m_lives < 0)
				m_state = ST_GAMEOVER;

			ballsIndex.clear();
			player->reset(updateBorder);
			updateBorder = false;
		}
	}
}

void Game::Draw()
{
	unsigned colorID = m_score/m_bg_freq+1;	//every m_bg_freq pts bg color change
	if(colorID >= m_bgColor.size())
		colorID = 8;

	if(!m_playing)
		sf2d_set_clear_color(m_bgColor[0]);
	else
		sf2d_set_clear_color(m_bgColor[colorID]);

	sf2d_start_frame(GFX_TOP, GFX_LEFT);

		if(!m_playing)
			DrawTextCentered(titleFont, HEIGHT/2-17, C_WHITE, 35, "BallEsca");

		else
		{
			u32 color = C_WHITE;
			if(colorID == 1)
				color = C_BLACK;

			DrawTextCentered(titleFont, 10, RGBA8(70, 53, 198,255), 30, "BallEsca");

			if(m_state != ST_GAMEOVER)
			{
				sftd_draw_textf(mainFont, 5, 5, color, 15, "%d FPS", (int)sf2d_get_fps());
				DrawTextCentered(mainFont, HEIGHT/2.5, C_BLACK, 25, "Score");
				DrawTextCenteredF(scoreFont, HEIGHT/2, RGBA8(96, 6, 6, 255), 35, "%d", m_score);
				DrawTextCenteredF(mainFont, HEIGHT-60, RGBA8(5, 53, 104, 255), 25, "Lives: %d", m_lives);
				sftd_draw_textf(mainFont, TOP_W/2, HEIGHT-20, color, 15, "You are playing in %s mode", m_difficulty_str.c_str());
				sftd_draw_text(mainFont, 5, HEIGHT-20, color, 15, "DOWN or B to pause");
			}
			else{
				DrawTextCentered(mainFont, HEIGHT/3, C_RED, 25, "Game over !");
				DrawTextCentered(mainFont, HEIGHT/2, color, 20, "Your final score is");
				DrawTextCenteredF(scoreFont, HEIGHT/1.7, RGBA8(96, 6, 6, 255), 25, "%d", m_score);
				DrawTextCenteredF(mainFont, HEIGHT/1.2, color, 17, "in %s mode.", m_difficulty_str.c_str());
			}
		}

		if(m_open_effect)
			sf2d_draw_rectangle(0, 0, TOP_W, HEIGHT, RGBA8(0, 0, 0, m_open_effect_tick));

	sf2d_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

		if(!m_playing){
			if(m_state == ST_MENU){	
				DrawTextCentered(mainFont, HEIGHT/3, RGBA8(18, 72, 109, 255), 22, "Press A for a normal game");
				DrawTextCentered(mainFont, HEIGHT*2/3-40, RGBA8(18, 72, 109, 255), 18, "UP or DOWN + A for an easy/hardcore game");
				DrawTextCentered(mainFont, HEIGHT-60, C_WHITE, 20, "Start to exit");
			}
			if(m_state == ST_EXIT_CONFIRM)
				DrawTextCentered(mainFont, HEIGHT-60, C_WHITE, 20, "Really? Start to confirm/B to return");

			DrawTextCenteredF(mainFont, HEIGHT-20, C_GREY, 16, "Game by cyno | 2016 | version %s", VERSION);
		}

		if(m_state != ST_PAUSE && m_state != ST_GAMEOVER && m_playing){
			for(unsigned i = 0; i < ballsIndex.size(); i++)
				ballsIndex[i].Draw();

			player->Draw();
		}

		if(m_state == ST_PAUSE)
			DrawTextCentered(mainFont, HEIGHT/2-10, C_RED, 25, "PAUSE");

		if(m_state == ST_GAMEOVER)
			DrawTextCentered(mainFont, HEIGHT/2, C_BLUE, 20, "Press A to continue.");

		if(m_state == ST_LOST)
			DrawTextCenteredF(mainFont, HEIGHT/2, C_RED, 18, "Touch! %d lives left", m_lives);

		if(m_open_effect)
			sf2d_draw_rectangle(0, 0, BOT_W, HEIGHT, RGBA8(0, 0, 0, m_open_effect_tick));

	sf2d_end_frame();

	sf2d_swapbuffers();

	if(m_state == ST_LOST)
		svcSleepThread(2000000000);
}

int Game::getState()
{
	return m_state;
}

void Game::Init(int difficulty)
{
	if(difficulty == 0)	//easy
	{
		m_spawn_freq = 40;
		m_lives = 10;
		m_difficulty_str = "easy";
		m_bg_freq = 1000;
		m_init_bg_freq = m_bg_freq;
	}
	if(difficulty == 1)	//normal
	{
		m_spawn_freq = 20;
		m_lives = 5;
		m_difficulty_str = "normal";
		m_bg_freq = 700;
		m_init_bg_freq = m_bg_freq;
	}
	if(difficulty == 2)	//hardcore
	{
		m_spawn_freq = 5;
		m_lives = 2;
		m_difficulty_str = "h4rdC0r3";
		m_bg_freq = 300;
		m_init_bg_freq = m_bg_freq;
	}
	
	player = new Ball(1);	//nouveau player

	m_open_effect = true;
	m_open_effect_tick = 255;
	m_state = ST_PLAYING;
	m_playing = true;

	//timer
	for (int i = 3; i >= 0; --i)
	{
		std::stringstream timeStr;
		timeStr << (i);
		std::string timeStrFinal = timeStr.str();
		if(i == 0)
			timeStrFinal = "GO!";

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
			if(i != 0) DrawTextCentered(mainFont, HEIGHT/2.6, C_GREY, 19, "Game start in...");
			DrawTextCentered(scoreFont, HEIGHT/2, RGBA8(255, 0, 0, 255), 20, timeStrFinal.c_str());
		sf2d_end_frame();
		sf2d_swapbuffers();

		svcSleepThread(1000000000);
	}
}

void Game::Opening() const
{
	sf2d_set_clear_color(C_BLACK);

	bool alphaDown(false), alphaUp(true);
	int alpha(0);

	while(true)
	{
		if(alphaDown == alphaUp)
			break;

		if(alphaUp && alpha < 255){
			alpha+=10;
			if(alpha >= 255){
				alpha = 255;
				alphaDown = true;
				alphaUp = false;
			}
		}
		else if(alphaDown && alpha > 0){
			alpha-=10;
			if(alpha <= 0){
				alpha = 0;
				alphaDown = false;
			}
		}

		sf2d_start_frame(GFX_TOP, GFX_LEFT);

			DrawTextCentered(mainFont, HEIGHT/2-15, RGBA8(255, 255, 255, alpha), 30, "a game by cyno");
			DrawTextCentered(mainFont, HEIGHT/2+30, RGBA8(255, 255, 255, alpha), 20, "thanks to smea & xerpi");

		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

			//just to have both sreens blacks

		sf2d_end_frame();


		sf2d_swapbuffers();

		if(alpha >= 255)
			svcSleepThread(2000000000);
	}
}

std::string Game::getDifficultyStr() const
{
	return m_difficulty_str;
}

int Game::getScore() const
{
	return m_score;
}

int Game::getInitBgFreq() const
{
	return m_init_bg_freq;
}