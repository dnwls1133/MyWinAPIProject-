#pragma once
#include <windows.h>
class Object
{
private:
	POINT m_pos;
	POINT m_scale;
	RECT m_rect;
	COLORREF m_color;
	int m_vel;
	int m_movetype;
	int m_objecttype;
	int m_xdir;
	int m_ydir;
public:
	int m_hp;
public:
	void setPos(POINT _pos) { m_pos = _pos; }
	void setscale(POINT _scale) { m_scale = _scale; }
	void setcolor(COLORREF _color) { m_color = _color; }
	void setmovetype(int _type) { m_movetype = _type; }
	void setobjecttype(int _type) { m_objecttype = _type; }
	void setvel(int _vel) { m_vel = _vel; }
	void setrect();
	void switchdir(int _num);
	void switchsignal(COLORREF _state);
	void clear() { m_pos = {}; m_scale = {}; m_rect = {}; m_color = 0;}
	POINT getPos() { return m_pos; }
	POINT getscale() { return m_scale; }
	COLORREF getcolor() { return m_color; }
	int getvel() { return m_vel; }
	int getmovetype() { return m_movetype; }
	int getxdir() { return m_xdir; }
	int getydir() { return m_ydir; }
	void render(HDC _dc);

public:
	Object();
	~Object();
};