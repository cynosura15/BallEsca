#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "commun.h"
#include "Ball.hpp"

#define ST_PAUSE 6
#define ST_DRAW_SCORES 5
#define ST_GAMEOVER 4
#define ST_LOST 3
#define ST_PLAYING 2
#define ST_MENU_RESET 1
#define ST_MENU 0
#define ST_EXIT_CONFIRM -1
#define ST_EXIT -2

class Game
{
public:
	Game();
	void CheckInputs();
	void Update();
	void Draw();
	int getState();
	void Init(int difficulty);
	void Opening() const;
	std::string getDifficultyStr() const;
	int getScore() const;
	int getInitBgFreq() const;
	
private:
	int m_state, m_last_state, m_score, m_timer, m_lives, m_open_effect_tick, m_spawn_freq, m_bg_freq, m_init_bg_freq;
	bool m_playing, m_touch_locked, m_open_effect, updateBorder;
	std::vector<u32> m_bgColor;
	std::vector<Ball> ballsIndex;
	Ball *player;
	std::string m_difficulty_str;
};

#endif