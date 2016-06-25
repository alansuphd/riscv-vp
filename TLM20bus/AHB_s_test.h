#ifndef AHB_S_H
#define AHB_S_H

struct AHB_S: sc_module
{	
	bool RESET;
	bool Lock, MastLock, LockedData, HoldLock;
	bool READY;
	bool clk;

	void Se(){
		
		sc_time delay = sc_time(10, SC_NS);

		if(RESET==1)
		{
			MastLock = 0;
			LockedData = 0;		
		}
		else if(READY==1)
		{
			MastLock = Lock;
			LockedData = MastLock;
		}
	}

	 SC_CTOR(AHB_S)
  	{	
		SC_THREAD(Se);
  	  	sensitive << clk.pos();
  	}
};
#endif
