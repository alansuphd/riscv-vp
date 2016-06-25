#define adreess_bound 1024

#define TRN_IDLE   0
#define TRN_BUSY   1
#define TRN_NONSEQ 2
#define TRN_SEQ    3

#define RSP_OKAY   0
#define RSP_ERROR  1
#define RSP_RETRY  2
#define RSP_SPLIT  3

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include "systemc"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "bus_test_initiator2.h"
#include "bus_test_initiator3.h"
#include "bus_test_target4.h"

struct AHB: sc_module
{
	bool Master1_REQ;
	bool Master2_REQ;
	uint8_t Master1_TRAN;
	uint8_t Master2_TRAN;
	uint32_t address;
	
	bool Master1_GRANT;
	bool Master2_GRANT;
	uint8_t RSP;
	uint8_t top_request;

	uint8_t pre_Master1_TRAN, pre_Master2_TRAN;
	bool pre_Master1_REQ, pre_Master2_REQ;
	
	bool Lock;
	
	bool HLock0, HLock1;
	
	bool READY;	
	
	InitiatorA *initiatorA_t;
	InitiatorB *initiatorB_t;
	Memory *memory;


  SC_CTOR(AHB)
  {
    if(top_request==1)
	{
		initiatorA_t = new InitiatorA("initiator_t");
		memory = new Memory("memory");
		initiatorA_t->socket.bind(memory->socket);
	}		
	else if(top_request==2)
	{
		initiatorB_t = new InitiatorB("initiator_t");
		memory = new Memory("memory");
		initiatorB_t->socket.bind(memory->socket);
	}
  }
  
  AHB()
  {
	if(address>adreess_bound)
		RSP=RSP_ERROR;
	else
		RSP=RSP_OKAY;

	if(Master1_REQ==1 && !(pre_Master1_REQ==1 && pre_Master1_TRAN==TRN_BUSY))             
	{
		Master1_GRANT=1;
		Master2_GRANT=0;
	}
	else if(Master2_REQ==1)                                                       
	{
		Master1_GRANT=0;
		Master2_GRANT=1;
	}	
	else
	{
		Master1_GRANT=0;
		Master2_GRANT=0;
	}
	
	
	if(Master1_GRANT==1 && Master1_TRAN==TRN_IDLE)           
	{
		top_request=1;
	}
	else if(Master2_GRANT==1 && Master2_TRAN==TRN_IDLE)
	{
		top_request=2;
	}
	
	switch(Master1_GRANT)
	{
		HLock0=0;
		HLock1=0;
		
		case 0: 
			Lock = HLock0;
			break;
		case 1:
			Lock = HLock1;
			break;
		default:
			Lock = 0;
	}	

	switch(Master2_GRANT)
	{
		HLock0=0;
		HLock1=0;
		
		case 0: 
			Lock = HLock0;
			break;
		case 1:
			Lock = HLock1;
			break;
		default:
			Lock = 0;
	}	
	
	pre_Master1_TRAN=Master1_TRAN;                        
	pre_Master2_TRAN=Master2_TRAN;
	pre_Master1_REQ=Master1_REQ;
	pre_Master2_REQ=Master2_REQ;
  }
};
