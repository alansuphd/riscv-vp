
#include "armv7a_L1_cache.h"

armv7a_L1_cache::armv7a_L1_cache( unsigned int core_id, bool is_i_cache)
{
	int i,j,k;
	//strcpy(cache_name, name);
	cid = core_id;
	is_i = is_i_cache;
	cache = (cache_entry**)malloc(sizeof(cache_entry*)*L1_CACHE_WAY);
	for(i=0;i<L1_CACHE_WAY;i++)
		cache[i] = (cache_entry*)malloc(sizeof(cache_entry)*L1_SET);
	
	for(i = 0; i < L1_CACHE_WAY; i++){
		for(j = 0; j < L1_SET; j++){
			cache[i][j].valid 	= false;
			cache[i][j].tag		= 0;
			for(k=0;k<L1_CACHE_LINE_SIZE;k++)
			{
				cache[i][j].clean[k]	= true;
				cache[i][j].line.word[k] = 0;
			}
		}
	}
	for(j = 0; j < L1_SET; j++)
		rp_flag[j] = 0;

	cache_access = 0;
	cache_miss = 0;
	miss_penalty = 0;
	eviction = 0;
		
	/* Set the mask for tag comparing */
	offset_mask = (uint32_t)(L1_CACHE_LINE_SIZE*4 - 1);
	index_mask 	= (uint32_t)((L1_SET*L1_CACHE_LINE_SIZE*4-1) & (~offset_mask));
	tag_mask	= (uint32_t)(0xFFFFFFFF & (~(index_mask | offset_mask)));
}

armv7a_L1_cache::~armv7a_L1_cache(){
	/* Deconstructor */
	free(cache);
}

uint32_t armv7a_L1_cache::run(int cmd,int data,int addr,int size){
	uint32_t tmp_addr;
	uint32_t tmp_data = 0;
	uint32_t tlm_data;
	
	bool is_hit;


		if(cmd == 0)/* read */
		{
			cache_access++;
			tmp_addr = addr;
			is_hit = compare_tag(tmp_addr);
			/* After doing compare_tag, we know cache miss is occur or not.
			   If hit, hit_way show the way we want to use. */
			if(!is_hit){
				cache_miss++;
				handle_miss(tmp_addr);
				/* After handle_miss, hit_way is the way we want to use */
			}
			
			/* Read a word and do mask for correct value */
			set_index = (tmp_addr&index_mask)>>((uint32_t)(log2(offset_mask+1)));
			switch(size)
			{
				case 1:
					tmp_data =  ((cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)]));
					break;
				case 2:
					tmp_data =  ((cache[hit_way][set_index].line.half[(tmp_addr&offset_mask)>>1]));
					break;
				case 4:
					tmp_data =  ((cache[hit_way][set_index].line.word[(tmp_addr&offset_mask)>>2]));
					break;
				default:;
			}

		}

		if(addr == 0x76543210 && cmd == 1 && data == 1){
			printf("[Magic spell!]\n");
			for(int way=0;way < L1_CACHE_WAY;way++)
				for(int index=0;index<L1_SET;index++)
					for(int i=0; i < L1_CACHE_LINE_SIZE;i++)
						if(cache[way][index].clean[i] == false){
							/* Write dirty data back ba ck to memory */
							uint32_t addr = cache[way][index].tag | (index<<6) | (i<<2);
							printf("bus access: write data=%d to addr=%d\t",
								cache[way][index].line.word[i],
								addr); // [FIXME]: Connect with bus 
							cache[way][index].clean[i] = true;
							printf("CLean up [%x][%x][%x][%x]\n", way, index, i,addr);
						}
					
		}
		else if(cmd == 1)	/* write */
		{
			
			cache_access++;
			tmp_addr = addr;
			tmp_data = data;
			
			is_hit = compare_tag(tmp_addr);
			if(!is_hit){
				cache_miss++;
				handle_miss(tmp_addr);
				/* After handle_miss, hit_way is the way we want to use */
			}
			set_index = (tmp_addr&index_mask)>>((uint32_t)(log2(offset_mask+1)));
			switch(size)
			{
				case 1:
					cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)] = (uint8_t)(tmp_data&0xFF);
					if(L1_WRITEBACK)
						cache[hit_way][set_index].clean[(tmp_addr&offset_mask)>>2] = false;
					else
						printf("bus access: write %d to %d\n",cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)],tmp_addr);
					
#ifndef STANDALONE
							bus_b_access(cid,true,tmp_addr, 
							(unsigned char*)(&(cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)])), 1);
							// [FIXME]: Connect with bus 
#endif
						
					break;
				case 2:
					((cache[hit_way][set_index].line.half[(tmp_addr&offset_mask)>>1])) = (uint16_t)(tmp_data&0xFFFF);
					if(L1_WRITEBACK)
						cache[hit_way][set_index].clean[(tmp_addr&offset_mask)>>2] = false;
					else
						printf("bus access: write %d to %d\n",cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)],tmp_addr);
#ifndef STANDALONE
						bus_b_access(cid,true, tmp_addr, 
						(unsigned char*)(&(cache[hit_way][set_index].line.half[(tmp_addr&offset_mask)>>1])), 2);
						// [FIXME]: Connect with bus 
