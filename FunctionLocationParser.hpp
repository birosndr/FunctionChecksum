#ifndef __FUNCTION_LOCATION_PARSER_HPP__
#define __FUNCTION_LOCATION_PARSER_HPP__
#pragma once

#include <string>
#include <vector>

struct FunctionLocation {
	std::string		functionName;
	unsigned int	line{0};
	std::string		functionFile;
	std::string		functionHeader;
};


class FunctionsLocationParser {
private:
	std::vector<FunctionLocation>	functionLocations;
	std::string						ctagFileName;

public:
	FunctionsLocationParser (const std::string& ctagFileName) : ctagFileName (ctagFileName) {}

	bool	Parse ();

	const std::vector<FunctionLocation>& GetFunctionLocations () const { return functionLocations; }
};

#endif // !__FUNCTION_LOCATION_PARSER_HPP__