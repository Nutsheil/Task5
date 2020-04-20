#pragma once
#pragma warning(disable: 4996)

#include <Windows.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef _DEBUG
#define Assert(condition, message) if(!condition)MyAssert(__LINE__,__FILE__,(message));;
#else
#define Assert(condition, message);
#endif

#define FatalError(condition, message, return_value) if(!condition)MyFatalError(__LINE__,__FILE__,(message),(return_value));;

typedef enum
{
	my_assert, my_fatal_error
}assert_or_fatal;

typedef enum 
{
	ignore_this_assert, ignore_all_asserts
}ASSERT_action;

void SystemOpen(int max_num_assert);

void SystemClose();

void CreateWinAPI(assert_or_fatal a_or_f, int line, char* file, char* message);

void Button_Handl(ASSERT_action act, int line, char* file);
