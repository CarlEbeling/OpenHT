#include "Ht.h"
using namespace Ht;

#define MEM_QW_SIZE     64

int main(int argc, char **argv)
{
	CHtHif *pHtHif = new CHtHif();
	CHtAuUnit *pAuUnit = new CHtAuUnit(pHtHif);

	printf("#AUs = %d\n", pHtHif->GetUnitCnt());
	fflush(stdout);

	// host memory
	uint64_t *pHostMem;
	ht_posix_memalign((void **)&pHostMem, 64, MEM_QW_SIZE * sizeof(uint64_t));

	for (int i = 0; i < MEM_QW_SIZE; i++)
		pHostMem[i] = 0xdeadbeef00aa0000 | i;

	pAuUnit->SendHostMsg(HOST_MEM_ADDR, (uint64_t)pHostMem);

	// coproc memory
	uint64_t *pCoprocMem = (uint64_t*)pHtHif->MemAllocAlign(64, MEM_QW_SIZE * sizeof(uint64_t));

	for (int i = 0; i < MEM_QW_SIZE; i++)
		pCoprocMem[i] = 0x01234567deadbeefULL + i;

	pAuUnit->SendHostMsg(COPROC_MEM_ADDR, (uint64_t)pCoprocMem);

	// start test
	pAuUnit->SendCall_htmain();

	// wait for return
	bool rtn_passed;
	while (!pAuUnit->RecvReturn_htmain(rtn_passed))
		usleep(1000);

	delete pHtHif;

	if (!rtn_passed)
		printf("FAILED\n");
	else
		printf("PASSED\n");

	return rtn_passed ? 0 : -1;
}
