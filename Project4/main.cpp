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
void initObject(Object& _object, POINT _pos, POINT _scale);
void moveObject(Object& _object, int _worh, bool _jump, POINT _fPos, POINT _ePos);
void jumpObject(Object& _object, int _worh);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	srand(time(NULL));
	HDC hDC;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	static RECT boardinfo;
	static POINT resolution,fPos,ePos;
	static Object* Player;
	static Object Playermal;
	static Object Aimal;
	static bool is_init,is_start,is_width,is_height,is_jump;
	static int playerthrow,Aithrow,playeridx,aiidx;
	switch (uMessage)
	{
	case WM_CREATE:			// 윈도우가 생성될때 발생하는 메세지, 초기화 작업을 수행
		resolution.x = 1280;
		resolution.y = 1080;
		is_init = false;
		is_start = false;
		Player = (Object*)malloc(sizeof(Object) * 30);
		playerthrow = 0;
		Aithrow = 0;
		playeridx = 1;
		aiidx = 1;
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		boardinfo = makeYuutboard(hDC, resolution);
		fPos = focus_return(boardinfo)[0];
		ePos = focus_return(boardinfo)[1];
		if (is_init == false)
		{
			is_init = true;
			int count = 0;
			while (count != 30)
			{
				if (count < 6)
				{
					if (count % 5 == 0)
					{
						initObject(Player[count], { fPos.x + (count * 164),ePos.y }, { 40,40 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
					else
					{
						initObject(Player[count], { fPos.x + (count * 164),ePos.y }, { 20,20 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
				}
				if (6 <= count && count < 11)
				{
					if (count % 5 == 0)
					{
						initObject(Player[count], { ePos.x,ePos.y - ((count-5)*172)}, {40,40});
						Player[count].setcolor(RGB(255, 255, 255));
					}
					else
					{
						initObject(Player[count], { ePos.x,ePos.y - ((count-5) * 172) }, { 20,20 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
				}
				if (11 <= count && count < 16)
				{
					if (count % 5 == 0)
					{
						initObject(Player[count], { ePos.x-((count-10)*164),fPos.y}, {40,40});
						Player[count].setcolor(RGB(255, 255, 255));
					}
					else
					{
						initObject(Player[count], { ePos.x - ((count-10) * 164),fPos.y }, { 20,20 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
				}
				if (16 <= count && count < 20)
				{
					if (count % 5 == 0)
					{
						initObject(Player[count], { fPos.x,fPos.y + ((count-15) * 172) }, { 40,40 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
					else
					{
						initObject(Player[count], { fPos.x,fPos.y + ((count-15) * 172) }, { 20,20 });
						Player[count].setcolor(RGB(255, 255, 255));
					}
				}
				if (20 <= count && count < 22)
				{
					initObject(Player[count], { ePos.x - ((count - 19) * 164),fPos.y + ((count - 19) * 172) }, { 20,20 });
					Player[count].setcolor(RGB(255, 255, 255));
				}
				if (22 <= count && count < 24)
				{
					initObject(Player[count], { ePos.x - ((count - 21) * 164),ePos.y - ((count - 21) * 172) }, { 20,20 });
					Player[count].setcolor(RGB(255, 255, 255));
				}
				if (24 == count)
				{
					initObject(Player[24], { fPos.x + ((25 - 23) * 164),fPos.y + ((25 - 23) * 172) }, { 20,20 });
					initObject(Player[25], { fPos.x + ((24 - 23) * 164),fPos.y + ((24 - 23) * 172) }, { 20,20 });
					Player[24].setcolor(RGB(255, 255, 255));
					Player[25].setcolor(RGB(255, 255, 255));
				}
				if (26 == count)
				{
					initObject(Player[26], { fPos.x + ((27 - 25) * 164),ePos.y - ((27 - 25) * 172) }, { 20,20 });
					initObject(Player[27], { fPos.x + ((26 - 25) * 164),ePos.y - ((26 - 25) * 172) }, { 20,20 });
					Player[26].setcolor(RGB(255, 255, 255));
					Player[27].setcolor(RGB(255, 255, 255));
				}
				if (count == 28)
				{
					initObject(Player[count], { fPos.x + ((ePos.x - fPos.x) / 2),fPos.y + ((ePos.y - fPos.y) / 2) }, { 40,40 });
					Player[count].setcolor(RGB(255, 255, 255));
				}
				++count;
			}
			initObject(Playermal, { 10,10 }, { 20,20 });
			Playermal.setcolor(RGB(0, 0, 255));
			initObject(Aimal, { 40,10 }, { 20,20 });
			Aimal.setcolor(RGB(255, 0, 0));
			
		}

		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		for (int i = 0; i < 30; ++i)
		{
			hBrush = CreateSolidBrush(Player[i].getcolor());
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Player[i].render(hDC);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}
		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(Playermal.getcolor());
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Playermal.render(hDC);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);
		oldPen = (HPEN)SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(Aimal.getcolor());
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Aimal.render(hDC);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		SelectObject(hDC, oldPen);
		DeleteObject(hPen);
		if (Playermal.getPos() == Player[0].getPos())
		{
			int a = 0;
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'p':
			is_start = true;
			break;
		case '1':
			if (is_start == true)
			{
				playerthrow = rand() % 4 + 1;
				SetTimer(hWnd, 1, 200, NULL);
			}
			break;
		case '2':
			if (is_start == true)
			{
				Aithrow = rand() % 4 + 1;
				SetTimer(hWnd, 2, 200, NULL);
			}
			break;
		}
		
		break;
	case WM_KEYDOWN:
		
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (playerthrow == 0)
			{
				if (playeridx == 6)
				{
					playeridx = 22;
					
				}
				if (playeridx == 11)
				{
					playeridx = 20;
					
				}
				if (playeridx == 28)
				{
					Playermal.setPos(Player[playeridx].getPos());
					playeridx = 26;
					InvalidateRect(hWnd, NULL, true);
				}
				KillTimer(hWnd, 1);
				break;
			}

			if (playeridx == 19)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 0;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (playeridx == 27)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 0;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (playeridx == 21)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 28;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (playeridx == 23)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 28;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				
				break;
			}
			else if (playeridx == 28)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 24;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				
				break;
			}
			else if (playeridx == 25)
			{
				Playermal.setPos(Player[playeridx].getPos());
				playeridx = 15;
				--playerthrow;
				InvalidateRect(hWnd, NULL, true);
				
				break;
			}
		
	
			Playermal.setPos(Player[playeridx++].getPos());
			--playerthrow;
			
			InvalidateRect(hWnd, NULL, true);
			break;
		case 2:
			if (Aithrow == 0)
			{
				if (aiidx == 6)
				{
					
					aiidx = 22;
					
				}
				if (aiidx == 11)
				{
					
					aiidx = 20;
					
				}
				if (aiidx == 28)
				{
					Aimal.setPos(Player[aiidx].getPos());
					aiidx = 26;
					InvalidateRect(hWnd, NULL, true);
				}
				KillTimer(hWnd, 2);
				break;
			}

			if (aiidx == 19)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 0;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (aiidx == 27)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 0;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (aiidx == 21)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 28;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
			else if (aiidx == 23)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 28;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);

				break;
			}
			else if (aiidx == 28)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 24;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);

				break;
			}
			else if (aiidx == 25)
			{
				Aimal.setPos(Player[aiidx].getPos());
				aiidx = 15;
				--Aithrow;
				InvalidateRect(hWnd, NULL, true);

				break;
			}


			Aimal.setPos(Player[aiidx++].getPos());
			--Aithrow;

			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);	// 윈도우를 종료하는 메세지
		break;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}
void initObject(Object& _object, POINT _pos, POINT _scale)
{
	_object.setPos(_pos);
	_object.setscale(_scale);
	_object.setcolor(RGB(rand() % 255, rand() % 255, rand() % 255));

}
void moveObject(Object& _object, int _worh,bool _jump,POINT _fPos,POINT _ePos)
{
	if (_worh == WIDTH)
	{
		
		POINT pPos = _object.getPos();
		if (_jump == true)
		{
			pPos.y += _object.getvel() * _object.getydir();
			
		}
		if (pPos.x >= _ePos.x || pPos.x <= _fPos.x)
		{
			_object.switchdir(X);
			if (pPos.y >= _ePos.y || pPos.y <= _fPos.y)
			{
				_object.switchdir(Y);
			}
			pPos.y += _object.getvel() * _object.getydir();

		}
		pPos.x += _object.getvel() * _object.getxdir();
		_object.setPos(pPos);
	}
	else if (_worh == HEIGHT)
	{
		POINT pPos = _object.getPos();
		if (_jump == true)
		{
			pPos.x += _object.getvel() * _object.getxdir();
		}
		if (pPos.y >= _ePos.y || pPos.y <= _fPos.y)
		{
			_object.switchdir(Y);
			if (pPos.x >= _ePos.x || pPos.x <= _fPos.x)
			{
				_object.switchdir(X);
			}
			pPos.x += _object.getvel() * _object.getxdir();

		}

		pPos.y += _object.getvel() * _object.getydir();


		_object.setPos(pPos);
	}
}
void jumpObject(Object& _object, int _worh)
{
	if (_worh == WIDTH)
	{

		POINT pPos = _object.getPos();
		pPos.y -= _object.getvel() * _object.getydir();
		_object.setPos(pPos);
		
	}
	else if (_worh == HEIGHT)
	{
		
		POINT pPos = _object.getPos();
		pPos.x -= _object.getvel() * _object.getxdir();
		_object.setPos(pPos);
		
	}
}