#include "Assert.h"

#ifdef _DEBUG
bool DEBUG_VER = true;
#else
bool DEBUG_VER = false;
#endif

///////////////////////////////////// глобальные переменные
char szClassName[] = "Window1";
HWND hWnd;
///////////////////////////////////// прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, UINT, LONG);

ATOM registerMyClass(HINSTANCE hInstance);

int createMyWindow(HINSTANCE hInstance, int nCmdShow, char* window_name);
//////////////////////////////////////

typedef struct
{
	char** file;
	int* line;
	int amount;
}assert_to_ignore;

assert_to_ignore ASSERT_ignore;

static int ASSERT_line;
static char* ASSERT_file;
static char* ASSERT_message;
static bool ASSERT_ignore_all;

void SystemClose()
{
	free(ASSERT_ignore.file);
	free(ASSERT_ignore.line);
}

void SystemOpen(int max_num_assert)
{
	ASSERT_ignore_all = false;
	ASSERT_ignore.amount = 0;
	ASSERT_ignore.file = (char**)malloc(max_num_assert * sizeof(char*));
	ASSERT_ignore.line = (int*)malloc(max_num_assert * sizeof(int));
}

void MyAssert(int line, char* file, char* message)
{
	if (ASSERT_ignore_all == true || DEBUG_VER == false)
		return;

	for (int i = 0; i < ASSERT_ignore.amount; i++)
		if (strcmp(ASSERT_ignore.file[i], file) == 0 && ASSERT_ignore.line[i] == line)
			return;

	ASSERT_line = line;
	ASSERT_file = file;
	ASSERT_message = message;

	createMyWindow(NULL, 1, "ASSERT");
	HWND window_button1 = CreateWindow("button", "ignore", WS_CHILD | WS_VISIBLE, 100, 200, 70, 20, hWnd, (HMENU)1, NULL, NULL);
	HWND window_button2 = CreateWindow("button", "ignore all", WS_CHILD | WS_VISIBLE, 200, 200, 70, 20, hWnd, (HMENU)2, NULL, NULL);
	HWND window_button3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 300, 200, 70, 20, hWnd, (HMENU)3, NULL, NULL);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}

void MyFatalError(int line, char* file, char* message, int return_value)
{
	ASSERT_line = line;
	ASSERT_file = file;
	ASSERT_message = message;

	createMyWindow(NULL, 1, "FATAL ERROR");
	HWND window_button3 = CreateWindow("button", "exit", WS_CHILD | WS_VISIBLE, 200, 200, 70, 20, hWnd, (HMENU)3, NULL, NULL);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	free(ASSERT_ignore.file);
	free(ASSERT_ignore.line);
	exit(return_value);
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
			ASSERT_ignore.line[ASSERT_ignore.amount] = ASSERT_line;
			ASSERT_ignore.file[ASSERT_ignore.amount] = ASSERT_file;
			ASSERT_ignore.amount++;
			DestroyWindow(hWnd);
			break;
		case 2:
			ASSERT_ignore_all = true;
			DestroyWindow(hWnd);
			break;
		case 3:
			DestroyWindow(hWnd);
			//exit(1);
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