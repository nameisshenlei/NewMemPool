#include "windows.h"

#include "MemPool.h"
#include "TPacket.h"

int main()
{
	MemPool<TPacket>* pMemPool = MemPool<TPacket>::CreateMemPool(20, 2);
	TPacket* pkt = pMemPool->Pull(10000);
	pkt->Unref();
	pMemPool->Push(pkt);
	return 0;
}