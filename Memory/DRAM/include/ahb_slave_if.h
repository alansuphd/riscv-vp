#ifndef _AHB_SLAVE_IF_H_
#define _AHB_SLAVE_IF_H_

#include <stdint.h>
using namespace std;

#include <systemc.h>

#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
using namespace tlm;
using namespace tlm_utils;

//#include <debug_utils.h>

class ahb_slave_if
{
    public:
        simple_target_socket<ahb_slave_if> ahb_slave_socket;

        ahb_slave_if(uint32_t mapping_size,const char* ahb_slave_name);
        ~ahb_slave_if()
        {
        }

        virtual void b_transport(tlm_generic_payload& trans, sc_time& delay);
        virtual bool local_access(bool write, uint32_t addr, uint32_t& data, unsigned int length, uint32_t burst_length) = 0;

        uint32_t get_address_mask();

    protected:

    private:
        uint32_t address_mask;
        inline bool is_power_of_two(uint32_t addr);
};
#endif
