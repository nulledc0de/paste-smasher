#pragma once

#include <string>
#include <fstream>
#include "../../dependencies/xor/xor.h"

namespace utils
{
	inline void log_to_file(const char* format, ...)
	{
		std::ofstream file;

		file.open(E("paste_smasher.txt"), std::ios_base::app);

		char arr[256] = { 0 };

		va_list argList;
		va_start(argList, format);
		vsprintf_s(arr, format, argList);
		va_end(argList);

		file << arr;

		file.close();
	}
}