#include "FunctionChecksumGenerator.hpp"
#include "CRC32Utility.hpp"

#include <sstream>
#include <istream>
#include <fstream>


std::string FunctionChecksumGenerator::GetFunctionStr (const int lineStart, const int lineEnd, std::ifstream& inFile)
{
	std::string functionStrainStr;
	std::string lineStr;
	while (std::getline (inFile, lineStr)) {
		fileLine++;
		if (fileLine <= lineStart)
			continue;
		if (fileLine >= lineEnd)
			break;

		functionStrainStr.append (lineStr);
	}
	return functionStrainStr;
}


std::string FunctionChecksumGenerator::GetFunctionStrain (const std::string& functionStr)
{
	int countBracket = 0;
	bool isStarted = false;
	std::string functionStrainStr;
	for (std::size_t i = 0; i < functionStr.size (); i++) {
		if (functionStr[i] == '{')
			countBracket++;
		if (functionStr[i] == '}')
			countBracket--;
		if (countBracket == 1)
			isStarted = true;

		functionStrainStr.push_back (functionStr[i]);

		if (isStarted && countBracket == 0)
			break;
	}
	return functionStrainStr;
}


bool FunctionChecksumGenerator::Generate ()
{
	std::ifstream inFile;
	inFile.open (sourceFile);
	if (inFile.is_open ()) {
		for (std::size_t i = 0; i < functionLocations.size () - 1; i++) {
			GenerateFunctionChecksum (functionLocations[i], functionLocations[i + 1].line, inFile);
		}
		GenerateFunctionChecksum (functionLocations[functionLocations.size () - 1], UINT32_MAX, inFile);

		return true;
	}
	return false;
}


void FunctionChecksumGenerator::GenerateFunctionChecksum (const FunctionLocation& function, unsigned int nextFunctionLine, std::ifstream& inFile)
{
	std::string functionStrainStr = GetFunctionStr (function.line, nextFunctionLine, inFile);
	functionStrainStr = GetFunctionStrain (functionStrainStr);

	functionChecksums.push_back ({function, crc32c ((unsigned char*)functionStrainStr.c_str ())});
}
