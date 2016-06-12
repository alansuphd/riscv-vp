// DMA.h
#include "systemc.h"
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
using namespace tlm;
using namespace tlm_utils;

struct DMArm: sc_module
{	

	simple_initiator_socket<DMArm> DMA_master_socket;
	simple_target_socket<DMArm> DMA_slave_socket;


	sc_signal<sc_uint<32> > m_rdata;
	sc_signal<sc_uint<32> > s_rdata;
	sc_signal<bool> m_rw_flag;
  	sc_in<bool> clk,rst;
  	sc_out<bool> intr;		
	sc_signal<sc_uint<32> >m_addr,m_wdata;

	sc_signal<bool> s_rw_flag;
  	sc_signal<sc_uint<32> >  s_addr ;
        sc_signal<sc_uint<32> >  s_data ;
  	//unsigned int s_length;
      //  int s_burst_length ;


	uint32_t tmp_m_rdata;

        sc_time delay;

	sc_signal<sc_uint<32> >data_reg;
	sc_signal<sc_uint<32> > ctrl_reg[5];	 // 0 for SOURCE , 1 for TARGET , 2 for SIZE , 3 for START
	//sc_signal<sc_uint<32> >ctrl_reg1[5];
	sc_signal<sc_uint<2> >state_reg;

	void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay );
	void DMA();//process
	//Constructor
	SC_CTOR(DMArm):DMA_master_socket("DMA_master_socket"),DMA_slave_socket("DMA_slave_socket"){
	DMA_slave_socket.register_b_transport(this, &DMArm::b_transport);
   	SC_CTHREAD(DMA,clk.pos());
    	reset_signal_is(rst,false);
	}

};
