#include <Windows.h>
#include <time.h>
#include "func.h"
#include "Object.h"
#include "define.h"
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Winodw Class Name"; // 클래스 이름 설정
LPCTSTR lpszWindowName = L"Window program 1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
// hInstance : 현재 실행중인 어플리케이션의 인스턴스핸들
// hPrevInstance : 동일한 어플리케이션이 실행중일 경우 이전에 실행된 프로그램의 인스턴스 핸들
// lpszCmdLine : 커멘드라인 상에서 프로그램 구동 시 전달된 문자열
// nCmdShow : 윈도우가 화면에 출력될 형태	\]
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpszCmdParam, _In_ int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = g_hInst;
	WndClass.lpfnWndProc = WndProc;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassExW(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL
		, 0, 0, 1280, 1080, NULL, (HMENU)NULL, g_hInst, 0);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);

	}
	return Message.wParam;
}

// hWnd : 메세지가 발생한 윈도우 핸들
// uMessage : 발생한 메세지 (종류)
// wParam, lpParam : 메세지에 따라 추가적인 정보를 전달하기 위한 매개변수
void passengerupdate(Object& _passenger, int& plocate, POINT _fPos, POINT _ePos, COLORREF _wfcolor, COLORREF _hfcolor, bool selection);
void initObject(Object& _object, POINT _pos, POINT _scale, int _movetype, int _objecttype, int _vel, COLORREF _color);
void moveObject(Object& _object, int _movetype, POINT _fPos, POINT _ePos);
void jumpObject(Object& _object, int _worh);
bool collidercheck(Object* _car, Object _frontcar, POINT fPos, POINT ePos,int _idx);
void carrendr(HDC _dc, Object& _car, POINT _fPos, POINT _ePos);
void carupdate(Object* _car, POINT _fPos, POINT _ePos, COLORREF _wcolor, COLORREF _hcolor, RECT _crossroad, bool _selection);
bool crosscheck(Object& _car, RECT crossroad);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	srand(time(NULL));
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	static RECT boardinfo, carcross;
	static POINT resolution, fPos, ePos;
	static Object* Car;
	static Object Passenger;
	static Object WTrafficlight;
	static Object HTrafficlight;
	static bool is_init, is_start,wSelection,hSelection,a_Tap,lselection,Rselection;
	static int playerthrow, Aithrow, playeridx, aiidx,signal,mx,my,vel,passengerlocate;
	switch (uMessage)
	{
	case WM_CREATE:			// 윈도우가 생성될때 발생하는 메세지, 초기화 작업을 수행
		resolution.x = 1280;
		resolution.y = 1080;
		is_init = false;
		is_start = false;
		wSelection = false;
		hSelection = false;
		lselection = false;
		Rselection = false;
		a_Tap = false;
		Car = (Object*)malloc(sizeof(Object) * 16);
		playerthrow = 0;
		Aithrow = 0;
		playeridx = 1;
		aiidx = 1;
		signal = 0;
		passengerlocate = 0;
		vel = 250;
		SetTimer(hWnd, 1, vel, NULL);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		{
			boardinfo = makeboard(hDC, resolution);
			carcross.left = boardinfo.left + 200;
			carcross.top = boardinfo.top + 200;
			carcross.right = boardinfo.right - 200;
			carcross.bottom = boardinfo.bottom - 200;
			fPos = focus_return(boardinfo)[0];
			ePos = focus_return(boardinfo)[1];
			hPen = CreatePen(PS_SOLID, 2, RGB(255, 205, 10));
			oldPen = (HPEN)SelectObject(hDC, hPen);
			MoveToEx(hDC, (fPos.x-50), (fPos.y-50) + ((((ePos.y+50) - (fPos.y - 50))) / 2), NULL);
			LineTo(hDC, (ePos.x+50), (fPos.y-50) + ((((ePos.y + 50) - (fPos.y - 50))) / 2));
			MoveToEx(hDC, (fPos.x - 50) + ((((ePos.x + 50) - (fPos.x - 50)) / 2)), (fPos.y - 50), NULL);
			LineTo(hDC, (fPos.x - 50) + ((((ePos.x + 50) - (fPos.x - 50)) / 2)), (ePos.y + 50));
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
		}

		if (is_init == false)
		{
			is_init = true;
			int count = 0;
			while (count <8)
			{
				if (count < 2)
				{
					POINT cPos = fPos;
					cPos.y += 300;
					cPos.x += 100 * count;
					initObject(Car[count], cPos, { 40,20 },RIGHT,0, (rand() % 5 +25), RGB(0, 0, 0));
				}
				if (2 <= count && count < 4)
				{
					POINT cPos = ePos;
					cPos.y -= 300;
					cPos.x -= 100 * (count - 2);
					initObject(Car[count], cPos, { 40,20 }, LEFT,0, (rand() % 5 + 25), RGB(0, 0, 0));
					
				}
				if (4 <= count && count < 6)
				{
					POINT cPos = fPos;
					cPos.y += 100 * (count -4);
					cPos.x += 200;
					initObject(Car[count], cPos, { 20,40 }, DOWN,0, (rand() % 5 + 25), RGB(0, 0, 0));
				
				}
				if (6 <= count && count < 8)
				{
					POINT cPos = ePos;
					cPos.y -= 100 * (count - 6);
					cPos.x -= 200;
					initObject(Car[count], cPos, { 20,40 }, UP,0, (rand() % 5 + 25), RGB(0, 0, 0));
				}
				++count;
			}
			POINT twPos = fPos;
			twPos.x += 100;
			initObject(WTrafficlight, twPos, { 20,20 }, UP, 1, 0, RGB(255, 0, 0));
			POINT thPos = ePos;
			thPos.y -= 100;
			
			initObject(HTrafficlight, thPos, { 20,20 }, UP, 1, 0, RGB(0, 0, 255));
			POINT pPos = fPos;
			pPos.x += 100;
			pPos.y += 400;
			initObject(Passenger, pPos, { 20,20 }, UP, 1, 25, RGB(255, 100, 50));
		}
		for (int i = 0; i < 8; ++i)
		{
			hPen = CreatePen(PS_SOLID, 2, Car[i].getcolor());
			oldPen = (HPEN)SelectObject(hDC, hPen);
			carrendr(hDC, Car[i], fPos, ePos);
			SelectObject(hDC, oldPen);
			DeleteObject(hPen);
		}
		
		hBrush = CreateSolidBrush(WTrafficlight.getcolor());
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		WTrafficlight.render(hDC);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(HTrafficlight.getcolor());
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		HTrafficlight.render(hDC);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(Passenger.getcolor());
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Passenger.render(hDC);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'a':
		{
			if (a_Tap == false)
			{
				a_Tap = true;
			}
			else
			{
				a_Tap = false;
			}

		}
		break;
		case '+':
		{
			vel -= 50;
			SetTimer(hWnd, 1, vel, NULL);
		}
			break;
		case '-':
		{
			vel += 50;
			SetTimer(hWnd, 1, vel, NULL);
		}
		break;
		case 'q':
		{
			if (!(Car == nullptr))
			{
				free(Car);
			}
			PostQuitMessage(0);	// 윈도우를 종료하는 메세지
		}
			break;
		}

		InvalidateRect(hWnd, &boardinfo, true);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		POINT wPos = WTrafficlight.getPos();
		if (InCircle(wPos.x, wPos.y, mx, my, WTrafficlight.getscale().x))
		{
			WTrafficlight.switchsignal(WTrafficlight.getcolor());
		}
		if (InCircle(wPos.x, wPos.y, mx, my, HTrafficlight.getscale().x))
		{
			HTrafficlight.switchsignal(HTrafficlight.getcolor());
		}
		else
		{
			
			lselection = true;
		}
		InvalidateRect(hWnd, &boardinfo, true);
		break;
	case WM_RBUTTONDOWN:
	{
		Rselection = true;
		
	}
		
		
		InvalidateRect(hWnd, &boardinfo, true);
		break;
	case WM_TIMER:
		if (lselection == true)
		{
			bool check = false;
			RECT car;
			RECT nullrect;
			
			for (int i = 0; i < 8; ++i)
			{
				car.left = Car[i].getPos().x - Car[i].getscale().x;
				car.top = Car[i].getPos().y - Car[i].getscale().y;
				car.right = Car[i].getPos().x + Car[i].getscale().x;
				car.bottom = Car[i].getPos().y + Car[i].getscale().y;
				if (IntersectRect(&nullrect,&car,&carcross))
				{
					check = true;
				}
			}
			if (check == false)
			{
				a_Tap = false;
				WTrafficlight.setcolor(RGB(255, 255, 0));
				HTrafficlight.setcolor(RGB(255, 255, 0));
			}
			
		}
		if(Rselection == true)
		{
			RECT nullrect;
			RECT passrect;
			passrect.left = Passenger.getPos().x - Passenger.getscale().x;
			passrect.top = Passenger.getPos().y - Passenger.getscale().y;
			passrect.right = Passenger.getPos().x + Passenger.getscale().x;
			passrect.bottom = Passenger.getPos().y + Passenger.getscale().y;
			if (IntersectRect(&nullrect, &passrect, &carcross) == false)
			{
				POINT LD = fPos;
				LD.x += 100;
				LD.y += 400;
				POINT LU = fPos;
				LU.x += 100;
				LU.y += 100;
				POINT RU = ePos;
				RU.x -= 100;
				RU.y -= 400;
				POINT RD = ePos;
				RD.x -= 100;
				RD.y -= 100;
				switch (passengerlocate)
				{
				case 0:
					Passenger.setPos(RU);
					break;
				case 1:
					Passenger.setPos(RD);
					break;
				case 2:
					Passenger.setPos(LD);
					break;
				case 3:
					Passenger.setPos(LU);
					break;
				}
				WTrafficlight.setcolor(RGB(255, 0, 0));
				HTrafficlight.setcolor(RGB(0, 0, 255));
				Rselection = false;
				lselection = false;
			}
		}
		if (a_Tap == true)
		{
			if (signal % 12 == 0 && signal != 0)
			{
				WTrafficlight.switchsignal(WTrafficlight.getcolor());
				HTrafficlight.switchsignal(HTrafficlight.getcolor());

			}
			++signal;
		}
		carupdate(Car, fPos, ePos, HTrafficlight.getcolor(), WTrafficlight.getcolor(), carcross,lselection);
		passengerupdate(Passenger, passengerlocate,fPos, ePos, HTrafficlight.getcolor(), WTrafficlight.getcolor(),lselection);
		
		InvalidateRect(hWnd,&boardinfo, true);
		break;
	case WM_DESTROY:
		if (!(Car == nullptr))
		{
			free(Car);
		}
		PostQuitMessage(0);	// 윈도우를 종료하는 메세지
		break;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}
