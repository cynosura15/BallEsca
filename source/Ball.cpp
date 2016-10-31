#include "Ball.hpp"

Ball::Ball(int type, int posx, int posy, int radius, u32 color)
{
	if(type == 0)	//ennemy
	{
		int randPos = rand() % 4;
		int randSpeedX, randSpeedY;

		switch(randPos)
		{
			case 0:	//TOP
				m_x = rand() % BOT_W;
				m_y = 0;
				do{
					randSpeedX = rand() % 200 + (-100);
					m_incX = ((float)randSpeedX/100.)*3;
					randSpeedY = rand() % 100;
					m_incY = ((float)randSpeedY/100.)*3;
				} while((randSpeedX <= 20 && randSpeedX >= -20) || (randSpeedY <= 20 && randSpeedY >= -20));
				break;
			case 1:	//LEFT
				m_x = 0;
				m_y = rand() % HEIGHT;
				do{
					randSpeedX = rand() % 100;
					m_incX = ((float)randSpeedX/100.)*3;
					randSpeedY = rand() % 200 + (-100);
					m_incY = ((float)randSpeedY/100.)*3;
				} while((randSpeedX <= 20 && randSpeedX >= -20) || (randSpeedY <= 20 && randSpeedY >= -20));
				break;
			case 2:	//BOT
				m_x = rand() % BOT_W;
				m_y = HEIGHT;
				do{
					randSpeedX = rand() % 200 + (-100);
					m_incX = ((float)randSpeedX/100.)*3;
					randSpeedY = rand() % 100;
					m_incY = ((float)randSpeedY/100.)*-3;
				} while((randSpeedX <= 20 && randSpeedX >= -20) || (randSpeedY <= 20 && randSpeedY >= -20));
				break;
			case 3:	//RIGHT
				m_x = BOT_W;
				m_y = rand() % HEIGHT;
				do{
					randSpeedX = rand() % 100;
					m_incX = ((float)randSpeedX/100.)*-3;
					randSpeedY = rand() % 200 + (-100);
					m_incY = ((float)randSpeedY/100.)*3;
				} while((randSpeedX <= 20 && randSpeedX >= -20) || (randSpeedY <= 20 && randSpeedY >= -20));
				break;
		}

		m_color = RGBA8(255, 0, 0, 255);
		m_radius = 3;
		isPlayer = false;
		isBorder = false;
		m_border_up = false;
	}
	else if(type == 1)	//player
	{
		m_x = BOT_W/2;
		m_y = HEIGHT/2;
		m_color = RGBA8(0, 0, 255, 255);
		m_radius = 4;
		isPlayer = true;
		isBorder = false;
		m_border_up = false;
	}
	else if(type == 2)	//borders
	{
		m_x = posx;
		m_y = posy;
		m_radius = radius;
		m_color = color;
		isPlayer = false;
		isBorder = true;
		//int randNum = rand() % 2;
		//m_border_up = false;
		//if(randNum == 1)
			m_border_up = true;
	}
}

bool Ball::Update(touchPosition const& touch, bool const& updateBorder)
{
	if(isPlayer)
	{
		m_x = (float)touch.px;
		m_y = (float)touch.py;
		return false;
	}

	else if(!isPlayer && !isBorder)
	{
		m_x += m_incX;
		m_y += m_incY;
	}
	else if(updateBorder && isBorder)
	{
		if(m_border_up){
			m_radius+=0.1;
			if(m_radius >= 40)
				m_border_up = false;
		}
		if(!m_border_up){
			m_radius-=0.1;
			if(m_radius <= 5)
				m_border_up = true;
		}
	}

	return (m_x-m_radius > BOT_W || m_x+m_radius < 0 || m_y-m_radius > HEIGHT || m_y+m_radius < 0);
}

void Ball::Draw()
{
	sf2d_draw_fill_circle(m_x, m_y, m_radius, m_color);
}

bool Ball::isPressed(float const& touch_x, float const& touch_y)
{
	return ((touch_x <= m_x+m_radius) && (touch_x >= m_x-m_radius) && (touch_y <= m_y+m_radius) && (touch_y >= m_y-m_radius));
}

bool Ball::touch(Ball const& ball, bool const& updateBorder)
{
	float x2 = ball.get('x');
	float y2 = ball.get('y');
	float radius2 = ball.get('r');

	float distx = fabs(m_x - x2);
	float disty = fabs(m_y - y2);
	float distSq = distx*distx+disty*disty;//x, y, x, y

	if(distSq > ((m_radius + radius2) * (m_radius + radius2)))
    	return false; // no intersection

    return true;
}

int Ball::get(char const& type) const
{
	if(type == 'x')
		return m_x;
	if(type == 'y')
		return m_y;
	if(type == 'r')
		return m_radius;

	return -666;
}

void Ball::reset(bool const& updateBorder)
{
	m_x = BOT_W/2;
	m_y = HEIGHT/2;

	if(updateBorder)
	{
		m_x = BOT_W/3;
		m_y = HEIGHT/3;
	}
}