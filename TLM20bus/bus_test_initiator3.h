#ifndef INITIATORB_H
#define INITIATORB_H
using namespace tlm;
using namespace tlm_utils;

struct InitiatorB: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_initiator_socket<InitiatorB> socket;

  SC_CTOR(InitiatorB)
  : socket("socket")  // Construct and name socket
  {
    SC_THREAD(thread_process);
  }

  void thread_process()
  {
    int master_id;
    int burst;
    int burst_len;
    bool write;
    sc_dt::uint64 addr;
    unsigned char* data;
    unsigned int length;

    // TLM-2 generic payload transaction, reused across calls to b_transport
    tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
    sc_time delay = sc_time(10, SC_NS);

    // Generate a random sequence of reads and writes
    for (int i=0; i<burst; i++)
    {
      uint32_t tmp_data;
    
      tmp_data = ((uint32_t*)data)[i];

      // Initialize 8 out of the 10 attributes, byte_enable_length and extensions being unused
      trans->set_command( write ? TLM_WRITE_COMMAND : TLM_READ_COMMAND );
      trans->set_address( addr+i*4 );
      trans->set_data_ptr( ((uint8_t*)(&tmp_data)) );
      if(length>4)
      	trans->set_data_length( 4 );
      else
        trans->set_data_length( length );
      trans->set_streaming_width( burst_len ); // = data_length to indicate no streaming
      trans->set_byte_enable_ptr( 0 ); // 0 indicates unused
      trans->set_dmi_allowed( false ); // Mandatory initial value
      trans->set_response_status( tlm::TLM_INCOMPLETE_RESPONSE ); // Mandatory initial value

      socket->b_transport( *trans, delay );  // Blocking transport call

      // Initiator obliged to check response status and delay
      if ( trans->is_response_error() )
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

      cout << "trans = { " << (write ? 'W' : 'R') << ", " << hex << i
           << " } , data = " << hex << data << " at time " << sc_time_stamp()
           << " delay = " << delay << endl;

      // Realize the delay annotated onto the transport call
      wait(delay);
      ((uint32_t*)data)[i] = tmp_data;
    }
  }
  // Internal data buffer used by initiator with generic payload
};

#endif
