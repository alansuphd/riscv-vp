// DMA.cpp
#include "DMA.h"
  void DMArm:: b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
  {
		 tlm_command cmd = trans.get_command();
  	     
                 
		//s_addr.write(addr);
		//s_rdata.write( * data_ptr);
if((cmd == TLM_WRITE_COMMAND)){
			if(trans.get_address()==0x40000000||trans.get_address()==0x40000004||trans.get_address()==0x40000008||trans.get_address()==0x4000000c||trans.get_address()==0x40000010)
		    s_rw_flag.write(1);
			else
  s_rw_flag.write(0);
}

else
 			s_rw_flag.write(0);
  		    s_addr.write(trans.get_address());

 			uint32_t* data_ptr = (uint32_t*)trans.get_data_ptr();
					//printf("%d\n",* data_ptr);
      		   s_data.write(* data_ptr);

	trans.set_response_status(TLM_OK_RESPONSE);
		
			
}
void DMArm::DMA() {

	data_reg.write(0);
	state_reg.write(0);
	for( unsigned int i = 0; i < 5; i++ )
		ctrl_reg[i].write(0);
    //reset here needed
	wait();

  while(1) {	

	if(s_rw_flag.read() == 0){
//printf("%d",(int)ctrl_reg[3].read());
	if(ctrl_reg[3].read() == 1)
		{	
		if(ctrl_reg[2].read() != 0)
			{
			if(state_reg.read() ==0)
				{
				m_rw_flag.write(0);
				m_addr.write(ctrl_reg[0].read());					
				state_reg.write(1);
				ctrl_reg[0].write(ctrl_reg[0].read()+1);
				}
			else if(state_reg.read() ==1)
				state_reg.write(2);
			else if(state_reg.read() ==2){
				data_reg.write(tmp_m_rdata);
				state_reg.write(3);	
			}			
		        else if(state_reg.read() ==3){
				m_rw_flag.write(1);
				m_addr.write(ctrl_reg[1].read());
				m_wdata.write(data_reg.read());
				state_reg.write(0);
				ctrl_reg[1].write(ctrl_reg[1].read()+1);
				ctrl_reg[2].write(ctrl_reg[2].read()-1);
			}	
		
		}
	        else{//printf("\nnn%d\n",(int)ctrl_reg[4].read());	
			if(ctrl_reg[4].read()!=0){
				intr.write(0);
				data_reg.write(0);
				state_reg.write(0);
				for( unsigned int i = 0; i < 5; i++ )
				ctrl_reg[i].write(0);
				printf("wwwww");
			}
			else{
			//ctrl_reg[3].write(0);
		        intr.write(1);
			printf("www");
			}
		

		}
	tlm_generic_payload payload;
	uint32_t tmp_data;
	tmp_data=m_wdata.read();
        payload.set_command(m_rw_flag.read() ? TLM_WRITE_COMMAND : TLM_READ_COMMAND);
    	payload.set_address(m_addr.read());
    	payload.set_data_ptr( ((uint8_t*)(&tmp_data)) );
	payload.set_data_length(4);
    	payload.set_streaming_width(1); //= data_length, means no streaming
    	payload.set_byte_enable_ptr(0);
    	payload.set_dmi_allowed(false);
    	payload.set_response_status(TLM_INCOMPLETE_RESPONSE);
	DMA_master_socket->b_transport(payload,delay);

	    	if(payload.is_response_error())
	    	{
			printf("transaction returned with error: %s", payload.get_response_string().c_str());
			return false;
	    	}
			//m_rdata.write(tmp_data);
		
			tmp_m_rdata=tmp_m_rdata;
	}
		}	
	else{
			
			if(s_addr.read() == 0x40000000){	//ctrl_reg[0] for SOURCE  : 0x4000_0000 
				//printf("\n %d\n", (int)s_data.read());
				ctrl_reg[0].write( s_data.read());
				}
			else if(s_addr.read() == 0x40000004)	//ctrl_reg[1] for TARGET  : 0x4000_0004 
				ctrl_reg[1].write( s_data.read());
			else if(s_addr.read() == 0x40000008)	//ctrl_reg[2] for SIZE    : 0x4000_0008 
				ctrl_reg[2].write(s_data.read());
			else if(s_addr.read()== 0x4000000c)	{
			//	printf("\n %d\n", (int)s_data.read());//ctrl_reg[3] for START   : 0x4000_000c 
				ctrl_reg[3].write(s_data.read());
			//	printf("%d\n",(int)ctrl_reg[3].read());
}
			else if(s_addr.read() == 0x40000010){	//ctrl_reg[4] for clr   : 0x4000_000c 
				ctrl_reg[4].write(s_data.read());		
  				

}
		}


		
    //	if(delay.to_seconds() > 0.0)
    //	{
    //    	wait(delay);
    //	}
    	

	wait();		
  }
}
