#include "AquaCrop/Utils.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>

namespace AquaCrop {

void assert_true(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "ABORT: " << message << std::endl;
        exit(1);
    }
}

void upper_case(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void write_file(int fhandle, const std::string& line, bool advance, int& iostat) {
    // This is a placeholder implementation.
    // In a real application, you would use the file handle to write to the file.
    // For now, we will just print to standard output.
    if (advance) {
        std::cout << line << std::endl;
    } else {
        std::cout << line;
    }
    iostat = 0;
}

std::string int2str(int i) {
    return std::to_string(i);
}

void open_file(int& fhandle, const std::string& filename, char mode, int& iostat) {
    // This is a placeholder implementation.
    // In a real application, you would use the file handle to open the file.
    // For now, we will just set the file handle to a dummy value.
    fhandle = 1;
    iostat = 0;
}

dp roundc(dp value, int mold)
{
    dp x_clipped;

    if (value > 2147483647.0) {
        x_clipped = 2147483647.0;
    } else if (value < -2147483648.0) {
        x_clipped = -2147483648.0;
    } else {
        x_clipped = value;
    }

    if (std::abs(x_clipped - std::floor(x_clipped) - 0.5) < 1e-9) {
        if (x_clipped > 0) {
            if (static_cast<int64_t>(std::abs(trunc(x_clipped))) % 2 == 0) {
                return std::floor(x_clipped);
            } else {
                return std::ceil(x_clipped);
            }
        } else {
            if (static_cast<int64_t>(std::abs(trunc(x_clipped))) % 2 == 0) {
                return std::ceil(x_clipped);
            } else {
                return std::floor(x_clipped);
            }
        }
    } else {
        return std::round(x_clipped);
    }
}

std::string GetReleaseDate() {
    return "August 2024";
}

std::string GetVersionString() {
    return "7.2";
}

int32_t trunc(dp value)
{
    if (value > 0) {
        return static_cast<int32_t>(std::floor(value));
    } else {
        return static_cast<int32_t>(std::ceil(value));
    }
}

} // namespace AquaCrop
