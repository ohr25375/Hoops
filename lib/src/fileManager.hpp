#pragma once

#include <vector>
#include <fstream>
#include <iostream>

bool readBinary(const std::string& path, std::vector<std::byte>& out);
void writeBinary(const std::string& path, const std::vector<std::byte>& data);