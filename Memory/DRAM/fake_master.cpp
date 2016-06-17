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

        bus_write(data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        data += 100;
        bus_write(data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        data += 100;
        bus_write(data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        data += 100;
        bus_write(data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        data += 100;
        bus_write(data, addr, 4);
        wait(200, SC_NS);

        addr = 0;
        bus_read(&data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        bus_read(&data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        bus_read(&data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        bus_read(&data, addr, 4);
        wait(200, SC_NS);

        addr += 4;
        bus_read(&data, addr, 4);
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

