#include "dram.h"
#include "ahb_slave_if.h"
#include "ahb_master_if.h"
#include "fake_master.h"
#include "DMA.h"
#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include <tlm.h>
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
using namespace tlm;
using namespace tlm_utils;

SC_MODULE(top)
{
    sc_in_clk clk;

    uint32_t ram_size = 32<<10;
	sc_in<bool> rst;
	sc_signal<bool> intr;
    auto_ptr<fake_master> f_master;
    auto_ptr<DRAM> dramA;
   // auto_ptr<DRAM> dramB;
	DMArm    *DMA1;

    SC_CTOR(top){
        f_master.reset(new fake_master("f_master"));
       
	DMA1    = new DMArm   ("DMA1");
        dramA.reset(new DRAM("dramA", ram_size, "dram1"));
	//dramB.reset(new DRAM("dramB", ram_size, "dram2"));
  	f_master->clk(clk);
	DMA1->clk(clk);
	DMA1->rst(rst);
 	f_master->intr(intr);
	DMA1->intr(intr);
        f_master->ahb_master_socket.bind(DMA1->DMA_slave_socket);	 	
        DMA1->DMA_master_socket.bind(dramA->ahb_slave_socket);
	
	



    }
};
