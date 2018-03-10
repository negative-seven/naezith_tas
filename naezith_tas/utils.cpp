#include "utils.h"

std::string readFile(const char *filepath)
{
	std::ifstream file;
	std::ostringstream buffer;

	file.open(filepath);
	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}
