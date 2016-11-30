#ifndef _T_PACKET_H__
#define _T_PACKET_H__

#include "mutex"

#include "ObjectBase.h"

class TPacket : public ObjectBase{
public:
	TPacket(uint8_t* pData, uint32_t iBuffSize);
	void        SetPts(const int64_t iPts);
	int64_t     GetPts() const;

	virtual void UnrefPacket();

	friend class MemPool<TPacket>;
private:
	int64_t     m_iPts = 0;
};

#endif//_T_PACKET_H__
