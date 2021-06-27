#include "HostClass.hpp"

int main(void)
{
	HostClass test("default.conf");

	test.parseConfig();
	return 0;
}
