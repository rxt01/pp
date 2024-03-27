#pragma once

#include <vector>

namespace Rxt {
    std::vector<unsigned char> read(const char* filepath);

    void write(const char* filepath, std::vector<unsigned char> content);
}