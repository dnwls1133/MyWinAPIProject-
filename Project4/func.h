#pragma once
#include <Windows.h>
#include <cmath>
#define PI 3.1415926535
RECT makeboard(HDC _dc, POINT _resolution)
{
	RECT boardinfo;
	int x = 230;
	int y = 110;
	int bresoly = 0;
	int bresolx = 0;
	int count = 0;
	while (count < 83)
	{
		if (count < 41)
		{
			MoveToEx(_dc, x, y, NULL);
			LineTo(_dc, _resolution.x - x, y);
			y += 20;
			bresoly = y;
		}
		else
		{
			y = 110;
			MoveToEx(_dc, x, y, NULL);
			LineTo(_dc, x, bresoly - 20);
			x += 20;
			bresolx = x;
		}

		++count;
	}
	x = 230;
	y = 110;
	boardinfo.left = x;
	boardinfo.top = y;
	boardinfo.right = bresolx - 20;
	boardinfo.bottom = bresoly - 20;
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
	_fpos.x = _rect.left;
	_fpos.y = _rect.top;
	_epos.x = _rect.right;
	_epos.y = _rect.bottom;
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


bool operator==(const POINT& _one, const POINT& _other) // 논리연산일 때는 비교 대상이 되는 매개변수 2개가 있어야한다.
{
	return _one.x == _other.x && _one.y == _other.y;
}
POINT operator+(const POINT& _one,float _other)
{
	return { _one.x + (long)_other,_one.y + (long)_other };
}