#endif
					break;
				case 4:
					((cache[hit_way][set_index].line.word[(tmp_addr&offset_mask)>>2])) = (uint32_t)(tmp_data&0xFFFFFFFF);
					if(L1_WRITEBACK)
						cache[hit_way][set_index].clean[(tmp_addr&offset_mask)>>2] = false;
					else
						printf("bus access: write %d to %d\n",cache[hit_way][set_index].line.byte[(tmp_addr&offset_mask)],tmp_addr);
#ifndef STANDALONE
						bus_b_access(cid,true, tmp_addr, 
						(unsigned char*)(&(cache[hit_way][set_index].line.word[(tmp_addr&offset_mask)>>2])), 4);
						// [FIXME]: Connect with bus 
#endif
					break;
				default:;
			}

		}
		else if(cmd == 2)
		{
			invalid_cache_line();

		}
		return tmp_data;
}

inline bool armv7a_L1_cache::compare_tag(uint32_t addr)
{
	bool hit = false;
	int i;
	uint32_t index;

	index = (addr&index_mask)>>((uint32_t)(log2(offset_mask+1)));
	for(i=0;i<L1_CACHE_WAY;i++)
	{
		if(cache[i][index].valid == true)
		{
			if(cache[i][index].tag == (addr & tag_mask))
			{
				hit_way = i;
				hit = true;
				return hit;
			}
			else
				hit = false;
		}
		else
			hit = false;
	}
	
	return hit;
}

inline void armv7a_L1_cache::handle_miss(uint32_t addr)
{
	
	int i;	
	uint32_t data_get[L1_CACHE_LINE_SIZE];
	uint32_t data_writeback[L1_CACHE_LINE_SIZE];
	bool clean_bit[L1_CACHE_LINE_SIZE];
	uint32_t addr_get;
	uint32_t addr_writeback;
	uint32_t index;
	bool all_line_clean;
	printf("handle miss \n");
	addr_get = addr&(~offset_mask);
	index = (addr&index_mask) >> ((uint32_t)(log2(offset_mask+1)));
	hit_way = rp_flag[index];
	if(hit_way >= L1_CACHE_WAY)
	rp_flag[index] = (rp_flag[index]+1) % L1_CACHE_WAY; /* Round Robin Replace Policy */
	
	all_line_clean = true;
	for(i=0;i<L1_CACHE_LINE_SIZE;i++)
	{
		if(cache[hit_way][index].clean[i] == false)
		{
			all_line_clean = false;
			break;
		}
	}
	
	if((cache[hit_way][index].valid == true) && (all_line_clean == false)) /* we should write back the dirty data */
	{
		eviction++;
		addr_writeback = (cache[hit_way][index].tag) | (addr & index_mask);
		/* we use burst write */
		for(i=0;i<L1_CACHE_LINE_SIZE;i++)
		{
			data_writeback[i] = cache[hit_way][index].line.word[i];
			clean_bit[i] = cache[hit_way][index].clean[i];
		}
#ifndef STANDALONE
		bus_b_access(cid,true, (addr_writeback), (unsigned char*)(data_writeback), L1_CACHE_LINE_SIZE*4);
		// [FIXME]: Connect with bus 
#endif
		printf("bus access: write %d to %d\n",*data_writeback,addr);
	}
#ifndef STANDALONE
	bus_b_access(cid,false, (addr_get), 
		// [FIXME]: Connect with bus 
	reinterpret_cast<unsigned char*>(data_get), L1_CACHE_LINE_SIZE*4);
#endif
	//}
	printf("bus access: read %d \n",addr);
	// [FIXME]: Connect with bus 
	for(i=0;i<L1_CACHE_LINE_SIZE;i++)
	{
		cache[hit_way][index].line.word[i] = data_get[i];
		cache[hit_way][index].clean[i] = true;
	}

	cache[hit_way][index].tag = addr & tag_mask;
	cache[hit_way][index].valid = true;
}

inline void armv7a_L1_cache::invalid_cache_line()
{
	int i,j,k;
	uint32_t addr_writeback;
	uint32_t data_writeback[L1_CACHE_LINE_SIZE];
	bool clean_bit[L1_CACHE_LINE_SIZE];
	bool all_line_clean;
	for(i=0;i<L1_CACHE_WAY;i++)
	{
		for(j=0;j<L1_SET;j++)
		{
			if(L1_WRITEBACK && (!is_i))
			{
				all_line_clean = true;
				/* we should write back the dirty data */
				if(cache[i][j].valid == true)
				{
					addr_writeback = (cache[i][j].tag) | ( j << ((uint32_t)(log2(offset_mask+1))) );		
					for(k=0;k<L1_CACHE_LINE_SIZE;k++)
					{
						data_writeback[k] = cache[i][j].line.word[k];
						if(!cache[i][j].clean[k])
							all_line_clean = false;
					}
					if(!all_line_clean)
					{
#ifndef STANDALONE
						bus_b_access(cid,true, (addr_writeback), 
						(unsigned char*)(data_writeback), L1_CACHE_LINE_SIZE*4);
						// [FIXME]: Connect with bus 
#endif
						printf("bus access: write %d to %d\n",*data_writeback,addr);
						// [FIXME]: Connect with bus 
					}
				}
			}
			cache[i][j].valid = false;
		}
	}
}

void bus_b_access(int master_id,bool write, uint32_t addr, unsigned char* data, unsigned int length)
{

	printf("TLM interface should be implemented\n");
}