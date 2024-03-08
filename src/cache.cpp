/*
 * cache.cpp
 *
 *  Created on: 27-Dec-2023
 *      Author: karthik
 */
#include <cache.hpp>

void LRU::put(int address, int value)
{
	auto it = cacheMap.find(address);

	if(it != cacheMap.end())
	{
		/*
		 * Address already exists
		 * Move address to the top of the LRU list
		 */
		it->second->second = value;
		cacheList.splice(cacheList.begin(), cacheList, it->second);
	}
	else
	{
		/*
		 ; If the cache is full, Remove the Least recently Used item
		 */
		if(cacheList.size() == capacity)
		{
			int leastUsedKey = cacheList.back().first;
			cacheMap.erase(leastUsedKey);
			cacheList.pop_back();
		}
		/*
		 * Insert the new value to front of the List
		 */
		cacheList.emplace_front(address, value);
		cacheMap[address] = cacheList.begin();
	}
}

std::list<std::pair<int, int>>::iterator LRU::get(int address)
{
	auto it = cacheMap.find(address);

	/*
	 * if Address is not present return
	 */
	if (it == cacheMap.end())
	{
		return cacheList.end(); // TODO convert to #define
	}

	//Update the address node to the front of the list
	cacheList.splice(cacheList.begin(), cacheList, it->second);

	return it->second;
}

int LRU::getDatafromCache(Memory &memory,int address)
{
	auto it = get(address);
	if(it == cacheList.end())
	{
		put(address,memory.data[address]);
	}
	return get(address)->second;
}

void LRU::putDataToMemoryAndCache(Memory &memory,int address, int value)
{
	put(address,value);
	memory.data[address] = value;
}
