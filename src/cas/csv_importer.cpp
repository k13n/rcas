#include "cas/csv_importer.hpp"
#include "cas/key.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <stdexcept>


template<class VType>
cas::CsvImporter<VType>::CsvImporter(cas::Index<VType>& index, char delimiter)
    : index_(index)
    , delimiter_(delimiter)
    , highest_ref_(cas::DEFAULT_REF)
{ }


template<class VType>
void cas::CsvImporter<VType>::Load(const std::string& filename) {
  highest_ref_ = cas::DEFAULT_REF;
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::invalid_argument("file cannot be opened: " + filename);
  }
  std::string line;
  while (std::getline(infile, line)) {
    cas::Key<VType> key = ProcessLine(line);
    index_.Insert(key);
  }
}


template<class VType>
uint64_t cas::CsvImporter<VType>::BulkLoad(const std::string& filename) {
  std::deque<cas::Key<VType>> keys;
  highest_ref_ = cas::DEFAULT_REF;
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::invalid_argument("file cannot be opened: " + filename);
  }
  std::string line;
  while (std::getline(infile, line)) {
    cas::Key<VType> key = ProcessLine(line);
    keys.push_back(key);
  }
  return index_.BulkLoad(keys);
}


template<class VType>
cas::Key<VType> cas::CsvImporter<VType>::ProcessLine(const std::string& line) {
  cas::Key<VType> key;

  std::stringstream line_stream(line);
  std::string path;
  std::string value;
  std::string ref;
  std::getline(line_stream, path,  delimiter_);
  std::getline(line_stream, value, delimiter_);
  std::getline(line_stream, ref,   delimiter_);

  key.path_  = path;
  key.value_ = ParseValue(value);
  if (!ref.empty()) {
    key.ref_ = cas::RefFromString(ref);
  }

  highest_ref_ = key.ref_;

  return key;
}


template<>
cas::vint32_t cas::CsvImporter<cas::vint32_t>::ParseValue(std::string& value) {
  return std::stoi(value);
}
template<>
cas::vint64_t cas::CsvImporter<cas::vint64_t>::ParseValue(std::string& value) {
  return std::stoll(value);
}
template<>
std::string cas::CsvImporter<std::string>::ParseValue(std::string& value) {
  size_t pos = 0;
  while (pos < value.size() && value[pos] == ' ') {
    ++pos;
  }
  return value.substr(pos);
}


// explicit instantiations to separate header from implementation
template class cas::CsvImporter<cas::vint32_t>;
template class cas::CsvImporter<cas::vint64_t>;
template class cas::CsvImporter<cas::vstring_t>;
