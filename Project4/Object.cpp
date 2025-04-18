#include "Object.h"
#include <windows.h>
#include <tchar.h>
#include <cmath>
#define PI 3.14


void Object::setrect()
{

	m_rect.left = m_pos.x - m_scale.x;
	m_rect.top = m_pos.y - m_scale.y;
	m_rect.right = m_pos.x + m_scale.x;
	m_rect.bottom = m_pos.y + m_scale.y;

}

void Object::switchdir(int _num)
{
	if (_num == 0)
	{
		m_xdir *= -1;
	}
	else
	{
		m_ydir *= -1;
	}
}


void Object::render(HDC _dc)
{
	setrect();
	
	Rectangle(_dc, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	
}

Object::Object()
	: m_pos{}
	, m_rect{}
	, m_scale{}
	, m_color()
	, m_hp()
	, m_vel(20.f)
	, m_xdir(1)
	, m_ydir(1)
	
{
	
}

Object::~Object()
{

}
