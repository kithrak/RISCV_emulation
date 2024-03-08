/*
 * programLoad.h
 *
 *  Created on: 22-Dec-2023
 *      Author: karthik
 */

#ifndef PROGRAMLOAD_H_
#define PROGRAMLOAD_H_

#include <iostream>
#include <fstream>
#include <string>

#include "risc_machine.h"
#include "symbolTable.h"
#include "registers.h"
#include "instruction.h"

#define SECTOR_SEGMENT 0
#define TEXT_SEGMENT 1

int loadInstructionSetToMemory(Machine &machine, std::string &inputFileNames);
int loadProgramToMemory(Machine &machine, std::string &inputFileNames);

/*
 *  Below function will be compiled only for test environments
 *
 */
int loadOperand(Memory &memory, SymbolTable &symbol, int textCounter , std::string s);

#endif /* PROGRAMLOAD_H_ */
