/*
 * memory.cpp
 *
 *  Created on: 26-Dec-2023
 *      Author: karthik
 */

/* Memory related define */

#include <mainMemory.h>
#include <cstring>


Memory::Memory()
{
	memset(text,0,sizeof(text));
	memset(data,0,sizeof(data));

	nextFreeData = 0;
	nextFreeText = 0;
}
