#include <ahb_master_if.h>

ahb_master_if::ahb_master_if(): ahb_master_socket("ahb_master_socket")
{
	//std::cout << "!!!!!HINT!!!!!" << endl;	
}

bool ahb_master_if::bus_b_access(int master_id,bool write, sc_dt::uint64 addr, unsigned char* data, unsigned int length, sc_time delay)
{
	int burst_len;
	if(length <= 4)
	{
		burst_len = 1;
	}
	else if(length%4 != 0)
	{
		std::cout << "Just support 32-bits burst" << endl;
	}
	else
	{
		burst_len = length/4;
	}
	for(int i=0;i<burst_len;i++)
	{
		uint32_t tmp_data;
		tmp_data = ((uint32_t*)data)[i];
    	tlm_generic_payload payload;
    	payload.set_command(write ? TLM_WRITE_COMMAND : TLM_READ_COMMAND);
    	payload.set_address(addr+i*4);
    	payload.set_data_ptr( ((uint8_t*)(&tmp_data)) );
		if(length > 4)
	    	payload.set_data_length(4);
		else
	    	payload.set_data_length(length);
    	payload.set_streaming_width(burst_len); //= data_length, means no streaming
    	payload.set_byte_enable_ptr(0);
    	payload.set_dmi_allowed(false);
    	payload.set_response_status(TLM_INCOMPLETE_RESPONSE);
	
    	if(delay.to_seconds() > 0.0)
    	{
        	wait(delay);
    	}
    	ahb_master_socket->b_transport(payload, delay);

    	if(payload.is_response_error())
    	{
        	printf(" ahb_transaction returned with error: %s", payload.get_response_string().c_str());
        	return false;
    	}
		((uint32_t*)data)[i] = tmp_data;
	}

    return true;
}

