#include "armv7a_L1_cache.h"
int main()
{

	// Initialization: Cache
	armv7a_L1_cache* dcache=new armv7a_L1_cache( 0, true);
	
	// cmd data addr size
	//cmd =0 read cmd=1 write cmd=2 invalid
	uint32_t x;
	dcache->run(1,123,0x204,4);
	x = dcache->run(0,123,0x204,4);
	printf("==>Return:%d\n",x);

	dcache->run(1,998,0x212,4);
	x = dcache->run(0,456,0x212,4);
	printf("==>Return:%d\n",x);

	dcache->run(1,528,0x3ff,4);
	x = dcache->run(0,123,0x3ff,4);
	printf("==>Return:%d\n",x);

	// Test OUr Magic Spell is work or not.
	dcache->run(1,1,0x76543210 ,1);
}
