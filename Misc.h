//#include "types.h"

int systemEx(std::string cmd);
static bool isValidFloat(const std::string& string);
static bool isValidInt(const std::string& string);
static bool isValidFloatExt(const std::string& string, float min, float max);
static bool isValidIntExt(const std::string& string, int min, int max);
void memcpyExt( void* dest, void*src , int size, int* offset );
