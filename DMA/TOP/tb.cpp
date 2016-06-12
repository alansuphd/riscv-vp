#include <top.h>

#include "systemc.h"

int sc_main(int argc, char* argv[]){
	sc_time clkprd(10, SC_NS);
	sc_time clkdly(0, SC_NS);
	sc_clock clk("clk", clkprd, 0.5, clkdly, true);
	sc_signal<bool> rst;
//    sc_signal<bool> reset;

    top *t;
    t = new top("top");
    t->clk(clk);
    t->rst(rst);
    rst.write(1);
    sc_start(10000, SC_NS);
    rst.write(0);
	
	return(0);
}
