#ifndef _AHB_MASTER_IF_H_
#define _AHB_MASTER_IF_H_

#include <stdint.h>
using namespace std;

#include <systemc.h>

#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
using namespace tlm;
using namespace tlm_utils;

//Initiator module generating generic payload transactions

class ahb_master_if
{
    public:
        //TLM-2 socket, defaults to 32-bits wide, base protocol
        simple_initiator_socket<ahb_master_if> ahb_master_socket;
        ahb_master_if();
        ~ahb_master_if()
        {
        }

    protected:
        bool bus_b_access(int master_id,bool write, sc_dt::uint64 addr, unsigned char* data, unsigned int length, sc_time delay = SC_ZERO_TIME);

    private:
        //tlm_payload_pool payload_pool;
};

#endif
