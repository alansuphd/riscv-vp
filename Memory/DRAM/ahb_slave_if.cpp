#include <ahb_slave_if.h>

ahb_slave_if::ahb_slave_if(uint32_t mapping_size,const char* ahb_slave_name): ahb_slave_socket(ahb_slave_name)
{
    //std::cout << "!!!!!AHB_SLAVE_IF CONSTRUCTOR!!!!!" << endl;
    if(is_power_of_two(mapping_size))
    {
        ahb_slave_socket.register_b_transport(this, &ahb_slave_if::b_transport);
        this->address_mask = mapping_size - 1;
    }
    else
    {
        std::cout << "illegal mapping size" << endl;
    }
}

void ahb_slave_if::b_transport(tlm_generic_payload& payload, sc_time& delay)
{
    tlm_command cmd = payload.get_command();
    uint32_t addr = (uint32_t)payload.get_address();
    uint32_t* data_ptr = (uint32_t*)payload.get_data_ptr();
    unsigned int length = payload.get_data_length();
    int burst_length = payload.get_streaming_width();

    /*
       if(addr & get_address_mask() >= 0)
       {
        payload.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
       }
       else*/

    if(length > 4)
    {
        std::cout << "length error" << endl;
        payload.set_response_status(TLM_BURST_ERROR_RESPONSE);
    }

    else
    {
        bool success;
        uint32_t data;

        if(cmd == TLM_READ_COMMAND)
        {
            success = local_access(false, addr, data, length, burst_length);
            printf("read from 0x%X: 0x%X\n", addr, data);

            if(success)
            {
                *data_ptr = data;
                payload.set_response_status(TLM_OK_RESPONSE);
            }

            else
            {
                std::cout << "read error" << endl;
                payload.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
            }

            //memcpy(ptr, &mem[adr], len);
        }

        else if(cmd == TLM_WRITE_COMMAND)
        {
            data = *data_ptr;
            success = local_access(true, addr, data, length, burst_length);
            printf("write to 0x%X: 0x%X\n", addr, data);

            if(success)
            {
                payload.set_response_status(TLM_OK_RESPONSE);
            }

            else
            {
                std::cout << "write error" << endl;
                payload.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
            }

            //memcpy(&mem[adr], ptr, len);
        }

        else
        {
            std::cout << "no such command" << endl;
            payload.set_response_status(TLM_COMMAND_ERROR_RESPONSE);
        }
    }
}
uint32_t ahb_slave_if::get_address_mask()
{
    return address_mask;
}

inline bool ahb_slave_if::is_power_of_two(uint32_t x)
{
    return((x != 0) && ((x & (~x + 1)) == x));
}

