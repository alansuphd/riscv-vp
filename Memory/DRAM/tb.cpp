#include <top.h>

#include "systemc.h"

int sc_main(int argc, char* argv[]){
	sc_time clkprd(10, SC_NS);
	sc_time clkdly(0, SC_NS);
	sc_clock clk("clk", clkprd, 0.5, clkdly, true);

//    sc_signal<bool> reset;

    top *t;
    t = new top("top");
    t->clk(clk);

    sc_start(5000, SC_NS);

	return(0);
}
