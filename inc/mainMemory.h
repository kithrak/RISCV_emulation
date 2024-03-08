/*
 * memory.h
 *
 *  Created on: 26-Dec-2023
 *      Author: karthik
 */

#ifndef MAINMEMORY_H_
#define MAINMEMORY_H_


#include <stdint.h>

#define MAX_DATA_MEMORY 1024
#define MAX_TEXT_MEMORY 1024

/*
 * Using single Memory bank for code as well memory operation
 * Text :- Contains the code to be executed
 * Memory :- Used to memory operations required for the code
 */
struct Memory
{

	uint32_t text[MAX_TEXT_MEMORY];
	uint32_t data[MAX_DATA_MEMORY];

	int nextFreeData;
	int nextFreeText;

public:
	Memory();

};




#endif /* MAINMEMORY_H_ */