void passengerupdate(Object& _passenger,int& plocate, POINT _fPos, POINT _ePos, COLORREF _wfcolor, COLORREF _hfcolor,bool selection)
{
	POINT LD = _fPos;
	LD.x += 100;
	LD.y += 400;
	POINT LU = _fPos;
	LU.x += 100;
	LU.y += 100;
	POINT RU = _ePos;
	RU.x -= 100;
	RU.y -= 400;
	POINT RD = _ePos;
	RD.x -= 100;
	RD.y -= 100;
	if (LD == _passenger.getPos())
	{
		plocate = 0;
	}
	if (LU == _passenger.getPos())
	{
		plocate = 1;
	}
	if (RU == _passenger.getPos())
	{
		plocate = 2;
	}
	if (RD == _passenger.getPos())
	{
		plocate = 3;
	}
	if (selection == false)
	{
		if (plocate == 0 && (LD.y >= _passenger.getPos().y && _passenger.getPos().y > LU.y) && _wfcolor == RGB(255, 0, 0))
		{
			_passenger.setmovetype(UP);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 1 && (LU.x <= _passenger.getPos().x && _passenger.getPos().x < RU.x) && _hfcolor == RGB(255, 0, 0))
		{
			_passenger.setmovetype(RIGHT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 2 && (RU.y <= _passenger.getPos().y && _passenger.getPos().y < RD.y) && _wfcolor == RGB(255, 0, 0))
		{
			_passenger.setmovetype(DOWN);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 3 && (RD.x >= _passenger.getPos().x && _passenger.getPos().x > LD.x) && _hfcolor == RGB(255, 0, 0))
		{
			_passenger.setmovetype(LEFT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
	
	}
	else
	{
		if (plocate == 0 &&
			(LD.x <= _passenger.getPos().x && _passenger.getPos().x < RU.x)
			&& (LD.y >= _passenger.getPos().y && _passenger.getPos().y > RU.y) && _wfcolor == RGB(255, 255, 0))
		{
			_passenger.setmovetype(UP);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
			_passenger.setmovetype(RIGHT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 1 &&(LU.x <= _passenger.getPos().x && _passenger.getPos().x < RD.x && _wfcolor == RGB(255, 255, 0))
			&& (LU.y <= _passenger.getPos().y && _passenger.getPos().y < RD.y))
		{
			_passenger.setmovetype(DOWN);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
			_passenger.setmovetype(RIGHT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 3 && (RD.x >= _passenger.getPos().x && _passenger.getPos().x > LU.x && _wfcolor == RGB(255, 255, 0))
			&& (RD.y >= _passenger.getPos().y && _passenger.getPos().y > LU.y))
		{
			_passenger.setmovetype(UP);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
			_passenger.setmovetype(LEFT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
		if (plocate == 2 && (RU.x >= _passenger.getPos().x && _passenger.getPos().x > LD.x && _wfcolor == RGB(255, 255, 0))
			&& (RU.y <= _passenger.getPos().y && _passenger.getPos().y < LD.y))
		{
			_passenger.setmovetype(DOWN);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
			_passenger.setmovetype(LEFT);
			moveObject(_passenger, _passenger.getmovetype(), _fPos, _ePos);
		}
	}





}

void carrendr(HDC _dc,Object& _car, POINT _fPos, POINT _ePos)
{
	int type = _car.getmovetype();
	switch (type)
	{
	case LEFT:
	{
		{
			POINT cPos = _car.getPos();
			POINT cScale = _car.getscale();

			if (cPos.x - cScale.x < _fPos.x)
			{
				POINT cpPos = _car.getPos();
				POINT cpscale = _car.getscale();
				cpPos.x += (_ePos.x + 50) - (_fPos.x - 50);

				Rectangle(_dc, cpPos.x - cpscale.x, cpPos.y - cpscale.y
					, cpPos.x + cpscale.x, cpPos.y + cpscale.y);
				_car.render(_dc);
			}
			else
			{
				_car.render(_dc);
			}
		}
		
	}
		break;
	case RIGHT:
	{
		{
			POINT cPos = _car.getPos();
			POINT cScale = _car.getscale();

			if (cPos.x + cScale.x > _ePos.x)
			{
				POINT cpPos = _car.getPos();
				POINT cpscale = _car.getscale();
				cpPos.x -= (_ePos.x+50) - (_fPos.x-50);

				Rectangle(_dc, cpPos.x - cpscale.x, cpPos.y - cpscale.y
					, cpPos.x + cpscale.x, cpPos.y + cpscale.y);
				_car.render(_dc);
			}
			else
			{
				_car.render(_dc);
			}
		}
		
	}
		break;
	case DOWN:
	{
		POINT cPos = _car.getPos();
		POINT cScale = _car.getscale();

		if (cPos.y + cScale.y > _ePos.y)
		{
			POINT cpPos = _car.getPos();
			POINT cpscale = _car.getscale();
			cpPos.y -= (_ePos.x + 50) - (_fPos.x - 50);

			Rectangle(_dc, cpPos.x - cpscale.x, cpPos.y - cpscale.y
				, cpPos.x + cpscale.x, cpPos.y + cpscale.y);
			_car.render(_dc);
		}
		else
		{
			_car.render(_dc);
		}
	}
		
		break;
	case UP:
	{
		POINT cPos = _car.getPos();
		POINT cScale = _car.getscale();

		if (cPos.y - cScale.y > _fPos.y)
		{
			POINT cpPos = _car.getPos();
			POINT cpscale = _car.getscale();
			cpPos.y += (_ePos.x + 50) - (_fPos.x - 50);

			Rectangle(_dc, cpPos.x - cpscale.x, cpPos.y - cpscale.y
				, cpPos.x + cpscale.x, cpPos.y + cpscale.y);
			_car.render(_dc);
		}
		else
		{
			_car.render(_dc);
		}
	}
		
		break;
	}
}
void initObject(Object& _object, POINT _pos, POINT _scale,int _movetype,int _objecttype, int _vel,COLORREF _color)
{
	_object.setPos(_pos);
	_object.setcolor(_color);
	_object.setscale(_scale);
	_object.setmovetype(_movetype);
	_object.setvel(_vel);
	_object.setobjecttype(_objecttype);
	

}
void moveObject(Object& _object, int _movetype, POINT _fPos, POINT _ePos)
{
	switch (_movetype)
	{
	case UP:
	{
		POINT cPos = _object.getPos();
		if (cPos.y - (_fPos.y - 50) <= 40)
		{
			
		}
		cPos.y -= _object.getvel();
		_object.setPos(cPos);
	}
	
		break;
	case DOWN:
	{
		POINT cPos = _object.getPos();
		if ((_ePos.y + 50)-cPos.y <= 40)
		{
			
		}
		cPos.y += _object.getvel();
		_object.setPos(cPos);
	}
		break;
	case LEFT:
	{
		POINT cPos = _object.getPos();
		
		cPos.x -= _object.getvel();
		_object.setPos(cPos);
	}
		break;
	case RIGHT:
	{
		POINT cPos = _object.getPos();
		cPos.x += _object.getvel();
		_object.setPos(cPos);
	}
		break;
	}
}
void jumpObject(Object& _object, int _worh)
{
	
}
bool collidercheck(Object* _car, Object _backcar,POINT _fPos,POINT _ePos,int _idx)
{
	int type = _backcar.getmovetype();
	for (int i = 0; i < 8; ++i)
	{
		if (i != _idx)
		{
			switch (type)
			{
			case LEFT:
			{
				if (_car[i].getmovetype() == LEFT)
				{
					POINT fcPos = _car[i].getPos();
					POINT fcScale = _car[i].getscale();
					POINT bcPos = _backcar.getPos();
					POINT bcScale = _backcar.getscale();
					int xlength = bcPos.x - fcPos.x;
					if (xlength >= 0 && abs(xlength) < 10+(bcScale.x * 2))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				
			}
				break;
			case RIGHT:
			{
				if (_car[i].getmovetype() == RIGHT)
				{
					POINT fcPos = _car[i].getPos();
					POINT fcScale = _car[i].getscale();
					POINT bcPos = _backcar.getPos();
					POINT bcScale = _backcar.getscale();
					int xlength = fcPos.x - bcPos.x;
					if (xlength >= 0 &&abs(xlength) < 10 + (bcScale.x * 2))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				
			}
				break;
			case DOWN:
			{
				if (_car[i].getmovetype() == DOWN)
				{
					POINT fcPos = _car[i].getPos();
					POINT fcScale = _car[i].getscale();
					POINT bcPos = _backcar.getPos();
					POINT bcScale = _backcar.getscale();
					int ylength = fcPos.y - bcPos.y;
					if (ylength >= 0 && abs(ylength) < 10 + (bcScale.y * 2))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			
			}
				break;
			case UP:
				if (_car[i].getmovetype() == UP)
				{
					POINT fcPos = _car[i].getPos();
					POINT fcScale = _car[i].getscale();
					POINT bcPos = _backcar.getPos();
					POINT bcScale = _backcar.getscale();
					int ylength = bcPos.y - fcPos.y;
					if (ylength >= 0 && abs(ylength) < 10 + (bcScale.y * 2))
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				
				break;
			}
			
		
		}
	}
	
}
bool IsOutofBounds(Object _car, POINT _fPos, POINT _ePos)
{
	int type = _car.getmovetype();
	switch (type)
	{
	case LEFT:
		if (_car.getPos().x + _car.getscale().x < _fPos.x - 50)
		{
			return true;
		}
		return false;
		break;
	case RIGHT:
		if (_car.getPos().x - _car.getscale().x > _ePos.x + 50)
		{
			return true;
		}
		return false;
		break;
	case UP:
		if (_car.getPos().y + _car.getscale().y < _fPos.y - 50)
		{
			return true;
		}
		return false;
		break;
	case DOWN:
		if (_car.getPos().y - _car.getscale().y > _ePos.y + 50)
		{
			return true;
		}
		return false;
		break;
	}
}
void relocatecar(Object& _car, POINT _fPos, POINT _ePos)
{
	int type = _car.getmovetype();
	switch (type)
	{
	case LEFT:
	{
		POINT cPos = _car.getPos();
		cPos.x += (_ePos.x + 50) - (_fPos.x - 50);
		_car.setPos(cPos);
	}
		break;
	case RIGHT:
	{
		POINT cPos = _car.getPos();
		cPos.x -= (_ePos.x + 50) - (_fPos.x - 50);
		_car.setPos(cPos);
	}
		break;
	case UP:
	{
		POINT cPos = _car.getPos();
		cPos.y += (_ePos.x + 50) - (_fPos.x - 50);
		_car.setPos(cPos);
	}
		break;
	case DOWN:
	{
		POINT cPos = _car.getPos();
		cPos.y -= (_ePos.x + 50) - (_fPos.x - 50);
		_car.setPos(cPos);
	}
		break;
	}
}
bool crosscheck(Object& _car, RECT crossroad)
{
	RECT car;
	RECT nullrect;
	int type = _car.getmovetype();
	car.left = _car.getPos().x - _car.getscale().x;
	car.top = _car.getPos().y - _car.getscale().y;
	car.right = _car.getPos().x + _car.getscale().x;
	car.bottom = _car.getPos().y + _car.getscale().y;
	switch (type)
	{
	case LEFT:
		crossroad.left -= 100;
		break;
	case RIGHT:
		crossroad.right += 100;
		break;
	case DOWN:
		crossroad.bottom += 100;
		break;
	case UP:
		crossroad.top -= 100;
		break;
	}

	if (IntersectRect(&nullrect, &car, &crossroad))
	{
		return true;
	}
	return false;
}
void carupdate(Object* _car, POINT _fPos, POINT _ePos, COLORREF _wcolor, COLORREF _hcolor, RECT _crossroad, bool _selection)
{
	
	for (int i = 0; i < 8; ++i)
		{
			int type = _car[i].getmovetype();

			if (i < 4 && _wcolor == RGB(0, 0, 255))
			{
				if (collidercheck(_car, _car[i], _fPos, _ePos, i))
				{

				}
				else
				{
					moveObject(_car[i], type, _fPos, _ePos);
				}
				if (IsOutofBounds(_car[i], _fPos, _ePos))
				{
					relocatecar(_car[i], _fPos, _ePos);
				}
			}
			else if (i < 4 && _wcolor == RGB(255, 255, 0))
			{

				if (collidercheck(_car, _car[i], _fPos, _ePos, i))
				{

				}
				else
				{
					if (crosscheck(_car[i], _crossroad))
					{
						moveObject(_car[i], type, _fPos, _ePos);
					}

				}
				if (IsOutofBounds(_car[i], _fPos, _ePos))
				{
					relocatecar(_car[i], _fPos, _ePos);
				}
			}
			else if (4 <= i && i <= 8 && _hcolor == RGB(0, 0, 255))
			{
				if (collidercheck(_car, _car[i], _fPos, _ePos, i))
				{
				}
				else
				{

					moveObject(_car[i], type, _fPos, _ePos);

				}
				if (IsOutofBounds(_car[i], _fPos, _ePos))
				{
					relocatecar(_car[i], _fPos, _ePos);
				}
			}
			else if (4 <= i && i <= 8 && _hcolor == RGB(255, 255, 0))
			{
				if (collidercheck(_car, _car[i], _fPos, _ePos, i))
				{
				}
				else
				{
					if (crosscheck(_car[i], _crossroad))
					{
						moveObject(_car[i], type, _fPos, _ePos);
					}
				}
				if (IsOutofBounds(_car[i], _fPos, _ePos))
				{
					relocatecar(_car[i], _fPos, _ePos);
				}
			}
		}
	
	
	
	

}
