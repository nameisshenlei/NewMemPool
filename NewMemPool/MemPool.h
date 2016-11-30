#ifndef _MEM_POLL_H__
#define _MEM_POLL_H__

#include "stdint.h"
#include "list"
#include "mutex"

#define MILLISECONDS(time)	std::chrono::milliseconds(time)

template <class T>
class MemPool {
public:
	static MemPool<T>* CreateMemPool(const int iChunkSize, const int iChunkCount = 50)
	{
		return new MemPool(iChunkSize, iChunkCount);
	}
	static void DestroyMemPool()
	{
		delete this;
	}

	T* Pull()
	{
		if (!m_listFreeObject.empty())
		{
			std::lock_guard<std::timed_mutex> lock(m_lockList);
			if (!m_listFreeObject.empty())
			{
				T* pkt = m_listFreeObject.front();
				m_listFreeObject.pop_front();
				pkt->RefPacket();

				m_listBusyObject.push_back(pkt);
				return pkt;
			}
		}
		return nullptr;
	}

	void Push(T* pkt)
	{
		pkt->UnrefPacket();
		if (!pkt->PacketValid())
		{
			std::lock_guard<std::timed_mutex> lock(m_lockList);
			std::list<T*>::iterator iter = find(m_listBusyObject.begin(), m_listBusyObject.end(), pkt);
			if (iter == m_listBusyObject.end())
				return;
			m_listBusyObject.remove(pkt);
			m_listFreeObject.push_back(pkt);
		}
	}

	//毫秒为单位,0表示永远阻塞
	T* Pull(uint32_t iWaitTime)
	{
		if (!iWaitTime)
			return Pull();

		T* pkt = nullptr;
		if (!m_listFreeObject.empty())
		{
			std::unique_lock<std::timed_mutex> lock(m_lockList, std::defer_lock);
			if (!lock.try_lock_for(MILLISECONDS(iWaitTime)))
				return nullptr;
			if (!m_listFreeObject.empty())
			{
				pkt = m_listFreeObject.front();
				m_listFreeObject.pop_front();
				pkt->RefPacket();

				m_listBusyObject.push_back(pkt);
			}
		}
		return pkt;
	}

	bool Push(T* pkt, uint32_t iWaitTime)
	{
		pkt->UnrefPacket();
		if (!pkt->PacketValid())
		{
			std::unique_lock<std::timed_mutex> lock(m_lockList, std::defer_lock);
			if (!lock.try_lock_for(MILLISECONDS(iWaitTime)))
				return false;
			std::list<T*>::iterator iter = find(m_listBusyObject.begin(), m_listBusyObject.end(), pkt);
			if (iter == m_listBusyObject.end())
				return true;
			m_listBusyObject.remove(pkt);
			m_listFreeObject.push_back(pkt);
		}
		return true;
	}

private:
	MemPool() {}
	MemPool(const int iChunkSize, const int iChunkCount)
		: m_iChunkSize(iChunkSize)
		, m_iChunkCount(iChunkCount)
	{
		m_pMemory = new uint8_t[iChunkCount * iChunkSize];
		memset(m_pMemory, 0, iChunkSize * iChunkCount);
		for (int i = 0; i < iChunkCount; i++)
		{
			m_listFreeObject.push_back(new T(m_pMemory + i * iChunkSize, iChunkSize));
		}
	}

	MemPool& operator= (const MemPool&) {return *this;}
	MemPool(const MemPool&)	{}
	~MemPool()
	{
		std::lock_guard<std::mutex> lock(m_lockList);
		{
			while (!m_listFreeObject.empty())
			{
				T* pObj = m_listFreeObject.front();
				m_listFreeObject.pop_front();
				delete pObj;
			}
		}
		{
			while (!m_listBusyObject.empty())
			{
				T* pObj = m_queBusyObject.front();
				m_listBusyObject.pop_front();
				delete pObj;
			}
		}
		delete[] m_pMemory;
		m_pMemory = nullptr;
	}

private:
	const int        m_iChunkSize;
	const int        m_iChunkCount;
	uint8_t*         m_pMemory = nullptr;

	std::timed_mutex m_lockList;
	std::list<T*>    m_listFreeObject;
	std::list<T*>    m_listBusyObject;
};

#endif//_MEM_POLL_H__