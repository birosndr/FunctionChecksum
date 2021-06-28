#ifndef __FUNCTIONCHECKSUMGENERATOR_HPP__
#define __FUNCTIONCHECKSUMGENERATOR_HPP__
#pragma once

#include <string>
#include <unordered_map>

#include "FunctionLocationParser.hpp"

class FunctionChecksumGenerator {
private:
	const std::vector<FunctionLocation>& functionLocations;
	std::string sourceFile;
	unsigned long fileLine{0};


	std::vector<std::pair<FunctionLocation, unsigned int>> functionChecksums;
	std::vector<std::pair<std::string, std::string>> functionStrains;

public:
	FunctionChecksumGenerator (const std::string& sourceFile, const std::vector<FunctionLocation>& functionLocatoins) :
		sourceFile (sourceFile), functionLocations (functionLocatoins) {}

	const std::vector<std::pair<FunctionLocation, unsigned int>>& GetFunctionChecksums () const { return functionChecksums; }
	const std::vector<std::pair<std::string, std::string>>& GetFunctionStrains () const { return functionStrains; }

	bool Generate ();

private:
	std::string GetFunctionStr (const unsigned int lineStart, const unsigned int lineEnd, std::ifstream& inFile);
	std::string GetFunctionStrain (const std::string& functionStr);
	void GenerateFunctionChecksum (const FunctionLocation& function, unsigned int nextFunctionLine, std::ifstream& inFile);

};

#endif // !__FUNCTIONCHECKSUMGENERATOR_HPP__
