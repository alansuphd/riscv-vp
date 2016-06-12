#include <fake_master.h>

fake_master::fake_master(sc_module_name name): ahb_master_if(), sc_module(name)
{
    SC_THREAD(run);
        sensitive << clk.pos();
        dont_initialize();
}

fake_master::~fake_master()
{
}

void fake_master::run()
{    
    uint32_t data = 0;
    uint32_t addr = 0;
    uint32_t length = 4;

        addr = 0x40000000;//ctrl_reg[0] for SOURCE  : 0x4000_0000
        data = 1;
        bus_write(data, 0x40000000, 4);
        wait(200, SC_NS);

	addr = 0x40000004;//ctrl_reg[1] for TARGET  : 0x4000_0004 
        data = 5;
        bus_write(data, 0x40000004, 4);
        wait(200, SC_NS);


 	addr = 0x40000008;//ctrl_reg[2] for SIZE    : 0x4000_0008 
        data = 3;
        bus_write(data, 0x40000008, 4);
        wait(200, SC_NS);
	if(intr.read()==0)printf("!!!no~!!!!\n ");
	addr = 0x4000000c;//ctrl_reg[3] for START   : 0x4000_000c 
        data = 1;
        bus_write(data, 0x4000000c, 4);
        wait(200, SC_NS);
	if(intr.read()==0)printf("!!!no~!!!!\n ");
	addr = 0x4000000c;//ctrl_reg[3] for START   : 0x4000_000c 
        data = 1;
        bus_write(data, 0x0000000c, 4);
        wait(200, SC_NS);		
	printf("!!!no~!!!!\n ");
	while(1){
		if(intr.read()==1){
		addr = 0x40000010;//ctrl_reg[4] for clr   : 0x4000_0010
		data = 1;
		printf("!!!interr!!!!\n ");
		bus_write(data, addr, 4);
	wait(200, SC_NS);
			break;	
		}
printf("!!!no~!!!!\n ");
 	wait(200, SC_NS);

	}
	 bus_write(data, 0x0000000c, 4);
        wait(200, SC_NS);
    
}

bool fake_master::bus_read(uint32_t* data, uint32_t addr, uint32_t length)
{
    bool success;
    success = bus_b_access(0, false, (sc_dt::uint64)addr, reinterpret_cast<unsigned char*>(data), length);

    if(success)
    {
        printf("bus read 0x%X:0x%X\n\n", addr, *data);
        return true;
    }
    else
    {
        printf("bus read 0x%X failed\n", addr);
        return false;
    }
}

bool fake_master::bus_write(uint32_t data, uint32_t addr, uint32_t length)
{
    bool success;
    success = bus_b_access(0, true, (sc_dt::uint64)addr, reinterpret_cast<unsigned char*>(&data), length);

    if(success)
    {
        printf("bus write 0x%X:0x%X\n\n", addr, data);
        return true;
    }
    else
    {
        printf("bus write 0x%X:0x%X failed\n", addr, data);
        return false;
    }
}

