#include <systemc.h>
#include <ahb_master_if.h>

using namespace std;

class fake_master: public sc_module, public ahb_master_if
{
    public:
        sc_in_clk clk;
	sc_in<bool> intr;
        SC_HAS_PROCESS(fake_master);
        fake_master(sc_module_name name);
        ~fake_master();

    protected:
        void run();
        bool bus_read(uint32_t* data, uint32_t addr, uint32_t length);
        bool bus_write(uint32_t data, uint32_t addr, uint32_t length);
};
