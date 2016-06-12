#ifndef _DRAM_H_
#define _DRAM_H_

#include <stdint.h>
#include <systemc.h>
#include <ahb_slave_if.h>
#include <math.h>

#define READ_PIPELINE
#define REF_TIME    6400000 //#cycle @ 100MHz (10ns)
#define REF_CYCLE   8192
#define CL          10	//ns
#define CWL         10	//ns
#define tRAS        12000   // Active to Precharge 
#define tRP         10       // Precharge to Active ns
#define DRAM_CLK	6		// NS

enum ST_SDRAM {IDLE,        // 0
               RowActive,   // 1
               READ,        // 2
               WRITE,       // 3
               PRECHARGE    // 4
               };
enum BURST_STATE{
    FIRST_BURST,
    BURST,
    NO_BURST
};

/* module of RAM */
class DRAM: public ahb_slave_if, public sc_module
{
    public:
        SC_HAS_PROCESS(DRAM);
        DRAM(sc_module_name name, uint32_t mapping_size, const char* slave_num);
        ~DRAM();

        bool read(uint32_t*, uint32_t, int);
        bool write(uint32_t, uint32_t, int);
        virtual bool local_access(bool write, uint32_t addr, uint32_t& data, unsigned int length,uint32_t burst_length);

        //DRAM Model
        void InitDramController();
        bool NeedActive(unsigned int addr);
        bool NeedPrecharge(unsigned int addr, unsigned int tRAS_count);


    private:
        uint8_t* bank;

		uint32_t bank_bit;
		uint32_t row_bit;
		uint32_t col_bit;
		uint32_t dw_bit;

        inline uint32_t* ptr_word(uint32_t addr)
        {
            return (uint32_t*)(bank + addr);
        }

        inline uint16_t* ptr_hword(uint32_t addr)
        {
            return (uint16_t*)(bank + addr);
        }

        inline uint8_t* ptr_byte(uint32_t addr)
        {
            return (uint8_t*)(bank + addr);
        }

        //DRAM Model
        ST_SDRAM        state;
        unsigned int    AddrOffset_Bank;
        unsigned int    AddrOffset_Row;
        unsigned int    AddrOffset_Col;

        unsigned int    AddrMask_Bank;
        unsigned int    AddrMask_Row;
        unsigned int    AddrMask_Col;

        unsigned int    Index_Bank;
        unsigned int    Index_Row;
        unsigned int    Index_Col;

        unsigned int    prev_Bank;
        unsigned int    prev_Row;
                
        void InitAddressDecode();
        void AddrDecode(unsigned int addr);
        unsigned int GetMask(unsigned int bit);

		char dram_name[16];
        uint32_t precharge_counter;
        int b_length;
        BURST_STATE burst_state;
        int pre_burst;
        bool pre_w_burst;
        bool pre_r_burst;
};

#endif

