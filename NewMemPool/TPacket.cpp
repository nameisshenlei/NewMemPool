#include "TPacket.h"

TPacket::TPacket(uint8_t* pData, uint32_t iBuffSize)
	: ObjectBase(pData, iBuffSize)
{

}

void TPacket::SetPts(const int64_t iPts)
{
	m_iPts = iPts;
}

int64_t TPacket::GetPts() const
{
	return m_iPts;
}

void TPacket::UnrefPacket()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);
	--m_iRefCount;
	if (m_iRefCount == 0)
		m_iPts = 0;
}
