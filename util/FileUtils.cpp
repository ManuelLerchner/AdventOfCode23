#include "FileUtils.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string FileUtils::getDataFolder() {
#ifdef ADVENT_OF_CODE_DATA_DIR
  return ADVENT_OF_CODE_DATA_DIR;
#else
  throw std::runtime_error("ADVENT_OF_CODE_DATA_DIR not defined");
#endif
}

std::stringstream FileUtils::loadFile(const std::string& path) {
  auto fullPath = getDataFolder() + path;

  std::ifstream file(fullPath);

  auto stringStream = std::stringstream();
  stringStream << file.rdbuf();

  return stringStream;
}

std::ofstream FileUtils::createOutputFile(const std::string& path) {
  auto fullPath = getDataFolder() + path;

  std::ofstream file(fullPath);

  return file;
}

void FileUtils::createOutputFile(const std::string& path, const std::string& content) {
  auto fullPath = getDataFolder() + path;

  auto file = createOutputFile(fullPath);

  file << content;
}
