﻿// CProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "CProject.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//플레이어 
RECT player;
RECT hitBox;

//외각선
RECT line;
//카메라
RECT camera;

RECT block;

bool crash = false;

//위치 이동
bool spacebar = true;

//타임
int g_timer;


//플레이어와 카메라 속도 맞춤
int speed = 4;


//플레이어와 카메라 좌표값 비교하여 카메라 이동
int cameraMove() {

    if( player.left - camera.left  > 150 )

        camera.left += speed;
        camera.right += speed;

    return 0;
}

//플레이어 이동
int playerMove() {

    if (spacebar == true) {
        player.top += speed;
        player.left += speed;
        player.bottom += speed;
        player.right += speed;
       }
    else if (spacebar == false) {
        player.top -= speed;
        player.left += speed;
        player.bottom -= speed;
        player.right += speed;

    }

    return 0;
}

int crashOn() {

   if (player.right > block.left && player.top < block.bottom && player.bottom>block.top)
        crash = true;
    return 0;

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_KEYDOWN:
	{
		switch (wParam) {
            //스페이스바 입력 눌린지 체크
			case VK_SPACE:
			{

				if (spacebar == false)
					spacebar = true;

				else 
                    spacebar = false;

			}
			break;
		}
		InvalidateRect(hWnd, NULL, true);
	}
	break;
		

	case WM_TIMER:
	{
        crashOn();

        if(crash == true)
		    KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, g_timer, NULL);
        //스페이스바를 눌렀다면 플레이어와 카메라 이동 방향 전환
		if (spacebar == true) {
            cameraMove();
            //플레이어가 외각선 하단에 위치했을때 속도를 더해서 외각선을 넘어가면 오른쪽으로만 이동
			if (player.bottom + speed > line.bottom)
			{
				player.left += speed;
				player.right += speed;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
            // 대각선 이동
            playerMove();

		}
		else if (spacebar == false) {
            cameraMove();
            //플레이어가 외각선 상단에 위치했을때 속도를 더해서 외각선을 넘어가면 오른쪽으로만 이동
			if (player.top - speed < line.top)
			{
				player.left += speed;
				player.right += speed;
				InvalidateRect(hWnd, NULL, true);
				break;
			}
            playerMove();

		}
	
		InvalidateRect(hWnd, NULL, true);
	}
	break;
	

	case WM_CREATE:
	{
        //전체적인 값 선언
		g_timer = 50;
		SetTimer(hWnd, 1, g_timer, NULL);
		player.top = 50;
		player.left = 50;
		player.bottom = 130;
		player.right = 130;

        hitBox.top = player.top;
        hitBox.right = player.right;
        hitBox.bottom = player.bottom;

		line.top = 10;
		line.left = 10;
		line.bottom = 400;
		line.right = 4000;

		camera.top = line.top;
		camera.left = line.top;
        camera.bottom = line.bottom;
        camera.right = 400;


        block.top = 300;
        block.left = 300;
        block.bottom = 400;
        block.right = 400;
	}
	break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			
           HDC hdc = BeginPaint(hWnd, &ps);
			
			Rectangle(hdc, line.left, line.top, line.right, line.bottom);

            Rectangle(hdc, block.left, block.top, block.right, block.bottom);
			
			Ellipse(hdc, player.left, player.top, player.right, player.bottom);

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
