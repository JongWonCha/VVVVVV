#include <windows.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#define BLOCK_SIZE 15
#define JAGAL_GAP 30
#define RING_GAP 30
#define BLOCK_WIDTH 60
#define BLOCK_HEIGHT 30
#define WIDTH_NUM 64
#define HEIGHT_NUM 48

HINSTANCE g_hInst,g_hInst2;
LPCTSTR lpszClass = TEXT("윈플");

static char mapState[WIDTH_NUM][HEIGHT_NUM];
static char mapState_copy[WIDTH_NUM][HEIGHT_NUM];
void SaveMap(void);
void stage1(void);
void stage2(void);
void stage3(void);
void stage4(void);
void mystage(void);
void newstage(void);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW ;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,
		978, 780, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
typedef struct monster {
	int xPos, yPos;
	int flag;
}MONSTER;

static BOOL movement, stage_1, stage_2, stage_3, stage_4;
static int gameState;	//게임상태 0:정지 1:시작 -1:종료
int answer;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, hmemdc, hmemdc2;
	static HBITMAP hBitmap,hBitmap2;
	PAINTSTRUCT ps;
	HBRUSH hBrush, hOldBrush;
	BITMAP bmp , bmp2;

	static MONSTER ring[4];
	static MONSTER jagal[2];
	static MONSTER block[9];

	static char makeBlockID;
	static int mWidth, mHeight;
	static int mx, my, newmx, newmy;
	static BOOL Drag;

	switch (uMsg) {
	case WM_CREATE:
		gameState = 0;
		movement, stage_1, stage_2, stage_3, stage_4 = FALSE;
		SetTimer(hWnd, 1, 500, NULL);
		g_hInst = hInst;
		g_hInst2 = hInst;
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmap2 = LoadBitmap(g_hInst2, MAKEINTRESOURCE(IDB_BITMAP3));
		makeBlockID = '0';
		for (int j = 0; j < HEIGHT_NUM; ++j)
		{
			for (int i = 0; i < WIDTH_NUM; ++i)
			{
				mapState[i][j] = '0';
			}
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hmemdc = CreateCompatibleDC(hdc);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		mWidth = bmp.bmWidth;
		mHeight = bmp.bmHeight;
		SelectObject(hmemdc, hBitmap);
		hmemdc2 = CreateCompatibleDC(hdc);
		GetObject(hBitmap2, sizeof(BITMAP), &bmp2);
		SelectObject(hmemdc2, hBitmap2);



		for (int j = 0; j < HEIGHT_NUM; ++j)
		{
			for (int i = 0; i < WIDTH_NUM; ++i)
			{
				if (mapState[i][j] == '0')
				{
					Rectangle(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, (BLOCK_SIZE * i) + BLOCK_SIZE, BLOCK_SIZE * j + BLOCK_SIZE);
				}
				else if (mapState[i][j] == '1')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, 0, 0, mWidth / 9, mHeight, SRCCOPY);

				}
				else if (mapState[i][j] == '2')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				else if (mapState[i][j] == '3')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 2, 0, mWidth / 9, mHeight, SRCCOPY);
				}

				else if (mapState[i][j] == '4')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 3, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				else if (mapState[i][j] == '5')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 4, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				
				else if (mapState[i][j] == '6')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 5, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				else if (mapState[i][j] == '7')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 6, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				else if (mapState[i][j] == '8')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 7, 0, mWidth / 9, mHeight, SRCCOPY);
				}
				else if (mapState[i][j] == '9')
				{
					StretchBlt(hdc, BLOCK_SIZE * i, BLOCK_SIZE * j, 16, 16, hmemdc, mWidth / 9 * 8, 0, mWidth / 9, mHeight, SRCCOPY);
				}
			}
		}

		if (movement && gameState)
		{
			if (stage_1)
			{
				StretchBlt(hdc, block[0].xPos, block[0].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록		
				StretchBlt(hdc, block[1].xPos, block[1].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록		
			}
			else if (stage_2)
			{
				StretchBlt(hdc, ring[0].xPos, ring[0].yPos, 30, 30, hmemdc2, 64, 0, 64, 64, SRCCOPY);//링
				StretchBlt(hdc, ring[1].xPos, ring[1].yPos, 30, 30, hmemdc2, 64, 0, 64, 64, SRCCOPY);//링
				StretchBlt(hdc, ring[2].xPos, ring[2].yPos, 30, 30, hmemdc2, 64, 0, 64, 64, SRCCOPY);//링
			}
			else if (stage_3)
			{
				StretchBlt(hdc, block[2].xPos, block[2].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록		
				StretchBlt(hdc, block[3].xPos, block[3].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록	
				StretchBlt(hdc, block[4].xPos, block[4].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록		
				StretchBlt(hdc, block[5].xPos, block[5].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록	
				StretchBlt(hdc, block[6].xPos, block[6].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록	
			}
			else if (stage_4)
			{
				StretchBlt(hdc, jagal[0].xPos, jagal[0].yPos, 30, 30, hmemdc2, 0, 0, 64, 64, SRCCOPY);//자갈
				StretchBlt(hdc, jagal[1].xPos, jagal[1].yPos, 30, 30, hmemdc2,0, 0, 64, 64, SRCCOPY);//자갈
				StretchBlt(hdc, ring[3].xPos, ring[3].yPos, 30, 30, hmemdc2, 64, 0, 64, 64, SRCCOPY);//링
				StretchBlt(hdc, block[7].xPos, block[7].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록	
				StretchBlt(hdc, block[8].xPos, block[8].yPos, 60, 30, hmemdc2, 128, 0, 128, 64, SRCCOPY);//블록	
			}
		}

		DeleteDC(hmemdc);
		DeleteDC(hmemdc2);
		EndPaint(hWnd, &ps);
		break;
	
	case WM_KEYDOWN:
		//가시
		if (wParam == '1')
		{
			makeBlockID = '1';
		}
		else if (wParam == '2')
		{
			makeBlockID = '2';
		}
		else if(wParam == '2')
		{
			makeBlockID = '2';
		}
		else if (wParam == '3')
		{
			makeBlockID = '3';
		}
		else if(wParam == '4')
		{
			makeBlockID = '4';
		}

		//맵 색상
		else if (wParam == '5')
		{
			makeBlockID = '5';
		}
		else if (wParam == '6')
		{
			makeBlockID = '6';
		}
		else if (wParam == '7')
		{
			makeBlockID = '7';
		}
		else if (wParam == '8')
		{
			makeBlockID = '8';
		}
		else if (wParam == '9')
		{
			makeBlockID = '9';
		}

		//지우기
		else if (wParam == '0')
		{
			makeBlockID = '0';
		}

		if (wParam == VK_RETURN)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
		}
		break;

	case WM_LBUTTONDOWN:
		if (!(gameState))
		{
			Drag = TRUE;
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			mapState[mx / BLOCK_SIZE][my / BLOCK_SIZE] = makeBlockID;
			InvalidateRgn(hWnd, NULL, TRUE);
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_STAGE1:
			gameState = 0;
			stage1();
			stage_1 = TRUE;
			movement = TRUE;
			block[0].flag = 1;
			block[1].flag = -1;

			block[0].xPos = BLOCK_SIZE * 20;
			block[0].yPos = BLOCK_SIZE * 22;

			block[1].xPos = BLOCK_SIZE * 35;
			block[1].yPos = BLOCK_SIZE * 5;

			stage_2 = FALSE;
			stage_3 = FALSE;
			stage_4 = FALSE;
			break;

		case ID_STAGE2:
			gameState = 0;
			stage2();
			stage_2 = TRUE;
			movement = TRUE;
		
			ring[0].flag = 1;
			ring[1].flag = -1;
			ring[2].flag = 1;

			ring[0].xPos = BLOCK_SIZE * 42;
			ring[0].yPos = BLOCK_SIZE * 10;

			ring[1].xPos = BLOCK_SIZE * 39;
			ring[1].yPos = BLOCK_SIZE * 26;

			ring[2].xPos = BLOCK_SIZE * 23;
			ring[2].yPos = BLOCK_SIZE * 20;

			stage_1 = FALSE;
			stage_3 = FALSE;
			stage_4 = FALSE;
			break;

		case ID_STAGE3:
			gameState = 0;
			stage3();
			stage_3 = TRUE;
			movement = TRUE;

			block[2].flag = 1;
			block[3].flag = -1;
			block[4].flag = 1;
			block[5].flag = -1;
			block[6].flag = 1;


			block[2].xPos = BLOCK_SIZE * 6;
			block[2].yPos = BLOCK_SIZE * 15;

			block[3].xPos = BLOCK_SIZE * 22;
			block[3].yPos = BLOCK_SIZE * 35;

			block[4].xPos = BLOCK_SIZE * 21;
			block[4].yPos = BLOCK_SIZE * 15;

			block[5].xPos = BLOCK_SIZE * 43;
			block[5].yPos = BLOCK_SIZE * 35;

			block[6].xPos = BLOCK_SIZE * 38;
			block[6].yPos = BLOCK_SIZE * 12;

			stage_1 = FALSE;
			stage_2 = FALSE;
			stage_4 = FALSE;
			break;

		case ID_STAGE4:
			gameState = 0;
			stage4();
			stage_4 = TRUE;
			movement = TRUE;

			jagal[0].flag = 1;
			jagal[1].flag = -1;
			ring[3].flag = 1; 
			block[7].flag = 1;
			block[8].flag = 1;

			jagal[0].xPos = BLOCK_SIZE * 20;
			jagal[0].yPos = BLOCK_SIZE * 40;

			jagal[1].xPos = BLOCK_SIZE * 40;
			jagal[1].yPos = BLOCK_SIZE * 43;

			ring[3].xPos = BLOCK_SIZE * 53;
			ring[3].yPos = BLOCK_SIZE * 31;

			block[7].xPos = BLOCK_SIZE * 11;
			block[7].yPos = BLOCK_SIZE * 16;

			block[8].xPos = BLOCK_SIZE * 36;
			block[8].yPos = BLOCK_SIZE * 8;

			stage_1 = FALSE;
			stage_2 = FALSE;
			stage_3 = FALSE;
			break;

		case ID_MYSTAGE:
			gameState = 0;
			mystage();
			movement = TRUE;

			stage_1 = FALSE;
			stage_2 = FALSE;
			stage_3 = FALSE;
			stage_4 = FALSE;
			break;

		case ID_MAKESTAGE:
			gameState = 0;
			newstage();
			mystage();
			break;

		case ID_SAVESTAGE:
			SaveMap();
			break;
//-----------------------튜토리얼---------------------------------
		case ID_TUTORIAL:	

			break;
//----------------------------------------------------------------
		case ID_STARTGAME:
			gameState = 1;
			break;

		case ID_EXIT:
			answer = MessageBox(hWnd, "게임을 그만하실껀가요?ㅠ_ㅠ?", "VVVVVV", MB_OK);
			if (answer == 1)
				PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (stage_1)
			{
				if (block[0].xPos > BLOCK_SIZE * 37) block[0].flag = -1;
				else if (block[0].xPos < BLOCK_SIZE * 20) block[0].flag = 1;

				if (block[1].xPos > BLOCK_SIZE * 35) block[1].flag = -1;
				else if (block[1].xPos < BLOCK_SIZE * 17) block[1].flag = 1;

				block[0].xPos = block[0].xPos + 5 * block[0].flag;
				block[1].xPos = block[1].xPos + 10 * block[1].flag;
			}

			else if (stage_2)
			{
				if (ring[0].yPos < BLOCK_SIZE * 11) ring[0].flag = 1;
				else if (ring[0].yPos >  BLOCK_SIZE * 26) ring[0].flag = -1;

				if (ring[1].yPos < BLOCK_SIZE * 16) ring[1].flag = 1;
				else if (ring[1].yPos >  BLOCK_SIZE * 26) ring[1].flag = -1;

				if (ring[2].yPos < BLOCK_SIZE * 21) ring[2].flag = 1;
				else if (ring[2].yPos >  BLOCK_SIZE * 32) ring[2].flag = -1;

				ring[0].yPos = ring[0].yPos + 5 * ring[0].flag;
				ring[1].yPos = ring[1].yPos + 5 * ring[1].flag;
				ring[2].yPos = ring[2].yPos + 10 * ring[2].flag;
			}

			else if (stage_3)
			{
				if (block[2].xPos > BLOCK_SIZE * 12) block[2].flag = -1;
				else if (block[2].xPos < BLOCK_SIZE * 7) block[2].flag = 1;

				if (block[3].xPos > BLOCK_SIZE * 21) block[3].flag = -1;
				else if (block[3].xPos < BLOCK_SIZE * 11) block[3].flag = 1;

				if (block[4].xPos > BLOCK_SIZE * 29) block[4].flag = -1;
				else if (block[4].xPos < BLOCK_SIZE * 22) block[4].flag = 1;
				
				if (block[5].xPos > BLOCK_SIZE * 42) block[5].flag = -1;
				else if (block[5].xPos < BLOCK_SIZE * 32) block[5].flag = 1;
				
				if (block[6].xPos > BLOCK_SIZE * 52) block[6].flag = -1;
				else if (block[6].xPos < BLOCK_SIZE * 37) block[6].flag = 1;

				block[2].xPos = block[2].xPos + 5 * block[2].flag;
				block[3].xPos = block[3].xPos + 5 * block[3].flag;
				block[4].xPos = block[4].xPos + 5 * block[4].flag;
				block[5].xPos = block[5].xPos + 7 * block[5].flag;
				block[6].xPos = block[6].xPos + 15 * block[6].flag;
			}
			else if (stage_4)
			{
				if (jagal[0].xPos > BLOCK_SIZE * 34) jagal[0].flag = -1;
				else if (jagal[0].xPos < BLOCK_SIZE *16) jagal[0].flag = 1;

				if (jagal[1].xPos > BLOCK_SIZE *37) jagal[1].flag = -1;
				else if (jagal[1].xPos < BLOCK_SIZE * 20) jagal[1].flag = 1;

				if (ring[3].yPos > BLOCK_SIZE * 41) ring[3].flag = -1;
				else if (ring[3].yPos < BLOCK_SIZE * 32) ring[3].flag = 1;

				if (block[7].xPos > BLOCK_SIZE * 22) block[7].flag = -1;
				else if (block[7].xPos < BLOCK_SIZE * 12) block[7].flag = 1;

				if (block[8].xPos > BLOCK_SIZE * 52) block[8].flag = -1;
				else if (block[8].xPos < BLOCK_SIZE * 36) block[8].flag = 1;

				jagal[0].xPos = jagal[0].xPos + 7 * jagal[0].flag;
				jagal[1].xPos = jagal[1].xPos + 7 * jagal[1].flag;
				ring[3].yPos = ring[3].yPos + 15 * ring[3].flag;
				block[7].xPos = block[7].xPos + 15 * block[7].flag;
				block[8].xPos = block[8].xPos + 30 * block[8].flag;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);

		break;

	case WM_LBUTTONUP:
		Drag = FALSE;
		break;

	case WM_MOUSEMOVE:
		if (Drag)
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			mapState[mx / BLOCK_SIZE][my / BLOCK_SIZE] = makeBlockID;
			InvalidateRgn(hWnd, NULL, TRUE);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void SaveMap(void)
{
	FILE* fstream;
	fstream = fopen("mystage.txt", "w");

	if (fstream)
	{
		for (int j = 0; j < HEIGHT_NUM; ++j)
		{
			for (int i = 0; i < WIDTH_NUM; ++i)
			{
				fwrite(&mapState[i][j], 1, 1, fstream);
			}
		}
	}
	fclose(fstream);
}

void stage1()
{
	FILE* fstream;
	fstream = fopen("stage1.txt", "r");

	int i = 0;
	int j = 0;
	while (i != 47 || j != 63)
	{
		fscanf(fstream, "%c", &mapState[j][i]);
		if (j < WIDTH_NUM - 1)
			++j;
		else
		{
			++i;
			j = 0;
		}
	}
	fclose(fstream);
}

void stage2()
{
	FILE* fstream;
	fstream = fopen("stage2.txt", "r");

	int i = 0;
	int j = 0;
	while (i != 47 || j != 63)
	{
		fscanf(fstream, "%c", &mapState[j][i]);
		if (j < WIDTH_NUM - 1)
			++j;
		else
		{
			++i;
			j = 0;
		}
	}
	fclose(fstream);
}

void stage3()
{
	FILE* fstream;
	fstream = fopen("stage3.txt", "r");

	int i = 0;
	int j = 0;
	while (i != 47 || j != 63)
	{
		fscanf(fstream, "%c", &mapState[j][i]);
		if (j < WIDTH_NUM - 1)
			++j;
		else
		{
			++i;
			j = 0;
		}
	}
	fclose(fstream);
}

void stage4()
{
	FILE* fstream;
	fstream = fopen("stage4.txt", "r");

	int i = 0;
	int j = 0;
	while (i != 47 || j != 63)
	{
		fscanf(fstream, "%c", &mapState[j][i]);
		if (j < WIDTH_NUM - 1)
			++j;
		else
		{
			++i;
			j = 0;
		}
	}
	fclose(fstream);
}

void mystage()
{
	FILE* fstream;
	fstream = fopen("mystage.txt", "r");

	int i = 0;
	int j = 0;
	while (i != 47 || j != 63)
	{
		fscanf(fstream, "%c", &mapState[j][i]);
		if (j < WIDTH_NUM - 1)
			++j;
		else
		{
			++i;
			j = 0;
		}
	}

	fclose(fstream);
}

void newstage(void)
{
	FILE* fstream;
	fstream = fopen("mystage.txt", "w");

	char temp[WIDTH_NUM][HEIGHT_NUM];
	for (int j = 0; j < HEIGHT_NUM; ++j)
	{
		for (int i = 0; i < WIDTH_NUM; ++i)
		{
			temp[i][j] = '0';
		}
	}
	if (fstream)
	{
		for (int j = 0; j < HEIGHT_NUM; ++j)
		{
			for (int i = 0; i < WIDTH_NUM; ++i)
			{
				fwrite(&temp[i][j], 1, 1, fstream);
			}
		}
	}
	fclose(fstream);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg,WPARAM wParam, LPARAM lParam) // 대화상자 메시지 처리함수
{
	switch (iMsg){
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK: // 버튼
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}

