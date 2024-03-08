/*
 * symbol.h
 *
 *  Created on: 22-Dec-2023
 *      Author: karthik
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <iostream>
#include <unordered_map>
#include <string>

class SymbolTable
{
private:
	std::unordered_map<std::string, int> symbol;

public:

	void addSymbol(const std::string& name, int value);
	int getVariableValue(const std::string& name);
	void updateSymbol(const std::string& name, int value);

};



#endif /* SYMBOLTABLE_H_ */
