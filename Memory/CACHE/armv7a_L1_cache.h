#ifndef _ARMV7A_L1_CACHE_H_
#define _ARMV7A_L1_CACHE_H_

//#include "systemc.h"
// #include "ahb_master_if.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include <cpu2cache_if.h>

/* L1 cache setting */
#define STANDALONE 

#define L1_CACHE_SIZE			0x8000		// 32KB
#define L1_CACHE_LINE_SIZE		16			// 16 words per line
#define L1_CACHE_WAY			2
#define L1_SET					L1_CACHE_SIZE/(L1_CACHE_WAY * L1_CACHE_LINE_SIZE * 4)
#define L1_WRITEBACK 			true // hardcode: Write is't implement!1
#define L1_delay				1

typedef struct cache_entry{
	bool	valid;
	bool	clean[L1_CACHE_LINE_SIZE];
	uint32_t tag;
	union{
		uint32_t word[L1_CACHE_LINE_SIZE];
		uint16_t half[L1_CACHE_LINE_SIZE*2];
		uint8_t byte[L1_CACHE_LINE_SIZE*8];
	}line;
}cache_entry;

class armv7a_L1_cache 
{
	public:		
		//sc_port<cache_serve_if> cache_serve;
		//sc_in_clk clk;
		
		char cache_name[30];
		
		//SC_HAS_PROCESS(armv7a_L1_cache);
		armv7a_L1_cache( unsigned int core_id, bool is_i_cache);
		~armv7a_L1_cache();

	
	public:
		uint32_t cid;
		bool is_i;
		/* cache and replace policy */
		cache_entry **cache;
		uint32_t rp_flag[L1_SET]; /* for replacement policy, we use round robin */
		
		/* mask */
		uint32_t tag_mask;
		uint32_t index_mask;
		uint32_t offset_mask;
		
		/* compare and replace */
		uint64_t cache_access;
		uint64_t cache_miss;
		uint32_t set_index;
		uint32_t hit_way;
		
		/* Receive from channel */
		uint32_t cmd;
		uint32_t addr;
		uint32_t data;
		uint32_t size;
	
	/* Profiling */
		uint64_t work_item_stack_miss;
		uint64_t work_item_stack_access;
		uint64_t global_mem_miss;
		uint64_t global_mem_access;
		uint64_t local_mem_miss;
		uint64_t local_mem_access;
		uint64_t func_mem_miss;
		uint64_t func_mem_access;
		uint64_t page_mem_miss;
		uint64_t page_mem_access;
		uint64_t other_mem_miss;
		uint64_t other_mem_access;
		uint64_t miss_penalty;
		uint64_t eviction;
		
	public:
		uint32_t run(int cmd,int data,int addr,int size);
		inline bool compare_tag(uint32_t addr);
		inline void handle_miss(uint32_t addr);
		inline void replace_cacheline(uint32_t addr);
		inline void invalid_cache_line();


};
void bus_b_access(int master_id,bool write, uint32_t addr, unsigned char* data, unsigned int length);
#endif
