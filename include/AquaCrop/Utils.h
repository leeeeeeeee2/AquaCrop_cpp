#pragma once

#include "AquaCrop/Global.h"

namespace AquaCrop {

// Function declarations
void assert_true(bool condition, const std::string& message);
void upper_case(std::string& str);
void write_file(int fhandle, const std::string& line, bool advance, int& iostat);
std::string int2str(int i);
void open_file(int& fhandle, const std::string& filename, char mode, int& iostat);
dp roundc(dp value, int mold);
std::string GetReleaseDate();
std::string GetVersionString();
int32_t trunc(dp value);

} // namespace AquaCrop
