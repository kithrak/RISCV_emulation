/*
 * symbolTable.cpp
 *
 *  Created on: 22-Dec-2023
 *      Author: karthik
 */

#include "symbolTable.h"
#include "risc_machine.h"

void SymbolTable::addSymbol(const std::string& name, int value)
{
	symbol[name] = value;
}

int SymbolTable::getVariableValue(const std::string& name)
{
	auto it = symbol.find(name);
	if (it != symbol.end())
	{
		return it->second;
	}
	else
	{
		//std::cout << "no such symbol\n" << std::endl;
		return NO_SYMBOL_FOUND;
	}
}

void SymbolTable::updateSymbol(const std::string& name, int value)
{
	symbol[name] = value;
}

