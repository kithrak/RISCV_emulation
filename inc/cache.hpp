/*
 * class.hpp
 *
 *  Created on: 27-Dec-2023
 *      Author: karthik
 */

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <iostream>
#include <list>
#include <unordered_map>
#include <iterator>

#include <mainMemory.h>

//using namespace std;

#define MAX_CACHE_CAPACITY 4

class LRU
{
public:
	LRU(): capacity(MAX_CACHE_CAPACITY){}
	LRU(int capacity): capacity(capacity) {}
	std::list<std::pair<int, int>>::iterator get(int address);
	void put(int address, int value);
	int  getDatafromCache(Memory &memory,int address);
	void putDataToMemoryAndCache(Memory &memory,int address, int value);
private:
	uint32_t capacity;          // cache size
	std::list<std::pair<int,int>> cacheList; // Stores address in LRU order
	std::unordered_map<int, std::list<std::pair<int,int>>::iterator> cacheMap; // Map for quick access
};



#endif /* CACHE_HPP_ */
