#pragma once

#include <fstream>
#include <string>

class FileUtils {
 public:
  static std::string getDataFolder();

  static std::stringstream loadFile(const std::string& path);

  static std::ofstream createOutputFile(const std::string& path);

  static void createOutputFile(const std::string& path, const std::string& content);
};