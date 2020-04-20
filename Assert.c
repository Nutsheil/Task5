#include "Assert.h"

typedef struct
{
	char* file;
	int line;
}assert_to_ignore;

assert_to_ignore* ASSERT_ignore;
int ASSERT_ignore_amount;

bool ASSERT_ignore_all;

void SystemClose()
{
	free(ASSERT_ignore);
}

void SystemOpen(int max_num_assert)
{
	ASSERT_ignore_amount = 0;
	ASSERT_ignore_all = false;
	ASSERT_ignore = (assert_to_ignore*)malloc(max_num_assert * sizeof(assert_to_ignore));
}

void MyAssert(int line, char* file, char* message)
{
	if (ASSERT_ignore_all == true)
		return;

	for (int i = 0; i < ASSERT_ignore_amount; i++)
		if (strcmp(ASSERT_ignore[i].file, file) == 0 && ASSERT_ignore[i].line == line)
			return;

	CreateWinAPI(my_assert, line, file, message);
}

void MyFatalError(int line, char* file, char* message, int return_value)
{
	CreateWinAPI(my_fatal_error, line, file, message);

	free(ASSERT_ignore);
	exit(return_value);
}

void Button_Handl(ASSERT_action act, int line, char* file)
{
	switch (act)
	{
	case ignore_this_assert:
		ASSERT_ignore[ASSERT_ignore_amount].line = line;
		ASSERT_ignore[ASSERT_ignore_amount].file = file;
		ASSERT_ignore_amount++;
		break;
	case ignore_all_asserts:
		ASSERT_ignore_all = true;
		break;
	default:
		break;
	}
}