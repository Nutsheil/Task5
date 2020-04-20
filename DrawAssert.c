#include "Assert.h"

///////////////////////////////////// глобальные переменные
char szClassName[] = "Window1";
HWND hWnd;
///////////////////////////////////// прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance);

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* window_name);
//////////////////////////////////////

static int ASSERT_line;
static char* ASSERT_file;
char* ASSERT_message;

void CreateWinAPI(assert_or_fatal a_or_f, int line, char* file, char* message)
{
	ASSERT_line = line;
	ASSERT_file = file;
	ASSERT_message = message;

	switch (a_or_f)
	{
	case my_assert:
		createMyWindow(NULL, 1, "ASSERT");
		HWND assert_window_button1 = CreateWindow("button", "ignore", WS_CHILD | WS_VISIBLE, 100, 200, 70, 20, hWnd, (HMENU)1, NULL, NULL);
		HWND assert_window_button2 = CreateWindow("button", "ignore all", WS_CHILD | WS_VISIBLE, 200, 200, 70, 20, hWnd, (HMENU)2, NULL, NULL);
		HWND assert_window_button3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 300, 200, 70, 20, hWnd, (HMENU)3, NULL, NULL);
		break;
	case my_fatal_error:
		createMyWindow(NULL, 1, "FATAL ERROR");
		HWND fatal_window_button3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 200, 200, 70, 20, hWnd, (HMENU)3, NULL, NULL);
		break;
	default:
		break;
	}

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Draw_Assert(HDC hdc)
{
	TextOutA(hdc, 20, 20, "Message:", 8);
	TextOutA(hdc, 20, 60, "File:", 5);
	TextOutA(hdc, 20, 100, "Line:", 5);

	TextOutA(hdc, 90, 20, ASSERT_message, strlen(ASSERT_message));
	TextOutA(hdc, 55, 60, ASSERT_file, strlen(ASSERT_file));

	char* line = (char*)malloc(10);
	itoa(ASSERT_line, line, 10);
	TextOutA(hdc, 60, 100, line, strlen(line));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1:
			Button_Handl(ignore_this_assert, ASSERT_line, ASSERT_file);
			DestroyWindow(hWnd);
			break;
		case 2:
			Button_Handl(ignore_all_asserts, ASSERT_line, ASSERT_file);
			DestroyWindow(hWnd);
			break;
		case 3:
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	}
	case WM_KEYDOWN:
	{
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		Draw_Assert(hdc);

		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* window_name)
{
	registerMyClass(hInstance);

	hWnd = CreateWindow(szClassName, window_name, WS_OVERLAPPEDWINDOW, 200, 200, 600, 400, NULL, NULL, hInstance, NULL);

	if (!hWnd) { return 0; }
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

ATOM registerMyClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	return RegisterClass(&wc);
}