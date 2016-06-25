#include "AHB_test.h"
#include "AHB_s_test.h"

int sc_main(int argc, char* argv[])
{
	sc_clock clk("clk", 10, SC_NS, 0.5, 0, true);

	AHB top("top");
	AHB_S test("test");
	test.clk(clk);
	sc_start();
	return 0;
}
