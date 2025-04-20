#pragma once
#include <Windows.h>
#include <cmath>
#define PI 3.1415926535
RECT makeboard(HDC _dc, POINT _resolution)
{
	RECT boardinfo;
	int x = 340;
	int y = 240;
	int bresoly = 0;
	int bresolx = 0;
	int count = 0;
	while (count < 8)
	{
		if (count < 4)
		{
			MoveToEx(_dc, x, y, NULL);
			LineTo(_dc, _resolution.x - x, y);
			y += 200;
			bresoly = y;
		}
		else
		{
			y = 240;
			MoveToEx(_dc, x, y, NULL);
			LineTo(_dc, x, bresoly - 200);
			x += 200;
			bresolx = x;
		}

		++count;
	}
	x = 340;
	y = 240;
	boardinfo.left = x;
	boardinfo.top = y;
	boardinfo.right = bresolx - 200;
	boardinfo.bottom = bresoly - 200;
	return boardinfo;
	
}
RECT makeYuutboard(HDC _dc, POINT _resolution)
{
	RECT boardinfo;
	int x = 230;
	int y = 110;
	int bresoly = 0;
	int bresolx = 0;
	int count = 0;
	MoveToEx(_dc, x, y, NULL);
	LineTo(_dc, _resolution.x - x, y);
	MoveToEx(_dc, _resolution.x - x, y,NULL);
	LineTo(_dc, _resolution.x - x, _resolution.y-y);
	MoveToEx(_dc, _resolution.x - x, _resolution.y - y, NULL);
	LineTo(_dc, x, _resolution.y - y);
	MoveToEx(_dc, x, _resolution.y - y, NULL);
	LineTo(_dc,x,y);
	MoveToEx(_dc, x, y, NULL);
	LineTo(_dc, _resolution.x - x, _resolution.y - y);
	MoveToEx(_dc, _resolution.x - x, y, NULL);
	LineTo(_dc,x, _resolution.y - y);







	x = 230;
	y = 110;
	boardinfo.left = x;
	boardinfo.top = y;
	boardinfo.right = _resolution.x - x;
	boardinfo.bottom = _resolution.y - y;
	return boardinfo;

}
POINT* focus_return(RECT _rect)
{
	POINT cpos[2];
	POINT _fpos;
	POINT _epos;
	_fpos.x = _rect.left+50;
	_fpos.y = _rect.top+50;
	_epos.x = _rect.right-50;
	_epos.y = _rect.bottom-50;
	cpos[0] = _fpos;
	cpos[1] = _epos;
	return cpos;
}
POINT* fentagone_return(POINT _cPos, int _r)
{
	POINT fentagone[5];
	for (int i = 0; i < 5; ++i)
	{
		double angle = (2 * PI * i / 5) - (PI / 2);
		fentagone[i].x = _cPos.x + _r * cos(angle);
		fentagone[i].y = _cPos.y + _r * sin(angle);
	}
	return fentagone;
}
POINT* octagone_retrun(POINT _cPos, POINT _scale)
{
	POINT octagone[6];
	for (int i = 0; i < 6; ++i)
	{
		double angle = (2 * PI * i / 6) - (PI / 2);
		octagone[i].x = _cPos.x + _scale.x * cos(angle);
		octagone[i].y = _cPos.y + _scale.y * sin(angle);
	}
	return octagone;
}

bool operator<=(const POINT& _one, const POINT& _other) // 논리연산일 때는 비교 대상이 되는 매개변수 2개가 있어야한다.
{
	return _one.x <= _other.x && _one.y <= _other.y;
}
bool operator<(const POINT& _one, const POINT& _other) // 논리연산일 때는 비교 대상이 되는 매개변수 2개가 있어야한다.
{
	return _one.x < _other.x && _one.y < _other.y;
}
bool operator==(const POINT& _one, const POINT& _other) // 논리연산일 때는 비교 대상이 되는 매개변수 2개가 있어야한다.
{
	return _one.x == _other.x && _one.y == _other.y;
}
POINT operator+(const POINT& _one,float _other)
{
	return { _one.x + (long)_other,_one.y + (long)_other };
}
//--- (x1, y1)과 (x2, y2)간의 길이
float LengthPts(int x1, int y1, int x2, int y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}
//--- 반지름
//--- (x1, y1)과 (x2, y2)의 길이가 반지름보다 짧으면 true, 아니면 false
BOOL InCircle(int x1, int y1, int x2, int y2,int BSIZE)
{
	if (LengthPts(x1, y1, x2, y2) < BSIZE)
		return TRUE;
	else
		return FALSE;
}
