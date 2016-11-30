#include "ObjectBase.h"

ObjectBase::ObjectBase(uint8_t* pData, uint32_t iBuffSize, MemPool<ObjectBase>* pPool /* = nullptr */)
	: m_pData(pData)
	, m_iBuffSize(iBuffSize)
	, m_pPool(pPool)
{

}

ObjectBase::~ObjectBase()
{
	m_pData = nullptr;
}

uint8_t* ObjectBase::GetData() const
{
	if (m_iRefCount == 0)
		return nullptr;
	return m_pData;
}

void ObjectBase::RefPacket()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);
	m_iRefCount++;
}

void ObjectBase::UnrefPacket()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);
	--m_iRefCount;
}

bool ObjectBase::PacketValid()
{
	return !!m_iRefCount;
}

bool ObjectBase::AppendData(const uint8_t* pData, const int iSize)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);
	if (m_iRefCount != 0)
		return false;
	++m_iRefCount;
	memcpy(m_pData, pData, iSize);
	return true;
}

ObjectBase* ObjectBase::Ref()
{
	if (m_iRefCount == 0)
		return nullptr;
	RefPacket();
	return this;
}

void ObjectBase::Unref()
{
	if (m_pPool)
	{
		m_pPool->Push(this);
	}
	else
	{
		DeleteThis(this);
	}
}

void ObjectBase::DeleteThis(ObjectBase* pThis)
{
	delete pThis;
}