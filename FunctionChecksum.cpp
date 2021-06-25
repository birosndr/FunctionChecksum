#include <iostream>
#include <fstream>

#include "FunctionLocationParser.hpp"
#include "FunctionChecksumGenerator.hpp"

#include "CRC32Utility.hpp"
#include "FunctionChecksum.h"

//#define PRINT2OUTPUT

std::vector<std::pair<FunctionLocation, unsigned int>> GenerateFunctionsChecksum (const std::string& ctagFileName, const std::string& sorurceFileName)
{
	FunctionsLocationParser parser (ctagFileName);
	parser.Parse ();

#ifdef PRINT2OUTPUT
	for (const auto& act : parser.GetFunctionLocations ()) {
		std::cout << act.functionName << " " << act.line << " " << act.functionFile << " " << act.functionHeader << std::endl;
	}
#endif // PRINT2OUTPUT

	FunctionChecksumGenerator generator (sorurceFileName, parser.GetFunctionLocations ());
	generator.Generate ();

#ifdef PRINT2OUTPUT
	for (const auto& act : generator.GetFunctionChecksums ()) {
		std::cout << act.first.functionHeader << " " << act.second << std::endl;
	}
#endif // PRINT2OUTPUT

	return generator.GetFunctionChecksums ();
}


std::vector<std::pair<FunctionLocation, unsigned int>> FilterSourceChecksumsByName (const std::string& functionName, const std::vector<std::pair<FunctionLocation, unsigned int>>& sourceChecksums)
{
	std::vector<std::pair<FunctionLocation, unsigned int>> result;
	for (const auto& act : sourceChecksums) {
		if (act.first.functionName == functionName)
			result.push_back (act);
	}
	return result;
}


bool IsFunctionDiffer (const std::pair<FunctionLocation, unsigned int>& chAct, const std::vector<std::pair<FunctionLocation, unsigned int>>& baseChecksums)
{
	std::vector<std::pair<FunctionLocation, unsigned int>> filteredBaseChecksums = FilterSourceChecksumsByName (chAct.first.functionName, baseChecksums);

	for (std::size_t i = 0; i < filteredBaseChecksums.size (); i++) {
		if (chAct.first.functionHeader == filteredBaseChecksums[i].first.functionHeader) {
			if (chAct.second != filteredBaseChecksums[i].second)
				return true;
		}
	}

	return false;
}


std::vector<FunctionLocation> GenerateDifference (const std::string& ctagFile1Name, const std::string& baseSorurceFileName, const std::string& ctagFile2Name, const std::string& changedSorurceFileName)
{
	std::vector<std::pair<FunctionLocation, unsigned int>> baseSourceChecksums = GenerateFunctionsChecksum (ctagFile1Name, baseSorurceFileName);
	std::vector<std::pair<FunctionLocation, unsigned int>> changedSourceChecksums = GenerateFunctionsChecksum (ctagFile2Name, changedSorurceFileName);

	std::vector<FunctionLocation> differentFunctions;
	for (const auto& chAct : changedSourceChecksums) {
		if (IsFunctionDiffer (chAct, baseSourceChecksums)) {
			differentFunctions.push_back (chAct.first);
		}
	}
	return differentFunctions;
}


bool CheckFiles (char* const argv[])
{
	bool result = true;
	for (std::size_t i = 1; i <= 4; i++) {
		std::ifstream inFile;
		inFile.open (argv[i]);
		if (!inFile.is_open ()) {
			std::cout << "Cannot open " << argv[i] << std::endl;
			result = false;
		}
	}
	return result;
}


int main (int argc, char* const argv[])
{
	if (argc < 5) {
		std::cout << "Use FunctionDifference <CTag functions file> <base source file> <CTag functions file> <changed source file> in parameter!" << std::endl << "\t Use CTag with \"ctags -x --c-types=f --format=1 <source file>\"" << std::endl;
		return 0;
	}

	if (CheckFiles (argv)) {
		std::vector<FunctionLocation> differentFunctions = GenerateDifference (argv[1], argv[2], argv[3], argv[4]);
		for (const auto& act : differentFunctions) {
			std::cout << act.functionName << " " << act.functionFile << std::endl;
		}
		return 0;
	}
	return 1;
}
