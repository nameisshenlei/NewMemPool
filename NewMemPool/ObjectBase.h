#ifndef _OBJECT_BASE_H__
#define _OBJECT_BASE_H__

#include "stdint.h"
#include "mutex"

#include "MemPool.h"

class ObjectBase{
public:
	ObjectBase(uint8_t* pData, uint32_t iBuffSize, MemPool<ObjectBase>* pPool = nullptr);

	virtual ~ObjectBase();

	//如果m_iRefCount为0，标明数据无效，返回空，并且引用计数也不增加
	ObjectBase* Ref();
	void Unref();
	bool PacketValid();

	//如果m_iRefCount不为0，表明已有有效数据，不做拷贝，返回false
	bool AppendData(const uint8_t* pData, const int iSize);
	virtual uint8_t* GetData() const;

	friend class MemPool<ObjectBase>;

private:
	ObjectBase() {}
	ObjectBase& operator=(const ObjectBase&) { return *this; }
	ObjectBase(const ObjectBase&) {}

	static void DeleteThis(ObjectBase* pThis);

protected:
	void  RefPacket();
	virtual void UnrefPacket();

	uint8_t*     m_pData = nullptr;
	uint32_t     m_iBuffSize = 0;

	uint32_t     m_iRefCount = 0;
	std::mutex   m_mutexLock;

	MemPool<ObjectBase>* m_pPool = nullptr;
};

#endif//_OBJECT_BASE_H__
