#include <sstream>
#include <istream>
#include <fstream>
#include <algorithm>

#include "FunctionLocationParser.hpp"




bool FunctionsLocationParser::Parse ()
{
	std::ifstream inFile;
	inFile.open (ctagFileName);
	if (inFile.is_open ()) {
		std::string line;
		while (std::getline (inFile, line)) {
			std::stringstream lineStream (line);
			std::string parsed;
			FunctionLocation funcLocation;
			while (std::getline (lineStream, parsed, ' ')) {
				if (funcLocation.functionName.empty ()) {
					funcLocation.functionName = parsed;
				} else if (funcLocation.line == 0) {
					funcLocation.line = std::strtoul (parsed.c_str (), nullptr, 0);
				} else if (funcLocation.functionFile.empty ()) {
					funcLocation.functionFile = parsed;
				} else {
					funcLocation.functionHeader.append (parsed);
				}
			}
			functionLocations.push_back (funcLocation);
		}

		std::sort (functionLocations.begin (), functionLocations.end (), [](const FunctionLocation& a, const FunctionLocation& b) { return a.line < b.line; });

		return true;
	}
	return false;
}

