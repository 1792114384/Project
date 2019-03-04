#pragma once
#include "Common.h"
#include <string>

const USH HASH_BITS = 15;
const USH HASH_SIZE = (1 << HASH_BITS);
const USH HASH_MASK = HASH_SIZE - 1;

class HashTable
{
public:
	HashTable(size_t size)
		:_prev(new USH[size * 2])
		,_head(_prev + size)
		,_hashSize(size)
	{
		//��ʼ��ֻ�ó�ʼ��_head
		//��Ϊ�������_head�����ֵ���뵽_prev��
		memset(_head, 0, sizeof(USH) * size);
	}
	~HashTable()
	{
		if (_prev)
		{
			delete[] _prev;
			_prev = _head = nullptr;
		}
	}
	//����
	void Insert(USH& hashAddr, UCH ch, USH pos, USH& matchHead)
	{
		HashFunc(hashAddr, ch);

		_prev[pos & HASH_MASK] = _head[hashAddr];
		matchHead = _head[hashAddr];
		_head[hashAddr] = pos;
	}

	void HashFunc(USH& hashAddr, UCH ch)
	{
		//HASH_MASHʵ���Ͼ��� 01111111 11111111����Ϊֻ��15��bitλ�����԰����λ��0
		hashAddr = (((hashAddr) << H_SHIFT()) ^ (ch)) & HASH_MASK;
	}

	USH GetNextPos(USH matchPos)
	{
		return _prev[matchPos];
	}
	//���¹�ϣ��
	void UpdateHashTable()
	{
		//���Ҵ��ڵ����ݸ��µ���
		//����_head[]
		for (size_t i = 0; i < HASH_SIZE; ++i)
		{
			if (_head[i] >= WSIZE)
				_head[i] -= WSIZE;
			else
				_head[i] = 0;
		}
		//����_prev[]
		for (size_t i = 0; i < WSIZE; ++i)
		{
			if (_prev[i] >= WSIZE)
				_prev[i] -= WSIZE;
			else
				_prev[i] = 0;
		}
	}

private:
	USH H_SHIFT()
	{
		//ʵ���Ͼ��Ƿ��� 5
		return (HASH_BITS + MIN_MATCH - 1) / MIN_MATCH;
	}
	
private:
	USH* _prev;
	USH* _head;
	size_t _hashSize;
};
