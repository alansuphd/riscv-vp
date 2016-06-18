#ifndef MEMORY_H
#define MEMORY_H

struct Memory: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<Memory> socket;

  enum {SIZE = 1024};

  uint32_t mapping_size, data;
  bool write, success;
  uint32_t address_mask;

  SC_CTOR(Memory)
  : socket("socket")
  {
    // Register callback for incoming b_transport interface method call
    
    if(is_power_of_two(mapping_size))
    {    
	socket.register_b_transport(this, &Memory::b_transport);	
	this -> address_mask = mapping_size -1;
    }
    else
    {
	std::cout<<"illegal mapping size"<<endl;
    }
  }
  
  // TLM-2 blocking transport method  
  virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
  {
    tlm::tlm_command cmd = trans.get_command();
    uint32_t   addr = trans.get_address();
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();
    int     burst_len = trans.get_streaming_width();

    // Obliged to check address range and check for unsupported features,
    //   i.e. byte enables, streaming, and bursts
    // Can ignore DMI hint and extensions
    // Using the SystemC report handler is an acceptable way of signalling an error
   
    if (addr >= uint32_t(SIZE) || len > 4 || burst_len < len)
      SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");

    if ( write == tlm::TLM_READ_COMMAND )
      memcpy(ptr, &mem[addr], len);
    else if ( write == tlm::TLM_WRITE_COMMAND )
      memcpy(&mem[addr], ptr, len);

    // Obliged to set response status to indicate successful completion
    trans.set_response_status( tlm::TLM_OK_RESPONSE );
    }
 uint32_t get_address_mask()
 {
	return address_mask;
 }
 inline bool is_power_of_two(uint32_t x)
 {
	return((x!=0) && ((x&(~x+1))== x));
 }
 int mem[SIZE];
};
#endif
