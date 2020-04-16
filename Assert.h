#pragma once
#pragma warning(disable: 4996)

#include <stdbool.h>
#include <Windows.h>
#include <stdlib.h>

#define Assert(condition, message) if(!condition)MyAssert(__LINE__,__FILE__,(message));;
#define FatalError(condition, message, return_value) if(!condition)MyFatalError(__LINE__,__FILE__,(message),(return_value));;

void SystemOpen(int max_num_assert);