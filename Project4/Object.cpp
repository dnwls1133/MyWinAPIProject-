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

void Object::switchsignal(COLORREF _state)
{
	switch (_state)
	{
	case RGB(255,0,0):
		m_color = RGB(0, 0, 255);
		break;
	case RGB(255,255, 0):
		m_color = RGB(255, 0, 0);
		break;
	
	case RGB(0, 0, 255):
		m_color = RGB(255, 255, 0);
		break;

	}
}


void Object::render(HDC _dc)
{
	setrect();
	switch (m_objecttype)
	{
	case 0:
		Rectangle(_dc, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		break;
	case 1:
		Ellipse(_dc, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		break;
	}
	
	
}

Object::Object()
	: m_pos{}
	, m_rect{}
	, m_scale{}
	, m_color()
	, m_hp()
	, m_vel(25)
	, m_xdir(1)
	, m_ydir(1)
	
{
	
}

Object::~Object()
{

}
