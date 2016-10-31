#ifndef _BALL_HPP_
#define _BALL_HPP_

#include "commun.h"

class Ball
{
public:
	Ball(int type, int posx = 0, int posy = 0, int radius = 0, u32 color = RGBA8(0, 0, 0, 0));
	bool Update(touchPosition const& touch, bool const& updateBorder);
	void Draw();
	bool isPressed(float const& touch_x, float const& touch_y);
	bool touch(Ball const& ball, bool const& updateBorder);
	int get(char const& type) const;
	void reset(bool const& updateBorder);

private:
	float m_x, m_y, m_radius;
	u32 m_color;
	float m_incX, m_incY;
	bool isPlayer, isBorder, m_border_up;
};

#endif