#include "Assert.h"

int main()
{
	int max_num_assert = 5;

	SystemOpen(max_num_assert);

	int a = 1;

	for (int i = 0; i < 3; i++)
		Assert(a == 0, "fail");

	for (int i = 0; i < 3; i++)
		Assert(a == 0, "fail");

	FatalError(a == 0, "error", 2);
}