#pragma once
#include <string>

namespace Seed {

void Asset_Name(const std::string &f, std::string &filename);
void Read_File(const std::string &f, std::string &s);

std::string GetConfigDir();
bool CreateDirs(const std::string &path);
} // namespace Seed
