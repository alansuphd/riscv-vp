#include "dram.h"
#include "ahb_slave_if.h"
#include "ahb_master_if.h"
#include "fake_master.h"

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

    auto_ptr<fake_master> f_master;
    auto_ptr<DRAM> my_dram;

    SC_CTOR(top){
        f_master.reset(new fake_master("f_master"));
        f_master->clk(clk);
        my_dram.reset(new DRAM("DRAM", ram_size, "DRAM_test"));
        f_master->ahb_master_socket.bind(my_dram->ahb_slave_socket);
    }
};
